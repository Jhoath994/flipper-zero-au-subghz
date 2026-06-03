#include "au_rolljam.h"

#include <storage/storage.h>
#include <flipper_format/flipper_format.h>

#define TAG "AuRolljamPersistence"

/* ================================================================
 *  rolljam_save_code — save a CapturedCode as a Flipper .sub RAW file
 * ================================================================ */

bool rolljam_save_code(CapturedCode* code, const char* filename) {
    furi_assert(code);
    furi_assert(filename);

    if(code->count == 0 || code->samples == NULL) {
        FURI_LOG_E(TAG, "No samples to save");
        return false;
    }

    Storage* storage = furi_record_open(RECORD_STORAGE);

    /* Ensure save directory exists */
    if(!storage_common_exists(storage, AU_ROLLJAM_SAVE_DIR)) {
        storage_common_mkdir(storage, AU_ROLLJAM_SAVE_DIR);
    }

    /* Build full path */
    char fullpath[256];
    snprintf(fullpath, sizeof(fullpath), "%s%s", AU_ROLLJAM_SAVE_DIR, filename);

    FlipperFormat* ff = flipper_format_file_alloc(storage);

    bool success = false;
    do {
        if(!flipper_format_file_open_always(ff, fullpath)) {
            FURI_LOG_E(TAG, "Failed to open %s for writing", fullpath);
            break;
        }

        /* Header */
        if(!flipper_format_write_header_cstr(ff, "Flipper SubGhz RAW File", 1)) break;
        if(!flipper_format_write_uint32(ff, "Frequency", &code->serial, 1)) {
            /* We store the frequency as a comment; since we don't have a
             * separate frequency field on CapturedCode, write a placeholder
             * using the default. In a real build the frequency would be a
             * field on CapturedCode. */
            uint32_t freq = AU_ROLLJAM_DEFAULT_FREQ;
            flipper_format_rewind(ff);
            if(!flipper_format_write_uint32(ff, "Frequency", &freq, 1)) break;
        }

        /* We need to rewrite properly: start over */
        flipper_format_file_close(ff);
        if(!flipper_format_file_open_always(ff, fullpath)) break;

        uint32_t version = 1;
        if(!flipper_format_write_header_cstr(ff, "Flipper SubGhz RAW File", version)) break;

        uint32_t freq = AU_ROLLJAM_DEFAULT_FREQ;
        if(!flipper_format_write_uint32(ff, "Frequency", &freq, 1)) break;

        const char* preset = "FuriHalSubGhzPresetOok650Async";
        if(!flipper_format_write_string_cstr(ff, "Preset", preset)) break;

        if(!flipper_format_write_string_cstr(ff, "Protocol", "RAW")) break;

        /* RAW_Data: level/duration pairs, up to 8 per line
         * Flipper convention: each pair is "LEVEL:DURATION" where
         * LEVEL is 0 or 1 and DURATION is in microseconds. */
        FuriString* raw_line = furi_string_alloc();
        size_t pair_count = 0;

        for(size_t i = 0; i < code->count; i++) {
            LevelDuration ld = code->samples[i];
            bool level = level_duration_get_level(ld);
            uint32_t duration = level_duration_get_duration(ld);

            if(pair_count > 0) {
                furi_string_cat(raw_line, " ");
            }
            char pair[32];
            snprintf(pair, sizeof(pair), "%u:%lu", level ? 1 : 0, (unsigned long)duration);
            furi_string_cat(raw_line, pair);
            pair_count++;

            /* Write 8 pairs per line (Flipper convention) */
            if(pair_count >= 8 || i == code->count - 1) {
                if(!flipper_format_write_string(ff, "RAW_Data", raw_line)) {
                    FURI_LOG_E(TAG, "Failed to write RAW_Data line %zu", i / 8);
                    break;
                }
                furi_string_reset(raw_line);
                pair_count = 0;
            }
        }

        furi_string_free(raw_line);
        success = true;

    } while(0);

    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);

    if(success) {
        FURI_LOG_I(TAG, "Saved code to %s", fullpath);
    }

    return success;
}

/* ================================================================
 *  rolljam_load_codes — scan save dir for .sub files
 * ================================================================ */

void rolljam_load_codes(void) {
    Storage* storage = furi_record_open(RECORD_STORAGE);

    if(!storage_common_exists(storage, AU_ROLLJAM_SAVE_DIR)) {
        FURI_LOG_I(TAG, "Save directory does not exist yet");
        furi_record_close(RECORD_STORAGE);
        return;
    }

    File* dir = storage_file_alloc(storage);
    if(!storage_dir_open(dir, AU_ROLLJAM_SAVE_DIR)) {
        FURI_LOG_W(TAG, "Failed to open save directory");
        storage_file_free(dir);
        furi_record_close(RECORD_STORAGE);
        return;
    }

    FileInfo file_info;
    char name[256];
    uint16_t count = 0;

    while(storage_dir_read(dir, &file_info, name, sizeof(name))) {
        if(file_info.flags & FSF_DIRECTORY) continue;
        /* Check .sub extension */
        size_t len = strlen(name);
        if(len >= 4 && strcasecmp(name + len - 4, ".sub") == 0) {
            FURI_LOG_I(TAG, "Found saved code: %s", name);
            count++;
        }
    }

    FURI_LOG_I(TAG, "Found %u saved rolljam codes", count);

    storage_dir_close(dir);
    storage_file_free(dir);
    furi_record_close(RECORD_STORAGE);
}

/* ================================================================
 *  rolljam_save_state — save app state for resume
 * ================================================================ */

bool rolljam_save_state(RollJamState state, RollJamConfig* config) {
    furi_assert(config);

    Storage* storage = furi_record_open(RECORD_STORAGE);

    /* Ensure directory exists */
    if(!storage_common_exists(storage, AU_ROLLJAM_SAVE_DIR)) {
        storage_common_mkdir(storage, AU_ROLLJAM_SAVE_DIR);
    }

    FlipperFormat* ff = flipper_format_file_alloc(storage);
    bool success = false;

    do {
        if(!flipper_format_file_open_always(ff, AU_ROLLJAM_STATE_FILE)) break;

        uint32_t version = 1;
        if(!flipper_format_write_header_cstr(ff, "AU RollJam State", version)) break;

        uint32_t freq = config->frequency;
        if(!flipper_format_write_uint32(ff, "Frequency", &freq, 1)) break;

        int32_t offset = config->jam_offset_hz;
        if(!flipper_format_write_int32(ff, "JamOffset", &offset, 1)) break;

        uint32_t proto = config->target_proto;
        if(!flipper_format_write_uint32(ff, "Protocol", &proto, 1)) break;

        uint32_t ext_present = config->ext_module_present ? 1 : 0;
        if(!flipper_format_write_uint32(ff, "ExtModule", &ext_present, 1)) break;

        uint32_t st = (uint32_t)state;
        if(!flipper_format_write_uint32(ff, "State", &st, 1)) break;

        success = true;
    } while(0);

    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);

    return success;
}

/* ================================================================
 *  rolljam_load_state — load saved config
 * ================================================================ */

bool rolljam_load_state(RollJamConfig* config) {
    furi_assert(config);

    Storage* storage = furi_record_open(RECORD_STORAGE);
    FlipperFormat* ff = flipper_format_file_alloc(storage);
    bool success = false;

    do {
        if(!flipper_format_file_open_existing(ff, AU_ROLLJAM_STATE_FILE)) break;

        uint32_t version;
        if(!flipper_format_read_header(ff, NULL, &version)) break;

        uint32_t freq;
        if(flipper_format_read_uint32(ff, "Frequency", &freq, 1)) {
            config->frequency = freq;
        }

        int32_t offset;
        if(flipper_format_read_int32(ff, "JamOffset", &offset, 1)) {
            config->jam_offset_hz = offset;
        }

        uint32_t proto;
        if(flipper_format_read_uint32(ff, "Protocol", &proto, 1)) {
            config->target_proto = (uint8_t)proto;
        }

        uint32_t ext_present;
        if(flipper_format_read_uint32(ff, "ExtModule", &ext_present, 1)) {
            config->ext_module_present = (ext_present != 0);
        }

        success = true;
    } while(0);

    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);

    if(success) {
        FURI_LOG_I(TAG, "Loaded config: freq=%lu offset=%ld proto=%u",
                   (unsigned long)config->frequency,
                   (long)config->jam_offset_hz,
                   config->target_proto);
    }

    return success;
}
