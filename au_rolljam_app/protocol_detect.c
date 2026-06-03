#include "protocol_detect.h"
#include <lib/toolbox/level_duration.h>
#include <furi.h>
#include "signal_capture.h"
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

/* =========================================================================
 * Helpers
 * ========================================================================= */

#define TAG "ProtoDetect"

#define DURATION_DIFF(a, b) (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))

/* Timing tolerance macros */
#define IN_TOL(dur, target, delta) (DURATION_DIFF(dur, target) <= (delta))

/* ---- Protocol timing constants (from Momentum firmware decoders) ---- */

/* KeeLoq HCS300/301 */
#define KL_TE_SHORT  400
#define KL_TE_LONG   800
#define KL_TE_DELTA  180
#define KL_MIN_PREAMBLE 12
#define KL_DATA_BITS     64

/* Nice Flor-S */
#define NF_TE_SHORT  500
#define NF_TE_LONG   1000
#define NF_TE_DELTA  300
#define NF_HEADER_US (NF_TE_SHORT * 38)   /* ~19000 us low */
#define NF_START_HI  (NF_TE_SHORT * 3)    /* 1500 us high  */
#define NF_START_LO  (NF_TE_SHORT * 3)    /* 1500 us low   */
#define NF_STOP_HI   (NF_TE_SHORT * 3)    /* 1500 us high  */
#define NF_DATA_BITS  52

/* CAME Atomo */
#define CA_TE_SHORT  600
#define CA_TE_LONG   1200
#define CA_TE_DELTA  250
#define CA_DATA_BITS  62

/* Somfy Telis RTS */
#define ST_TE_SHORT  640
#define ST_TE_LONG   1280
#define ST_TE_DELTA  250
#define ST_DATA_BITS  56
#define ST_HW_SYNC_US (ST_TE_SHORT * 4)   /* 2560 us       */
#define ST_SW_SYNC_HI 4550               /* us            */

/* =========================================================================
 * Simple Manchester decoder
 * =========================================================================
 *
 * G.E. Thomas convention:
 *   Bit 1 = high-to-low transition  (first half HIGH, second half LOW)
 *   Bit 0 = low-to-high transition  (first half LOW,  second half HIGH)
 *
 * LongHigh (2TE high) at boundary → bit=1, stay at boundary
 * LongLow  (2TE low)  at boundary → bit=0, stay at boundary
 * ShortX   (1TE) at boundary → record first-half level, go to mid
 * ShortX   (1TE) at mid      → decode bit from first_half + level, go to boundary
 * ========================================================================= */

typedef enum {
    MANCH_BOUNDARY = 0,
    MANCH_MID,
} ManchesterPhase;

typedef struct {
    ManchesterPhase phase;
    bool first_half_level;
} ManchesterState;

static void manchester_reset(ManchesterState* s) {
    s->phase = MANCH_BOUNDARY;
    s->first_half_level = false;
}

/* Returns: 1 = bit decoded (value in *bit), 0 = no bit yet, -1 = error */
static int manchester_feed(
    ManchesterState* s,
    bool level,
    uint32_t duration,
    uint32_t te_short,
    uint32_t te_delta,
    bool* bit) {
    bool is_short = IN_TOL(duration, te_short, te_delta);
    bool is_long = IN_TOL(duration, te_short * 2, te_delta * 2);

    if(!is_short && !is_long) return -1; /* invalid timing */

    if(s->phase == MANCH_BOUNDARY) {
        if(is_long) {
            /* Long pulse at boundary → direct bit decode */
            *bit = level; /* HIGH = bit 1, LOW = bit 0 (G.E. Thomas) */
            return 1;
        } else {
            /* Short pulse → first half of bit */
            s->first_half_level = level;
            s->phase = MANCH_MID;
            return 0;
        }
    } else { /* MANCH_MID */
        if(is_long) {
            /* Long pulse at mid-bit is invalid */
            return -1;
        }
        /* Short pulse → second half completes the bit */
        if(s->first_half_level && !level) {
            *bit = true;  /* HIGH then LOW = 1 */
        } else if(!s->first_half_level && level) {
            *bit = false; /* LOW then HIGH = 0 */
        } else {
            return -1; /* same level in both halves = error */
        }
        s->phase = MANCH_BOUNDARY;
        return 1;
    }
}

/* =========================================================================
 * KeeLoq detection
 * =========================================================================
 *
 * Preamble: 12+ alternating pulses of ~400us (high/low), 50% duty cycle
 * Header:   long LOW gap of ~4000us (10 × TE)
 * Data:     66 bits (64 data + 2 status), encoded as:
 *           bit 1 = short HIGH (1TE) + long LOW (2TE)
 *           bit 0 = long HIGH (2TE)  + short LOW (1TE)
 *
 * Data layout (64-bit word, MSB = first received bit):
 *   Bits [63:32] = hopping code (encrypted, transmitted first)
 *   Bits [31:0]  = fix part (serial + button, transmitted last)
 *
 * After bit-reversal (per Flipper convention):
 *   fix = upper 32 → btn(4) | serial(28)
 *   hop = lower 32 → encrypted(32)
 * ========================================================================= */

static uint64_t reverse_key(uint64_t key, uint8_t count_bit) {
    uint64_t rev = 0;
    for(uint8_t i = 0; i < count_bit; i++) {
        rev = (rev << 1) | (key & 1);
        key >>= 1;
    }
    return rev;
}

static bool detect_keeloq(LevelDuration* samples, size_t count, DecodedRollingCode* out) {
    size_t i = 0;

    /* ---- Phase 1: Find preamble (alternating ~400us pulses) ---- */
    size_t preamble_start __attribute__((unused)) = 0;
    size_t preamble_count = 0;

    while(i < count) {
        /* Try to find a run of alternating short pulses */
        if(level_duration_get_level(samples[i]) && IN_TOL(level_duration_get_duration(samples[i]), KL_TE_SHORT, KL_TE_DELTA)) {
            /* Found a short HIGH — count alternating pairs */
            size_t j = i;
            size_t pairs = 0;
            while(j + 1 < count) {
                bool hi_ok = level_duration_get_level(samples[j]) &&
                             IN_TOL(level_duration_get_duration(samples[j]), KL_TE_SHORT, KL_TE_DELTA);
                bool lo_ok = !level_duration_get_level(samples[j + 1]) &&
                             IN_TOL(level_duration_get_duration(samples[j + 1]), KL_TE_SHORT, KL_TE_DELTA);
                if(hi_ok && lo_ok) {
                    pairs++;
                    j += 2;
                } else {
                    break;
                }
            }
            if(pairs >= KL_MIN_PREAMBLE / 2) {
                preamble_start = i;
                preamble_count = pairs * 2;
                i = j;
                break;
            }
        }
        i++;
    }

    if(preamble_count < KL_MIN_PREAMBLE / 2) {
        FURI_LOG_D(TAG, "KL: no preamble found");
        return false;
    }

    FURI_LOG_D(TAG, "KL: preamble found at idx %zu (%zu pulses)", preamble_start, preamble_count);

    /* ---- Phase 2: Find header gap (~4000us LOW) ---- */
    /* After preamble pulses, look for a long LOW ~ 10*TE */
    bool found_header = false;
    while(i < count) {
        if(!level_duration_get_level(samples[i]) &&
           IN_TOL(level_duration_get_duration(samples[i]), KL_TE_SHORT * 10, KL_TE_DELTA * 10)) {
            found_header = true;
            i++; /* skip header, now pointing at first data pulse */
            break;
        }
        /* Also accept slightly shorter/longer header with wider tolerance */
        if(!level_duration_get_level(samples[i]) && level_duration_get_duration(samples[i]) >= KL_TE_SHORT * 8 &&
           level_duration_get_duration(samples[i]) <= KL_TE_SHORT * 14) {
            found_header = true;
            i++;
            break;
        }
        i++;
    }

    if(!found_header) {
        FURI_LOG_D(TAG, "KL: no header gap after preamble");
        return false;
    }

    /* ---- Phase 3: Decode data bits ---- */
    uint64_t data = 0;
    uint8_t bits_decoded = 0;

    while(i + 1 < count && bits_decoded < KL_DATA_BITS + 2) {
        bool hi_level = level_duration_get_level(samples[i]);
        uint32_t hi_dur = level_duration_get_duration(samples[i]);
        bool lo_level = !level_duration_get_level(samples[i + 1]);
        uint32_t lo_dur = level_duration_get_duration(samples[i + 1]);

        if(!hi_level || !lo_level) {
            /* Not a HIGH-LOW pair */
            break;
        }

        /* Check for end-of-transmission (very long LOW) */
        if(lo_dur >= (uint32_t)(KL_TE_SHORT * 2 + KL_TE_DELTA)) {
            break;
        }

        /* Decode bit:
         * bit 1: short HIGH + long LOW  (1TE + 2TE)
         * bit 0: long HIGH + short LOW  (2TE + 1TE) */
        bool short_hi = IN_TOL(hi_dur, KL_TE_SHORT, KL_TE_DELTA);
        bool long_lo = IN_TOL(lo_dur, KL_TE_LONG, KL_TE_DELTA * 2);
        bool long_hi = IN_TOL(hi_dur, KL_TE_LONG, KL_TE_DELTA * 2);
        bool short_lo = IN_TOL(lo_dur, KL_TE_SHORT, KL_TE_DELTA);

        if(short_hi && long_lo) {
            data = (data << 1) | 1;
            bits_decoded++;
            i += 2;
        } else if(long_hi && short_lo) {
            data = (data << 1) | 0;
            bits_decoded++;
            i += 2;
        } else {
            /* Timing mismatch — end of frame */
            break;
        }
    }

    FURI_LOG_D(TAG, "KL: decoded %d bits", bits_decoded);

    /* Validate: we need at least 64 data bits */
    if(bits_decoded < KL_DATA_BITS) {
        FURI_LOG_D(TAG, "KL: too few bits (%d < %d)", bits_decoded, KL_DATA_BITS);
        return false;
    }

    /* ---- Phase 4: Extract fields ---- */
    /* Reverse bits per Flipper convention to get correct byte order */
    uint64_t key = reverse_key(data, KL_DATA_BITS);
    uint32_t key_fix = (uint32_t)(key >> 32);
    uint32_t key_hop = (uint32_t)(key & 0xFFFFFFFF);

    out->proto = PROTO_KEELOQ;
    out->serial = key_fix & 0x0FFFFFFF;
    out->button = (uint8_t)(key_fix >> 28);
    out->hopping_hi = 0;
    out->hopping_lo = key_hop;
    out->counter = (uint16_t)(key_hop & 0xFFFF);
    strncpy(out->name, "KeeLoq", sizeof(out->name) - 1);

    FURI_LOG_D(
        TAG,
        "KL: serial=0x%08" PRIX32 " btn=0x%02X hop=0x%08" PRIX32 " rawcnt=0x%04X",
        out->serial,
        out->button,
        out->hopping_lo,
        out->counter);

    return true;
}

/* =========================================================================
 * Nice Flor-S detection
 * =========================================================================
 *
 * PWM encoding:
 *   bit 1 = long HIGH (2TE) + short LOW (1TE)
 *   bit 0 = short HIGH (1TE) + long LOW (2TE)
 *
 * Header:  ~18500 us LOW  (37 × 500)
 * Start:   1500 us HIGH + 1500 us LOW
 * Data:    52 bits PWM
 * Stop:    1500 us HIGH
 * ========================================================================= */

static bool detect_nice_flor_s(LevelDuration* samples, size_t count, DecodedRollingCode* out) {
    size_t i = 0;

    /* ---- Phase 1: Find header (~18500 us LOW) ---- */
    bool found_header = false;
    while(i < count) {
        if(!level_duration_get_level(samples[i]) &&
           IN_TOL(level_duration_get_duration(samples[i]), NF_HEADER_US, NF_TE_DELTA * 38)) {
            found_header = true;
            i++;
            break;
        }
        i++;
    }

    if(!found_header) {
        FURI_LOG_D(TAG, "NF: no header found");
        return false;
    }

    /* ---- Phase 2: Find start pattern (1500us HIGH + 1500us LOW) ---- */
    bool found_start = false;
    while(i + 1 < count) {
        if(level_duration_get_level(samples[i]) &&
           IN_TOL(level_duration_get_duration(samples[i]), NF_START_HI, NF_TE_DELTA * 3) &&
           !level_duration_get_level(samples[i + 1]) &&
           IN_TOL(level_duration_get_duration(samples[i + 1]), NF_START_LO, NF_TE_DELTA * 3)) {
            found_start = true;
            i += 2;
            break;
        }
        i++;
    }

    if(!found_start) {
        FURI_LOG_D(TAG, "NF: no start pattern after header");
        return false;
    }

    /* ---- Phase 3: Decode PWM data bits ---- */
    uint64_t data = 0;
    uint8_t bits_decoded = 0;

    while(i < count && bits_decoded < NF_DATA_BITS + 4) {
        /* Check for stop bit (1500us HIGH) */
        if(level_duration_get_level(samples[i]) &&
           IN_TOL(level_duration_get_duration(samples[i]), NF_STOP_HI, NF_TE_DELTA)) {
            /* Found stop bit — frame complete */
            break;
        }

        if(i + 1 >= count) break;

        /* Expect HIGH-LOW pair for PWM bit */
        if(!level_duration_get_level(samples[i])) break; /* unexpected LOW */

        uint32_t hi_dur = level_duration_get_duration(samples[i]);
        uint32_t lo_dur = level_duration_get_duration(samples[i + 1]);

        /* bit 1: long HIGH + short LOW */
        if(IN_TOL(hi_dur, NF_TE_LONG, NF_TE_DELTA) &&
           IN_TOL(lo_dur, NF_TE_SHORT, NF_TE_DELTA)) {
            data = (data << 1) | 1;
            bits_decoded++;
            i += 2;
        }
        /* bit 0: short HIGH + long LOW */
        else if(IN_TOL(hi_dur, NF_TE_SHORT, NF_TE_DELTA) &&
                IN_TOL(lo_dur, NF_TE_LONG, NF_TE_DELTA)) {
            data = (data << 1) | 0;
            bits_decoded++;
            i += 2;
        } else {
            /* Timing mismatch — end of frame */
            break;
        }
    }

    FURI_LOG_D(TAG, "NF: decoded %d bits", bits_decoded);

    if(bits_decoded != NF_DATA_BITS) {
        FURI_LOG_D(TAG, "NF: wrong bit count (%d != %d)", bits_decoded, NF_DATA_BITS);
        return false;
    }

    /* ---- Phase 4: Extract fields ---- */
    /* P0 (4-bit button) is the upper nibble, NOT encrypted */
    out->proto = PROTO_NICE_FLOR_S;
    out->button = (uint8_t)((data >> 48) & 0xF);
    out->serial = 0; /* encrypted — needs rainbow table */
    out->counter = 0; /* encrypted — needs rainbow table */
    out->hopping_hi = (uint32_t)((data >> 32) & 0xFFFFF);
    out->hopping_lo = (uint32_t)(data & 0xFFFFFFFF);
    strncpy(out->name, "Nice Flor-S", sizeof(out->name) - 1);

    FURI_LOG_D(
        TAG,
        "NF: btn=0x%02X raw=0x%05" PRIX32 "%08" PRIX32,
        out->button,
        out->hopping_hi,
        out->hopping_lo);

    return true;
}

/* =========================================================================
 * CAME Atomo detection
 * =========================================================================
 *
 * Manchester encoded, TE = 600/1200 us.
 *
 * Header: long HIGH pulse (~18000 us) + very long LOW gap.
 * The decoder matches LOW gaps of:
 *   ~12000 us (te_long * 10,  TOP44RBN)
 *   ~19200 us (te_long * 16,  TOPD4REN)
 *   ~72000 us (te_long * 60,  TOP42R/44R)
 *
 * Data: 62 bits Manchester, inverted (NOT data per Flipper convention).
 * Then an inter-parcel gap of ~68000 us.
 *
 * Decryption: invert all 62 bits, shift left 4, then atomo_decrypt.
 * After decryption: [8-bit HoldCnt][16-bit Counter][32-bit Serial][4-bit Btn][4-bit Zero]
 * ========================================================================= */

/* Atomo decryption (bit-stream cipher, from came_atomo.c) */
static void atomo_decrypt(uint8_t* buff) {
    buff[0] = (buff[0] ^ 5) & 0x7F;
    uint8_t tmpB = (-buff[0]) & 0x7F;

    uint8_t bitCnt = 8;
    while(bitCnt < 59) {
        if((tmpB & 0x18) && (((tmpB / 8) & 3) != 3)) {
            tmpB = ((tmpB << 1) & 0xFF) | 1;
        } else {
            tmpB = (tmpB << 1) & 0xFF;
        }
        if(tmpB & 0x80) {
            buff[bitCnt / 8] ^= (0x80 >> (bitCnt & 7));
        }
        bitCnt++;
    }
}

static bool detect_came_atomo(LevelDuration* samples, size_t count, DecodedRollingCode* out) {
    size_t i = 0;

    /* ---- Phase 1: Find header gap ----
     * The CAME Atomo header is a long HIGH followed by a very long LOW.
     * The Flipper decoder matches on the LOW portion:
     *   ~12000 us (te_long*10), ~19200 us (te_long*16), or ~72000 us (te_long*60)
     */
    bool found_header = false;
    while(i < count) {
        if(!level_duration_get_level(samples[i])) {
            uint32_t d = level_duration_get_duration(samples[i]);
            if(IN_TOL(d, CA_TE_LONG * 10, CA_TE_DELTA * 20) ||
               IN_TOL(d, CA_TE_LONG * 16, CA_TE_DELTA * 10) ||
               IN_TOL(d, CA_TE_LONG * 60, CA_TE_DELTA * 40)) {
                found_header = true;
                i++;
                break;
            }
        }
        i++;
    }

    if(!found_header) {
        FURI_LOG_D(TAG, "CA: no header gap found");
        return false;
    }

    /* ---- Phase 2: Decode Manchester data (62 bits) ----
     * The decoder starts after the header with the first HIGH-LOW
     * Manchester transition. CAME Atomo uses inverted data:
     *   stored_bit = !manchester_bit
     */
    ManchesterState mstate;
    manchester_reset(&mstate);

    uint64_t raw_data = 0;
    uint8_t bits_decoded = 0;

    /* Seed: first sample after the long LOW should be a short LOW (transition) */
    /* Try feeding each sample through the Manchester decoder */
    while(i < count && bits_decoded < CA_DATA_BITS) {
        bool level = level_duration_get_level(samples[i]);
        uint32_t dur = level_duration_get_duration(samples[i]);

        /* Check for inter-parcel gap (very long LOW → end of this parcel) */
        if(!level && dur >= (uint32_t)(CA_TE_LONG * 2 + CA_TE_DELTA)) {
            /* End of data section */
            break;
        }

        bool bit_val;
        int result = manchester_feed(&mstate, level, dur, CA_TE_SHORT, CA_TE_DELTA, &bit_val);

        if(result == 1) {
            /* CAME Atomo stores inverted data: stored = !bit */
            raw_data = (raw_data << 1) | (!bit_val);
            bits_decoded++;
        } else if(result < 0) {
            /* Timing error — abort */
            break;
        }
        /* result == 0: half-bit accumulated, continue */

        i++;
    }

    FURI_LOG_D(TAG, "CA: decoded %d bits", bits_decoded);

    if(bits_decoded != CA_DATA_BITS) {
        FURI_LOG_D(TAG, "CA: wrong bit count (%d != %d)", bits_decoded, CA_DATA_BITS);
        return false;
    }

    /* ---- Phase 3: Decrypt and extract fields ---- */
    /* Reverse the atomo encryption:
     * 1. Invert all bits
     * 2. Shift left by 4
     * 3. Apply atomo_decrypt
     */
    raw_data ^= 0xFFFFFFFFFFFFFFFF;
    raw_data <<= 4;

    uint8_t pack[8];
    pack[0] = (uint8_t)(raw_data >> 56);
    pack[1] = (uint8_t)((raw_data >> 48) & 0xFF);
    pack[2] = (uint8_t)((raw_data >> 40) & 0xFF);
    pack[3] = (uint8_t)((raw_data >> 32) & 0xFF);
    pack[4] = (uint8_t)((raw_data >> 24) & 0xFF);
    pack[5] = (uint8_t)((raw_data >> 16) & 0xFF);
    pack[6] = (uint8_t)((raw_data >> 8) & 0xFF);
    pack[7] = (uint8_t)(raw_data & 0xFF);

    atomo_decrypt(pack);

    uint16_t decrypted_counter = (uint16_t)pack[1] << 8 | pack[2];
    uint32_t decrypted_serial = (uint32_t)pack[3] << 24 | pack[4] << 16 | pack[5] << 8 | pack[6];
    uint8_t btn_raw = (pack[7] >> 4) & 0xF;

    /* Map raw button code to logical button number */
    uint8_t btn = 0;
    switch(btn_raw) {
    case 0x0:
        btn = 0x1;
        break;
    case 0x2:
        btn = 0x2;
        break;
    case 0x4:
        btn = 0x3;
        break;
    case 0x6:
        btn = 0x4;
        break;
    case 0xC:
        btn = 0x5;
        break;
    case 0xE:
        btn = 0x6;
        break;
    default:
        btn = btn_raw;
        break;
    }

    out->proto = PROTO_CAME_ATOMO;
    out->serial = decrypted_serial;
    out->counter = decrypted_counter;
    out->button = btn;
    out->hopping_hi = (uint32_t)((raw_data >> 32) & 0xFFFFFFFF);
    out->hopping_lo = (uint32_t)(raw_data & 0xFFFFFFFF);
    strncpy(out->name, "CAME Atomo", sizeof(out->name) - 1);

    FURI_LOG_D(
        TAG,
        "CA: serial=0x%08" PRIX32 " cnt=0x%04X btn=0x%02X",
        out->serial,
        out->counter,
        out->button);

    return true;
}

/* =========================================================================
 * Somfy Telis RTS detection
 * =========================================================================
 *
 * Manchester encoded, TE = 640/1280 us.
 *
 * Frame sequence:
 * 1. WAKE-UP:    HIGH ~9415us + LOW ~89565us  (optional, may not be captured)
 * 2. HW SYNC:    2+ repetitions of HIGH ~2560us + LOW ~2560us
 * 3. SW SYNC:    HIGH ~4550us + LOW ~640us
 * 4. DATA:       56 bits Manchester
 * 5. INTER-FRAME: LOW ~30415us
 *
 * Data decryption: frame[i] ^= frame[i-1] for i=1..6
 * Then: btn = (decrypted >> 44) & 0xF
 *       cnt = (decrypted >> 24) & 0xFFFF
 *       serial = decrypted & 0xFFFFFF
 * CRC: cksum=0; for i=0..6: cksum ^= frame[i] ^ (frame[i]>>4); cksum &= 0xF
 * ========================================================================= */

static uint8_t somfy_crc(uint64_t data) {
    uint8_t crc = 0;
    data &= 0xFFF0FFFFFFFFFFULL;
    for(uint8_t i = 0; i < 56; i += 8) {
        crc = crc ^ (uint8_t)(data >> i) ^ (uint8_t)(data >> (i + 4));
    }
    return crc & 0xF;
}

static bool detect_somfy_rts(LevelDuration* samples, size_t count, DecodedRollingCode* out) {
    size_t i = 0;

    /* ---- Phase 1: Find HW sync pulses ----
     * Look for at least 2 consecutive pairs of:
     *   HIGH ~2560us + LOW ~2560us
     */
    size_t hw_sync_start __attribute__((unused)) = 0;
    uint16_t hw_sync_count = 0;

    while(i + 1 < count) {
        if(level_duration_get_level(samples[i]) &&
           IN_TOL(level_duration_get_duration(samples[i]), ST_HW_SYNC_US, ST_TE_DELTA * 4) &&
           !level_duration_get_level(samples[i + 1]) &&
           IN_TOL(level_duration_get_duration(samples[i + 1]), ST_HW_SYNC_US, ST_TE_DELTA * 4)) {
            if(hw_sync_count == 0) hw_sync_start = i;
            hw_sync_count++;
            i += 2;
        } else if(hw_sync_count >= 2) {
            /* Got enough sync pulses, check for SW sync next */
            break;
        } else {
            hw_sync_count = 0;
            i++;
        }
    }

    if(hw_sync_count < 2) {
        FURI_LOG_D(TAG, "ST: not enough HW sync pulses (%d)", hw_sync_count);
        return false;
    }

    FURI_LOG_D(TAG, "ST: found %d HW sync pulses at idx %zu", hw_sync_count, hw_sync_start);

    /* ---- Phase 2: Find SW sync (HIGH ~4550us + LOW ~640us) ---- */
    bool found_sw_sync = false;
    while(i + 1 < count) {
        if(level_duration_get_level(samples[i]) &&
           IN_TOL(level_duration_get_duration(samples[i]), ST_SW_SYNC_HI, ST_TE_DELTA * 4) &&
           !level_duration_get_level(samples[i + 1]) &&
           IN_TOL(level_duration_get_duration(samples[i + 1]), ST_TE_SHORT, ST_TE_DELTA)) {
            found_sw_sync = true;
            i += 2;
            break;
        }
        i++;
    }

    if(!found_sw_sync) {
        FURI_LOG_D(TAG, "ST: no SW sync found");
        return false;
    }

    /* ---- Phase 3: Decode Manchester data (56 bits) ---- */
    ManchesterState mstate;
    manchester_reset(&mstate);

    /* Seed: after SW sync, the first symbol is a LongHigh in the Flipper decoder.
     * Feed it as if we saw a LongHigh to establish the correct Manchester phase. */
    bool dummy_bit;
    manchester_feed(&mstate, true, ST_TE_SHORT * 2, ST_TE_SHORT, ST_TE_DELTA, &dummy_bit);

    uint64_t raw_data = 0;
    uint8_t bits_decoded = 0;

    while(i < count && bits_decoded < ST_DATA_BITS) {
        bool level = level_duration_get_level(samples[i]);
        uint32_t dur = level_duration_get_duration(samples[i]);

        /* Check for inter-frame gap (very long LOW) */
        if(!level && dur >= (uint32_t)(ST_TE_LONG + ST_TE_DELTA)) {
            break;
        }

        bool bit_val;
        int result = manchester_feed(&mstate, level, dur, ST_TE_SHORT, ST_TE_DELTA, &bit_val);

        if(result == 1) {
            raw_data = (raw_data << 1) | bit_val;
            bits_decoded++;
        } else if(result < 0) {
            break;
        }

        i++;
    }

    FURI_LOG_D(TAG, "ST: decoded %d bits", bits_decoded);

    if(bits_decoded != ST_DATA_BITS) {
        FURI_LOG_D(TAG, "ST: wrong bit count (%d != %d)", bits_decoded, ST_DATA_BITS);
        return false;
    }

    /* ---- Phase 4: Decrypt (XOR running) and extract fields ---- */
    uint64_t decrypted = raw_data ^ (raw_data >> 8);

    /* CRC check */
    uint8_t crc_calc = somfy_crc(decrypted);
    uint8_t crc_recv = (uint8_t)((decrypted >> 40) & 0xF);

    if(crc_calc != crc_recv) {
        FURI_LOG_D(TAG, "ST: CRC mismatch (calc=0x%X recv=0x%X)", crc_calc, crc_recv);
        return false;
    }

    uint8_t btn = (uint8_t)((decrypted >> 44) & 0xF);
    uint16_t cnt = (uint16_t)((decrypted >> 24) & 0xFFFF);
    uint32_t serial = (uint32_t)(decrypted & 0xFFFFFF);

    out->proto = PROTO_SOMFY_RTS;
    out->serial = serial;
    out->counter = cnt;
    out->button = btn;
    out->hopping_hi = (uint32_t)(raw_data >> 32);
    out->hopping_lo = (uint32_t)(raw_data & 0xFFFFFFFF);
    strncpy(out->name, "Somfy RTS", sizeof(out->name) - 1);

    FURI_LOG_D(
        TAG,
        "ST: serial=0x%06" PRIX32 " cnt=0x%04X btn=0x%02X CRC OK",
        out->serial,
        out->counter,
        out->button);

    return true;
}

/* =========================================================================
 * Public API
 * ========================================================================= */

bool protocol_detect(LevelDuration* samples, size_t count, DecodedRollingCode* out) {
    furi_assert(samples);
    furi_assert(out);

    if(count == 0) return false;

    memset(out, 0, sizeof(DecodedRollingCode));

    /* Try each protocol in order of AU relevance */
    if(detect_keeloq(samples, count, out)) return true;
    if(detect_nice_flor_s(samples, count, out)) return true;
    if(detect_came_atomo(samples, count, out)) return true;
    if(detect_somfy_rts(samples, count, out)) return true;

    out->proto = PROTO_NONE;
    return false;
}

const char* protocol_name(RollingCodeProto p) {
    switch(p) {
    case PROTO_KEELOQ:
        return "KeeLoq";
    case PROTO_NICE_FLOR_S:
        return "Nice Flor-S";
    case PROTO_CAME_ATOMO:
        return "CAME Atomo";
    case PROTO_SOMFY_RTS:
        return "Somfy RTS";
    default:
        return "Unknown";
    }
}

uint8_t protocol_counter_bits(RollingCodeProto p) {
    (void)p;
    /* All four protocols use 16-bit rolling counters */
    return 16;
}

bool protocol_is_au_relevant(RollingCodeProto p) {
    switch(p) {
    case PROTO_KEELOQ:
    case PROTO_NICE_FLOR_S:
    case PROTO_CAME_ATOMO:
        return true;
    case PROTO_SOMFY_RTS:
        /* Somfy RTS uses 433.42 MHz, NOT 433.92 MHz */
        return false;
    default:
        return false;
    }
}
