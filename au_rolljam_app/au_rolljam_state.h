#pragma once

/* RollJam state machine states -- shared between au_rolljam.h and view headers */

typedef enum {
    ROLLJAM_STATE_IDLE,
    ROLLJAM_STATE_CONFIG,
    ROLLJAM_STATE_JAM_CAPTURE_A,
    ROLLJAM_STATE_INTERCEPTION_A,
    ROLLJAM_STATE_JAM_CAPTURE_B,
    ROLLJAM_STATE_INTERCEPTION_B,
    ROLLJAM_STATE_REPLAY,
    ROLLJAM_STATE_SAVE,
    ROLLJAM_STATE_SUCCESS,
    ROLLJAM_STATE_ERROR,
} RollJamState;
