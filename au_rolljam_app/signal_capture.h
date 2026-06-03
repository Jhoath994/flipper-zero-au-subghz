#pragma once

#include <furi.h>
#include <lib/toolbox/level_duration.h>

/*
 * Signal capture via the Flipper Zero INTERNAL CC1101 (managed by OS).
 * Received OOK edges are accumulated as LevelDuration samples for
 * later protocol detection and atomic replay on the external module.
 */

/* ---- Types ---- */

typedef void (*SignalCaptureCallback)(bool level, uint32_t duration, void* context);

typedef struct {
    LevelDuration* samples;
    size_t count;
    size_t capacity;
    uint32_t frequency;
    char protocol_name[32];
} CapturedSignal;

/* ---- Ring buffer for protocol detection (last N samples) ---- */
#define SIGNAL_CAPTURE_RING_SIZE 64

typedef struct {
    LevelDuration buf[SIGNAL_CAPTURE_RING_SIZE];
    size_t head;
    size_t tail;
    size_t len;
} SignalRingBuffer;

/* ---- Public API ---- */

void signal_capture_start(uint32_t freq_hz, SignalCaptureCallback callback, void* context);
void signal_capture_stop(void);
CapturedSignal* signal_capture_get_signal(void);
void signal_capture_reset(void);
void signal_capture_free(CapturedSignal* signal);
