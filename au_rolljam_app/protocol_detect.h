#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <lib/toolbox/level_duration.h>

/* ---- Rolling-code protocol enumeration ---- */
typedef enum {
    PROTO_NONE = 0,
    PROTO_KEELOQ,
    PROTO_NICE_FLOR_S,
    PROTO_CAME_ATOMO,
    PROTO_SOMFY_RTS,
    PROTO_COUNT
} RollingCodeProto;

/* ---- Decoded rolling-code result ---- */
typedef struct {
    RollingCodeProto proto;
    uint32_t serial;
    uint16_t counter;
    uint8_t button;
    uint32_t hopping_hi;
    uint32_t hopping_lo;
    char name[24];
} DecodedRollingCode;

/* ---- Public API ---- */

bool protocol_detect(LevelDuration* samples, size_t count, DecodedRollingCode* out);
const char* protocol_name(RollingCodeProto p);
uint8_t protocol_counter_bits(RollingCodeProto p);
bool protocol_is_au_relevant(RollingCodeProto p);
