#include "au_rolljam.h"
#include "au_rolljam_scene.h"
#include "au_rolljam_custom_event.h"
#include "au_rolljam_main_view.h"
#include "au_rolljam_attack_view.h"

#define TAG "AuRolljamApp"

/* ---- View-dispatcher callbacks ---- */

static bool au_rolljam_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    AuRollJamState* instance = context;
    return scene_manager_handle_custom_event(instance->scene_manager, event);
}

static bool au_rolljam_back_event_callback(void* context) {
    furi_assert(context);
    AuRollJamState* instance = context;
    return scene_manager_handle_back_event(instance->scene_manager);
}

static void au_rolljam_tick_event_callback(void* context) {
    furi_assert(context);
    AuRollJamState* instance = context;
    scene_manager_handle_tick_event(instance->scene_manager);
}

/* ---- CapturedCode helpers ---- */

void captured_code_clear(CapturedCode* code) {
    furi_assert(code);
    if(code->samples) {
        free(code->samples);
        code->samples = NULL;
    }
    code->count = 0;
    code->protocol[0] = '\0';
    code->serial = 0;
    code->counter = 0;
}

void captured_code_free(CapturedCode* code) {
    furi_assert(code);
    captured_code_clear(code);
}

/* ---- Allocation ---- */

AuRollJamState* au_rolljam_alloc(void) {
    AuRollJamState* instance = malloc(sizeof(AuRollJamState));
    memset(instance, 0, sizeof(AuRollJamState));

    /* Default configuration */
    instance->config.frequency = AU_ROLLJAM_DEFAULT_FREQ;
    instance->config.jam_offset_hz = AU_ROLLJAM_DEFAULT_JAM_OFFSET;
    instance->config.target_proto = ROLLJAM_PROTO_AUTO;
    instance->config.ext_module_present = false;
    instance->state = ROLLJAM_STATE_IDLE;
    instance->status_text[0] = '\0';

    /* Scene manager */
    instance->scene_manager = scene_manager_alloc(&au_rolljam_scene_handlers, instance);

    /* View dispatcher */
    instance->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_set_event_callback_context(instance->view_dispatcher, instance);
    view_dispatcher_set_custom_event_callback(instance->view_dispatcher, au_rolljam_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(instance->view_dispatcher, au_rolljam_back_event_callback);
    view_dispatcher_set_tick_event_callback(instance->view_dispatcher, au_rolljam_tick_event_callback, 100);

    /* GUI */
    instance->gui = furi_record_open(RECORD_GUI);

    /* Submenu module */
    instance->submenu = submenu_alloc();
    view_dispatcher_add_view(
        instance->view_dispatcher, AuRolljamViewSubmenu, submenu_get_view(instance->submenu));

    /* Variable item list (config) */
    instance->var_list = variable_item_list_alloc();
    view_dispatcher_add_view(
        instance->view_dispatcher, AuRolljamViewVarList, variable_item_list_get_view(instance->var_list));

    /* DialogEx module */
    instance->dialog_ex = dialog_ex_alloc();
    view_dispatcher_add_view(
        instance->view_dispatcher, AuRolljamViewDialogEx, dialog_ex_get_view(instance->dialog_ex));

    /* Widget (about / error) */
    instance->widget = widget_alloc();
    view_dispatcher_add_view(
        instance->view_dispatcher, AuRolljamViewWidget, widget_get_view(instance->widget));

    /* Custom main view */
    instance->view_main = au_rolljam_main_view_alloc();
    view_dispatcher_add_view(
        instance->view_dispatcher, AuRolljamViewMain, au_rolljam_main_view_get_view(instance->view_main));

    /* Custom attack view */
    instance->view_attack = au_rolljam_attack_view_alloc();
    view_dispatcher_add_view(
        instance->view_dispatcher, AuRolljamViewAttack, au_rolljam_attack_view_get_view(instance->view_attack));

    /* Attempt to load saved config */
    rolljam_load_state(&instance->config);

    return instance;
}

/* ---- Cleanup ---- */

void au_rolljam_free(AuRollJamState* instance) {
    furi_assert(instance);

    /* Stop timer if running */
    if(instance->timer) {
        furi_timer_stop(instance->timer);
        furi_timer_free(instance->timer);
        instance->timer = NULL;
    }

    /* Free captured codes */
    captured_code_free(&instance->code_a);
    captured_code_free(&instance->code_b);

    /* Remove custom views */
    view_dispatcher_remove_view(instance->view_dispatcher, AuRolljamViewAttack);
    au_rolljam_attack_view_free(instance->view_attack);

    view_dispatcher_remove_view(instance->view_dispatcher, AuRolljamViewMain);
    au_rolljam_main_view_free(instance->view_main);

    /* Remove standard modules */
    view_dispatcher_remove_view(instance->view_dispatcher, AuRolljamViewWidget);
    widget_free(instance->widget);

    view_dispatcher_remove_view(instance->view_dispatcher, AuRolljamViewDialogEx);
    dialog_ex_free(instance->dialog_ex);

    view_dispatcher_remove_view(instance->view_dispatcher, AuRolljamViewVarList);
    variable_item_list_free(instance->var_list);

    view_dispatcher_remove_view(instance->view_dispatcher, AuRolljamViewSubmenu);
    submenu_free(instance->submenu);

    /* Scene manager */
    scene_manager_free(instance->scene_manager);

    /* View dispatcher */
    view_dispatcher_free(instance->view_dispatcher);

    /* GUI */
    furi_record_close(RECORD_GUI);
    instance->gui = NULL;

    free(instance);
}

/* ================================================================
 *  ENTRY POINT
 * ================================================================ */

int32_t au_rolljam_app(void* p) {
    UNUSED(p);

    furi_hal_power_suppress_charge_enter();

    AuRollJamState* instance = au_rolljam_alloc();

    view_dispatcher_attach_to_gui(
        instance->view_dispatcher, instance->gui, ViewDispatcherTypeFullscreen);

    scene_manager_next_scene(instance->scene_manager, AuRolljamSceneStart);

    view_dispatcher_run(instance->view_dispatcher);

    au_rolljam_free(instance);

    furi_hal_power_suppress_charge_exit();

    return 0;
}
