#pragma once

#include <furi.h>
#include <furi_hal.h>

#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/submenu.h>
#include <gui/modules/dialog_ex.h>
#include <gui/modules/variable_item_list.h>
#include <gui/modules/widget.h>

#include <storage/storage.h>
#include <flipper_format/flipper_format.h>

#include <lib/subghz/types.h>
#include "au_rolljam_state.h"
#include "au_rolljam_custom_event.h"
#include "au_rolljam_scene.h"

#define AU_ROLLJAM_APP_NAME "AU RollJam"
#define AU_ROLLJAM_SAVE_DIR "/ext/subghz/au_rolljam/"
#define AU_ROLLJAM_STATE_FILE "/ext/subghz/au_rolljam/rolljam_state.cfg"
#define AU_ROLLJAM_DEFAULT_FREQ 433920000
#define AU_ROLLJAM_DEFAULT_JAM_OFFSET 900000
#define AU_ROLLJAM_JAM_OFFSET_MIN 700000
#define AU_ROLLJAM_JAM_OFFSET_MAX 1100000
#define AU_ROLLJAM_JAM_OFFSET_STEP 50000

/* ---- Target protocol selector ---- */
typedef enum {
    ROLLJAM_PROTO_AUTO = 0,
    ROLLJAM_PROTO_KEELOQ,
    ROLLJAM_PROTO_NICE,
    ROLLJAM_PROTO_CAME,
    ROLLJAM_PROTO_COUNT,
} RollJamProtocol;

/* ---- Configuration ---- */
typedef struct {
    uint32_t frequency;
    int32_t jam_offset_hz;
    uint8_t target_proto;
    bool ext_module_present;
} RollJamConfig;

/* ---- Captured rolling-code sample ---- */
typedef struct {
    LevelDuration* samples;
    size_t count;
    size_t _capacity; /* internal: buffer capacity for realloc */
    char protocol[32];
    uint32_t serial;
    uint16_t counter;
} CapturedCode;

/* ---- Forward declarations for custom views ---- */
typedef struct AuRolljamMainView AuRolljamMainView;
typedef struct AuRolljamAttackView AuRolljamAttackView;

/* ---- Main app state ---- */
typedef struct AuRollJamState AuRollJamState;

struct AuRollJamState {
    ViewDispatcher* view_dispatcher;
    SceneManager* scene_manager;
    Gui* gui;

    AuRolljamMainView* view_main;
    AuRolljamAttackView* view_attack;

    /* Standard GUI modules */
    Submenu* submenu;
    VariableItemList* var_list;
    DialogEx* dialog_ex;
    Widget* widget;

    /* App data */
    RollJamConfig config;
    CapturedCode code_a;
    CapturedCode code_b;
    RollJamState state;
    FuriTimer* timer;
    char status_text[128];
};

/* ---- View enum ---- */
typedef enum {
    AuRolljamViewNone = 0,
    AuRolljamViewMain,
    AuRolljamViewAttack,
    AuRolljamViewSubmenu,
    AuRolljamViewVarList,
    AuRolljamViewDialogEx,
    AuRolljamViewWidget,
} AuRolljamView;

/* ---- Allocation / cleanup ---- */
AuRollJamState* au_rolljam_alloc(void);
void au_rolljam_free(AuRollJamState* instance);

/* ---- CapturedCode helpers ---- */
void captured_code_clear(CapturedCode* code);
void captured_code_free(CapturedCode* code);

/* ---- Persistence (au_rolljam_persistence.c) ---- */
bool rolljam_save_code(CapturedCode* code, const char* filename);
void rolljam_load_codes(void);
bool rolljam_save_state(RollJamState state, RollJamConfig* config);
bool rolljam_load_state(RollJamConfig* config);

/* ---- Custom views (see au_rolljam_main_view.h / au_rolljam_attack_view.h) ---- */
