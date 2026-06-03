#include "signal_capture.h"
#include <lib/subghz/devices/devices.h>
#include <lib/subghz/devices/cc1101_int/cc1101_int_interconnect.h>
#include <furi.h>
#include <stdlib.h>
#include <string.h>

/* =========================================================================
 * Static state
 * ========================================================================= */

#define CAPTURE_CHUNK_SIZE 1024

static CapturedSignal captured_signal = {
    .samples = NULL,
    .count = 0,
    .capacity = 0,
    .frequency = 0,
    .protocol_name = {0},
};

static SignalRingBuffer ring = {
    .head = 0,
    .tail = 0,
    .len = 0,
};

static const SubGhzDevice* radio_device = NULL;

/* =========================================================================
 * Ring buffer helpers
 * ========================================================================= */

static void ring_push(LevelDuration ld) {
    ring.buf[ring.head] = ld;
    ring.head = (ring.head + 1) % SIGNAL_CAPTURE_RING_SIZE;
    if(ring.len < SIGNAL_CAPTURE_RING_SIZE) {
        ring.len++;
    } else {
        ring.tail = (ring.tail + 1) % SIGNAL_CAPTURE_RING_SIZE;
    }
}

/* =========================================================================
 * Internal callback — bridges from FuriHal async RX to sample accumulator
 * ========================================================================= */

static SignalCaptureCallback user_callback = NULL;
static void* user_context = NULL;

static void capture_rx_callback(bool level, uint32_t duration, void* context) {
    (void)context;

    /* Accumulate into the captured signal */
    if(captured_signal.count >= captured_signal.capacity) {
        size_t new_cap = captured_signal.capacity + CAPTURE_CHUNK_SIZE;
        LevelDuration* new_buf = realloc(captured_signal.samples, new_cap * sizeof(LevelDuration));
        if(new_buf == NULL) {
            if(user_callback) {
                user_callback(level, duration, user_context);
            }
            return;
        }
        captured_signal.samples = new_buf;
        captured_signal.capacity = new_cap;
    }

    LevelDuration ld = level_duration_make(level, duration);
    captured_signal.samples[captured_signal.count] = ld;
    captured_signal.count++;

    /* Push into ring buffer for protocol detection */
    ring_push(ld);

    /* Forward to user callback if registered */
    if(user_callback) {
        user_callback(level, duration, user_context);
    }
}

/* =========================================================================
 * Public API
 * ========================================================================= */

void signal_capture_start(uint32_t freq_hz, SignalCaptureCallback callback, void* context) {
    user_callback = callback;
    user_context = context;

    captured_signal.count = 0;
    captured_signal.frequency = freq_hz;
    memset(captured_signal.protocol_name, 0, sizeof(captured_signal.protocol_name));

    ring.head = 0;
    ring.tail = 0;
    ring.len = 0;

    /* Get the internal subghz radio device */
    subghz_devices_init();
    radio_device = subghz_devices_get_by_name(SUBGHZ_DEVICE_CC1101_INT_NAME);
    if(radio_device) {
        subghz_devices_begin(radio_device);
        subghz_devices_reset(radio_device);
        subghz_devices_load_preset(radio_device, FuriHalSubGhzPresetOok650Async, NULL);
        subghz_devices_set_frequency(radio_device, freq_hz);
        subghz_devices_start_async_rx(radio_device, capture_rx_callback, NULL);
    }
}

void signal_capture_stop(void) {
    if(radio_device) {
        subghz_devices_stop_async_rx(radio_device);
        subghz_devices_idle(radio_device);
        subghz_devices_end(radio_device);
        radio_device = NULL;
    }
    subghz_devices_deinit();

    user_callback = NULL;
    user_context = NULL;
}

CapturedSignal* signal_capture_get_signal(void) {
    return &captured_signal;
}

void signal_capture_reset(void) {
    if(captured_signal.samples) {
        free(captured_signal.samples);
        captured_signal.samples = NULL;
    }
    captured_signal.count = 0;
    captured_signal.capacity = 0;
    captured_signal.frequency = 0;
    memset(captured_signal.protocol_name, 0, sizeof(captured_signal.protocol_name));

    ring.head = 0;
    ring.tail = 0;
    ring.len = 0;
}

void signal_capture_free(CapturedSignal* signal) {
    if(signal && signal->samples) {
        free(signal->samples);
        signal->samples = NULL;
    }
    signal->count = 0;
    signal->capacity = 0;
}
