#include "au_rolljam_attack_view.h"
#include "au_rolljam.h"
#include "au_rolljam_custom_event.h"

#include <input/input.h>
#include <gui/elements.h>

#define TAG "AuRolljamAttackView"

struct AuRolljamAttackView {
    View* view;
    AuRolljamAttackViewCallback callback;
    void* context;
};

typedef struct {
    RollJamState state;
    char status_text[128];
    bool code_a_captured;
    bool code_b_captured;
    char proto_a[32];
    char proto_b[32];
} AuRolljamAttackViewModel;

static const char* rolljam_state_name(RollJamState state) {
    switch(state) {
    case ROLLJAM_STATE_IDLE:
        return "Idle";
    case ROLLJAM_STATE_CONFIG:
        return "Config";
    case ROLLJAM_STATE_JAM_CAPTURE_A:
        return "Jam+Capture A";
    case ROLLJAM_STATE_INTERCEPTION_A:
        return "Intercept A";
    case ROLLJAM_STATE_JAM_CAPTURE_B:
        return "Jam+Capture B";
    case ROLLJAM_STATE_INTERCEPTION_B:
        return "Intercept B";
    case ROLLJAM_STATE_REPLAY:
        return "Replaying";
    case ROLLJAM_STATE_SAVE:
        return "Saving";
    case ROLLJAM_STATE_SUCCESS:
        return "Success";
    case ROLLJAM_STATE_ERROR:
        return "Error";
    default:
        return "Unknown";
    }
}

static void au_rolljam_attack_view_draw(Canvas* canvas, AuRolljamAttackViewModel* model) {
    /* Title bar */
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_box(canvas, 0, 0, 128, 13);
    canvas_invert_color(canvas);
    canvas_draw_str_aligned(canvas, 64, 3, AlignCenter, AlignTop, "AU RollJam");
    canvas_invert_color(canvas);

    /* State name */
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 24, rolljam_state_name(model->state));

    /* Status text */
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 35, model->status_text);

    /* Captured codes info */
    char buf[64];
    if(model->code_a_captured) {
        snprintf(buf, sizeof(buf), "A: %s", model->proto_a);
        canvas_draw_str(canvas, 2, 46, buf);
    } else {
        canvas_draw_str(canvas, 2, 46, "A: ---");
    }

    if(model->code_b_captured) {
        snprintf(buf, sizeof(buf), "B: %s", model->proto_b);
        canvas_draw_str(canvas, 66, 46, buf);
    } else {
        canvas_draw_str(canvas, 66, 46, "B: ---");
    }

    /* Context-sensitive button hints */
    canvas_set_font(canvas, FontSecondary);
    if(model->state == ROLLJAM_STATE_IDLE) {
        elements_button_center(canvas, "Start");
    } else if(model->state == ROLLJAM_STATE_INTERCEPTION_B) {
        elements_button_center(canvas, "Replay");
    } else if(model->state == ROLLJAM_STATE_SUCCESS) {
        elements_button_center(canvas, "Done");
    } else if(model->state == ROLLJAM_STATE_ERROR) {
        elements_button_center(canvas, "Retry");
    } else if(
        model->state == ROLLJAM_STATE_JAM_CAPTURE_A ||
        model->state == ROLLJAM_STATE_JAM_CAPTURE_B ||
        model->state == ROLLJAM_STATE_INTERCEPTION_A) {
        elements_button_center(canvas, "Stop");
    } else if(model->state == ROLLJAM_STATE_REPLAY) {
        /* No button while replaying */
    }
}

static bool au_rolljam_attack_view_input(InputEvent* event, void* context) {
    furi_assert(event);
    furi_assert(context);

    AuRolljamAttackView* instance = (AuRolljamAttackView*)context;
    bool consumed = false;

    if(event->key == InputKeyBack && event->type == InputTypeShort) {
        if(instance->callback) {
            instance->callback(AuRolljamCustomEventBackPressed, instance->context);
        }
        consumed = true;
    } else if(event->key == InputKeyOk && event->type == InputTypeShort) {
        if(instance->callback) {
            RollJamState current_state = ROLLJAM_STATE_IDLE;

            with_view_model(
                instance->view,
                AuRolljamAttackViewModel * m,
                {
                    current_state = m->state;
                },
                false);

            uint32_t custom_event;
            switch(current_state) {
            case ROLLJAM_STATE_IDLE:
                custom_event = AuRolljamCustomEventStartAttack;
                break;
            case ROLLJAM_STATE_INTERCEPTION_B:
                custom_event = AuRolljamCustomEventReplayPressed;
                break;
            case ROLLJAM_STATE_SUCCESS:
            case ROLLJAM_STATE_ERROR:
                custom_event = AuRolljamCustomEventBackPressed;
                break;
            case ROLLJAM_STATE_JAM_CAPTURE_A:
            case ROLLJAM_STATE_JAM_CAPTURE_B:
            case ROLLJAM_STATE_INTERCEPTION_A:
                custom_event = AuRolljamCustomEventBackPressed;
                break;
            default:
                custom_event = AuRolljamCustomEventBackPressed;
                break;
            }
            instance->callback(custom_event, instance->context);
        }
        consumed = true;
    }

    return consumed;
}

static void au_rolljam_attack_view_enter(void* context) {
    furi_assert(context);
}

static void au_rolljam_attack_view_exit(void* context) {
    furi_assert(context);
}

AuRolljamAttackView* au_rolljam_attack_view_alloc(void) {
    AuRolljamAttackView* instance = malloc(sizeof(AuRolljamAttackView));
    instance->view = view_alloc();
    view_allocate_model(instance->view, ViewModelTypeLocking, sizeof(AuRolljamAttackViewModel));
    view_set_context(instance->view, instance);
    view_set_draw_callback(instance->view, (ViewDrawCallback)au_rolljam_attack_view_draw);
    view_set_input_callback(instance->view, au_rolljam_attack_view_input);
    view_set_enter_callback(instance->view, au_rolljam_attack_view_enter);
    view_set_exit_callback(instance->view, au_rolljam_attack_view_exit);

    with_view_model(
        instance->view,
        AuRolljamAttackViewModel * model,
        {
            model->state = ROLLJAM_STATE_IDLE;
            memset(model->status_text, 0, sizeof(model->status_text));
            model->code_a_captured = false;
            model->code_b_captured = false;
            memset(model->proto_a, 0, sizeof(model->proto_a));
            memset(model->proto_b, 0, sizeof(model->proto_b));
        },
        true);

    return instance;
}

void au_rolljam_attack_view_free(AuRolljamAttackView* instance) {
    furi_assert(instance);
    view_free(instance->view);
    free(instance);
}

View* au_rolljam_attack_view_get_view(AuRolljamAttackView* instance) {
    furi_assert(instance);
    return instance->view;
}

void au_rolljam_attack_view_set_callback(
    AuRolljamAttackView* instance,
    AuRolljamAttackViewCallback callback,
    void* context) {
    furi_assert(instance);
    instance->callback = callback;
    instance->context = context;
}

void au_rolljam_attack_view_set_state(
    AuRolljamAttackView* instance,
    RollJamState state,
    const char* status_text) {
    furi_assert(instance);
    with_view_model(
        instance->view,
        AuRolljamAttackViewModel * model,
        {
            model->state = state;
            if(status_text) {
                snprintf(model->status_text, sizeof(model->status_text), "%s", status_text);
            } else {
                model->status_text[0] = '\0';
            }
        },
        true);
}

void au_rolljam_attack_view_set_codes_info(
    AuRolljamAttackView* instance,
    bool code_a_captured,
    bool code_b_captured,
    const char* proto_a,
    const char* proto_b) {
    furi_assert(instance);
    with_view_model(
        instance->view,
        AuRolljamAttackViewModel * model,
        {
            model->code_a_captured = code_a_captured;
            model->code_b_captured = code_b_captured;
            if(proto_a) snprintf(model->proto_a, sizeof(model->proto_a), "%s", proto_a);
            else
                model->proto_a[0] = '\0';
            if(proto_b) snprintf(model->proto_b, sizeof(model->proto_b), "%s", proto_b);
            else
                model->proto_b[0] = '\0';
        },
        true);
}
