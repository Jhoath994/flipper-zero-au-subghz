#include "ext_cc1101.h"
#include <furi_hal_gpio.h>
#include <furi_hal_cortex.h>
#include <furi_hal_resources.h>
#include <lib/toolbox/level_duration.h>
#include <furi.h>

/* =========================================================================
 * Software SPI bit-bang primitives
 * ========================================================================= */

static inline void sw_spi_cs_lo(void) {
    furi_hal_gpio_write(EXT_CC1101_CS_PIN, false);
}

static inline void sw_spi_cs_hi(void) {
    furi_hal_gpio_write(EXT_CC1101_CS_PIN, true);
}

static inline void sw_spi_sck_lo(void) {
    furi_hal_gpio_write(EXT_CC1101_SCK_PIN, false);
}

static inline void sw_spi_sck_hi(void) {
    furi_hal_gpio_write(EXT_CC1101_SCK_PIN, true);
}

static inline void sw_spi_mosi_write(bool bit) {
    furi_hal_gpio_write(EXT_CC1101_MOSI_PIN, bit);
}

static inline bool sw_spi_miso_read(void) {
    return furi_hal_gpio_read(EXT_CC1101_MISO_PIN);
}

/* Write a single byte MSB-first, return MISO bit shifted in each clock */
static uint8_t sw_spi_write_byte(uint8_t byte) {
    uint8_t miso_val = 0;
    for(int8_t i = 7; i >= 0; i--) {
        sw_spi_sck_lo();
        sw_spi_mosi_write((byte >> i) & 1);
        /* brief setup time */
        sw_spi_sck_hi();
        miso_val <<= 1;
        if(sw_spi_miso_read()) {
            miso_val |= 1;
        }
    }
    sw_spi_sck_lo();
    return miso_val;
}

/* Read a single byte MSB-first (write dummy 0x00) */
static uint8_t sw_spi_read_byte(void) {
    return sw_spi_write_byte(0x00);
}

/* =========================================================================
 * CC1101 SPI protocol helpers
 * ========================================================================= */

static void cc1101_spi_write_reg(uint8_t addr, uint8_t val) {
    sw_spi_cs_lo();
    /* Wait for MISO to go low (chip ready) — short timeout */
    uint32_t timeout = 1000;
    while(sw_spi_miso_read() && --timeout)
        ;
    sw_spi_write_byte(addr & 0x7F); /* write: bit6=0, bit7=0 */
    sw_spi_write_byte(val);
    sw_spi_cs_hi();
}

static uint8_t cc1101_spi_read_reg(uint8_t addr) {
    uint8_t val;
    sw_spi_cs_lo();
    uint32_t timeout = 1000;
    while(sw_spi_miso_read() && --timeout)
        ;
    sw_spi_write_byte(addr | 0x80); /* read: bit7=1 */
    val = sw_spi_read_byte();
    sw_spi_cs_hi();
    return val;
}

static void cc1101_spi_strobe(uint8_t cmd) {
    sw_spi_cs_lo();
    uint32_t timeout = 1000;
    while(sw_spi_miso_read() && --timeout)
        ;
    sw_spi_write_byte(cmd);
    sw_spi_cs_hi();
}

static void cc1101_spi_write_burst(uint8_t addr, const uint8_t* data, uint16_t len) {
    sw_spi_cs_lo();
    uint32_t timeout = 1000;
    while(sw_spi_miso_read() && --timeout)
        ;
    sw_spi_write_byte(addr | CC1101_WRITE_BURST);
    for(uint16_t i = 0; i < len; i++) {
        sw_spi_write_byte(data[i]);
    }
    sw_spi_cs_hi();
}

/* =========================================================================
 * Public API
 * ========================================================================= */

void ext_cc1101_init(void) {
    /* Configure GPIO pins */
    /* CS: output, default HIGH */
    furi_hal_gpio_init_simple(EXT_CC1101_CS_PIN, GpioModeOutputPushPull);
    furi_hal_gpio_write(EXT_CC1101_CS_PIN, true);

    /* SCK: output, default LOW */
    furi_hal_gpio_init_simple(EXT_CC1101_SCK_PIN, GpioModeOutputPushPull);
    furi_hal_gpio_write(EXT_CC1101_SCK_PIN, false);

    /* MOSI: output, default LOW */
    furi_hal_gpio_init_simple(EXT_CC1101_MOSI_PIN, GpioModeOutputPushPull);
    furi_hal_gpio_write(EXT_CC1101_MOSI_PIN, false);

    /* MISO: input with pull-up */
    furi_hal_gpio_init(
        EXT_CC1101_MISO_PIN,
        GpioModeInput,
        GpioPullUp,
        GpioSpeedVeryHigh);

    /* GDO0: output, default LOW */
    furi_hal_gpio_init_simple(EXT_CC1101_GDO0_PIN, GpioModeOutputPushPull);
    furi_hal_gpio_write(EXT_CC1101_GDO0_PIN, false);

    /* Power-on reset sequence: assert CS, toggle SCK, then send SRES */
    sw_spi_cs_hi();
    sw_spi_sck_hi();
    furi_delay_us(10);
    sw_spi_sck_lo();
    furi_delay_us(10);
    sw_spi_cs_lo();
    /* Wait for MISO to go low indicating chip ready */
    uint32_t timeout = 1000;
    while(sw_spi_miso_read() && --timeout)
        ;
    sw_spi_cs_hi();
    furi_delay_us(10);

    /* Send SRES (reset) command strobe */
    cc1101_spi_strobe(CC1101_SRES);

    /* Wait for reset to complete */
    furi_delay_us(100);

    /* Write base OOK configuration registers */
    cc1101_spi_write_reg(CC1101_PKTCTRL0, 0x32); /* async serial, continuous, no whitening */
    cc1101_spi_write_reg(CC1101_IOCFG0, 0x0D);   /* GDO0 = async serial data out */
    cc1101_spi_write_reg(CC1101_MDMCFG2, 0x30);   /* OOK modulation, no preamble/sync */
    cc1101_spi_write_reg(CC1101_MCSM0, 0x18);     /* auto-calibrate, PO_TIMEOUT */
    cc1101_spi_write_reg(CC1101_AGCTRL2, 0x07);   /* AGC settings */
    cc1101_spi_write_reg(CC1101_AGCTRL1, 0x00);
    cc1101_spi_write_reg(CC1101_AGCTRL0, 0x91);
    cc1101_spi_write_reg(CC1101_FREND0, 0x11);    /* PA power setting index 1 */
    cc1101_spi_write_reg(CC1101_FSCAL3, 0xE9);    /* frequency calibration */
    cc1101_spi_write_reg(CC1101_FSCAL2, 0x2A);
    cc1101_spi_write_reg(CC1101_FSCAL1, 0x00);
    cc1101_spi_write_reg(CC1101_FSCAL0, 0x1F);
}

void ext_cc1101_deinit(void) {
    ext_cc1101_reset();

    /* Set all pins to analog / high-impedance input */
    furi_hal_gpio_init_simple(EXT_CC1101_CS_PIN, GpioModeAnalog);
    furi_hal_gpio_init_simple(EXT_CC1101_SCK_PIN, GpioModeAnalog);
    furi_hal_gpio_init_simple(EXT_CC1101_MOSI_PIN, GpioModeAnalog);
    furi_hal_gpio_init_simple(EXT_CC1101_MISO_PIN, GpioModeAnalog);
    furi_hal_gpio_init_simple(EXT_CC1101_GDO0_PIN, GpioModeAnalog);
}

void ext_cc1101_reset(void) {
    cc1101_spi_strobe(CC1101_SRES);
    furi_delay_us(100);
}

bool ext_cc1101_check_present(void) {
    /* Write 0x55 to the TX FIFO and read it back */
    cc1101_spi_write_reg(CC1101_FIFO | CC1101_WRITE_SINGLE, 0x55);
    uint8_t val = cc1101_spi_read_reg(CC1101_FIFO | CC1101_READ_SINGLE);
    /* Flush FIFOs to leave clean state */
    cc1101_spi_strobe(CC1101_SFTX);
    cc1101_spi_strobe(CC1101_SFRX);
    return (val == 0x55);
}

void ext_cc1101_write_reg(uint8_t addr, uint8_t val) {
    cc1101_spi_write_reg(addr, val);
}

uint8_t ext_cc1101_read_reg(uint8_t addr) {
    return cc1101_spi_read_reg(addr);
}

void ext_cc1101_write_burst(uint8_t addr, const uint8_t* data, uint16_t len) {
    cc1101_spi_write_burst(addr, data, len);
}

void ext_cc1101_set_frequency(uint32_t hz) {
    /*
     * Freq register value = (target_hz / (26 MHz * 2)) * 2^16
     *                      = (target_hz * 65536) / 52000000
     * Or equivalently:     = (target_hz / 26000000) * 65536 * 16 / 16
     * Simplified: freq_reg = (uint64_t)hz * 65536 / 52000000
     */
    uint64_t freq_reg = ((uint64_t)hz * 65536ULL) / 52000000ULL;
    uint8_t f2 = (freq_reg >> 16) & 0xFF;
    uint8_t f1 = (freq_reg >> 8) & 0xFF;
    uint8_t f0 = freq_reg & 0xFF;

    cc1101_spi_write_reg(CC1101_FREQ2, f2);
    cc1101_spi_write_reg(CC1101_FREQ1, f1);
    cc1101_spi_write_reg(CC1101_FREQ0, f0);
}

void ext_cc1101_start_cw_jam(uint32_t freq_hz, int32_t offset_hz) {
    /* Set frequency to target + offset */
    uint32_t jam_freq = (uint32_t)((int32_t)freq_hz + offset_hz);
    ext_cc1101_set_frequency(jam_freq);

    /* Flush TX FIFO */
    cc1101_spi_strobe(CC1101_SFTX);

    /* Ensure OOK async mode for CW */
    cc1101_spi_write_reg(CC1101_PKTCTRL0, 0x32);
    cc1101_spi_write_reg(CC1101_IOCFG0, 0x0D);

    /* Enter TX mode — carrier will be present when GDO0 is driven high */
    cc1101_spi_strobe(CC1101_STX);

    /* Small delay for state transition */
    furi_delay_us(50);

    /* Drive GDO0 HIGH for continuous-wave (always-on carrier in OOK) */
    furi_hal_gpio_write(EXT_CC1101_GDO0_PIN, true);
}

void ext_cc1101_stop_cw_jam(void) {
    /* Drive GDO0 LOW to turn off OOK carrier */
    furi_hal_gpio_write(EXT_CC1101_GDO0_PIN, false);

    /* Enter idle state */
    cc1101_spi_strobe(CC1101_SIDLE);
}

void ext_cc1101_start_async_tx(uint32_t freq_hz) {
    ext_cc1101_set_frequency(freq_hz);

    /* Flush TX FIFO */
    cc1101_spi_strobe(CC1101_SFTX);

    /* Async serial TX configuration */
    cc1101_spi_write_reg(CC1101_PKTCTRL0, 0x32);
    cc1101_spi_write_reg(CC1101_IOCFG0, 0x0D);

    /* Enter TX mode; GDO0 will be GPIO-driven for async bit pattern */
    cc1101_spi_strobe(CC1101_STX);

    /* Small delay for state transition */
    furi_delay_us(50);

    /* Start with GDO0 low */
    furi_hal_gpio_write(EXT_CC1101_GDO0_PIN, false);
}

void ext_cc1101_stop_async_tx(void) {
    /* Drive GDO0 low before leaving TX */
    furi_hal_gpio_write(EXT_CC1101_GDO0_PIN, false);

    /* Enter idle state */
    cc1101_spi_strobe(CC1101_SIDLE);
}

void ext_cc1101_atomic_replay_signal(const LevelDuration samples[], size_t count) {
    if(count == 0) return;

    /* Enable DWT cycle counter */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    /* Disable all interrupts for cycle-accurate timing */
    __disable_irq();

    /* Ensure GDO0 starts LOW */
    furi_hal_gpio_write(EXT_CC1101_GDO0_PIN, false);

    for(size_t i = 0; i < count; i++) {
        bool level = level_duration_get_level(samples[i]);
        uint32_t duration_us = level_duration_get_duration(samples[i]);
        uint32_t cycles = duration_us * DWT_CYCLES_PER_US;

        /* Set GDO0 to the sample level */
        furi_hal_gpio_write(EXT_CC1101_GDO0_PIN, level);

        /* Busy-wait using DWT cycle counter */
        uint32_t start = DWT->CYCCNT;
        while((DWT->CYCCNT - start) < cycles)
            ;
    }

    /* Ensure GDO0 ends LOW */
    furi_hal_gpio_write(EXT_CC1101_GDO0_PIN, false);

    /* Re-enable interrupts */
    __enable_irq();
}
