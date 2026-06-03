#pragma once

/**
 * Custom events for the AU RollJam application.
 * First 100 values reserved for view indices.
 */
typedef enum {
    AuRolljamCustomEventReserved = 100,

    /* Navigation / UI */
    AuRolljamCustomEventBackPressed,
    AuRolljamCustomEventStartAttack,
    AuRolljamCustomEventConfig,
    AuRolljamCustomEventAbout,

    /* Attack state machine */
    AuRolljamCustomEventSignalCapturedA,
    AuRolljamCustomEventSignalCapturedB,
    AuRolljamCustomEventReplayPressed,
    AuRolljamCustomEventReplayDone,
    AuRolljamCustomEventSaveDone,
    AuRolljamCustomEventError,

    /* Config changes */
    AuRolljamCustomEventConfigFrequency,
    AuRolljamCustomEventConfigJamOffset,
    AuRolljamCustomEventConfigProtocol,
} AuRolljamCustomEvent;
