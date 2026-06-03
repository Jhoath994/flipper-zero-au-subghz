#include "au_rolljam_main_view.h"
#include "au_rolljam_custom_event.h"

#include <input/input.h>
#include <gui/elements.h>

#define TAG "AuRolljamMainView"

typedef enum {
    MainMenuItemStartAttack = 0,
    MainMenuItemConfig,
    MainMenuItemAbout,
    MainMenuItemCount,
} MainMenuItem;

struct AuRolljamMainView {
    View* view;
    AuRolljamMainViewCallback callback;
    void* context;
    uint8_t index;
};

typedef struct {
    uint8_t index;
} AuRolljamMainViewModel;

static const char* const main_menu_items[] = {
    "Start Attack",
    "Config",
    "About",
};

static void au_rolljam_main_view_draw(Canvas* canvas, AuRolljamMainViewModel* model) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignTop, "AU RollJam");

    canvas_set_font(canvas, FontSecondary);
    for(uint8_t i = 0; i < MainMenuItemCount; i++) {
        uint8_t y = 22 + i * 16;
        if(model->index == i) {
            canvas_draw_box(canvas, 0, y - 1, 128, 15);
            canvas_invert_color(canvas);
            canvas_draw_str_aligned(canvas, 64, y + 6, AlignCenter, AlignCenter, main_menu_items[i]);
            canvas_invert_color(canvas);
        } else {
            canvas_draw_str_aligned(canvas, 64, y + 6, AlignCenter, AlignCenter, main_menu_items[i]);
        }
    }
}

static bool au_rolljam_main_view_input(InputEvent* event, void* context) {
    furi_assert(event);
    furi_assert(context);

    AuRolljamMainView* instance = (AuRolljamMainView*)context;

    if(event->key == InputKeyBack && event->type == InputTypeShort) {
        if(instance->callback) {
            instance->callback(AuRolljamCustomEventBackPressed, instance->context);
        }
        return true;
    }

    bool updated = false;
    bool is_short = (event->type == InputTypeShort) || (event->type == InputTypeRepeat);

    if(is_short) {
        if(event->key == InputKeyUp) {
            if(instance->index == 0) {
                instance->index = MainMenuItemCount - 1;
            } else {
                instance->index--;
            }
            updated = true;
        } else if(event->key == InputKeyDown) {
            if(instance->index >= MainMenuItemCount - 1) {
                instance->index = 0;
            } else {
                instance->index++;
            }
            updated = true;
        } else if(event->key == InputKeyOk) {
            if(instance->callback) {
                uint32_t event_id;
                switch(instance->index) {
                case MainMenuItemStartAttack:
                    event_id = AuRolljamCustomEventStartAttack;
                    break;
                case MainMenuItemConfig:
                    event_id = AuRolljamCustomEventConfig;
                    break;
                case MainMenuItemAbout:
                    event_id = AuRolljamCustomEventAbout;
                    break;
                default:
                    event_id = AuRolljamCustomEventBackPressed;
                    break;
                }
                instance->callback(event_id, instance->context);
            }
            updated = true;
        }
    }

    if(updated) {
        with_view_model(
            instance->view,
            AuRolljamMainViewModel * model,
            { model->index = instance->index; },
            true);
    }

    return updated;
}

static void au_rolljam_main_view_enter(void* context) {
    furi_assert(context);
}

static void au_rolljam_main_view_exit(void* context) {
    furi_assert(context);
}

AuRolljamMainView* au_rolljam_main_view_alloc(void) {
    AuRolljamMainView* instance = malloc(sizeof(AuRolljamMainView));
    instance->view = view_alloc();
    view_allocate_model(instance->view, ViewModelTypeLocking, sizeof(AuRolljamMainViewModel));
    view_set_context(instance->view, instance);
    view_set_draw_callback(instance->view, (ViewDrawCallback)au_rolljam_main_view_draw);
    view_set_input_callback(instance->view, au_rolljam_main_view_input);
    view_set_enter_callback(instance->view, au_rolljam_main_view_enter);
    view_set_exit_callback(instance->view, au_rolljam_main_view_exit);

    instance->index = 0;

    with_view_model(
        instance->view,
        AuRolljamMainViewModel * model,
        { model->index = instance->index; },
        true);

    return instance;
}

void au_rolljam_main_view_free(AuRolljamMainView* instance) {
    furi_assert(instance);
    view_free(instance->view);
    free(instance);
}

View* au_rolljam_main_view_get_view(AuRolljamMainView* instance) {
    furi_assert(instance);
    return instance->view;
}

void au_rolljam_main_view_set_callback(
    AuRolljamMainView* instance,
    AuRolljamMainViewCallback callback,
    void* context) {
    furi_assert(instance);
    instance->callback = callback;
    instance->context = context;
}
