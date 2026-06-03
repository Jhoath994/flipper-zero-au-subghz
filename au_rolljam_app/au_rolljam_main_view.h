#pragma once

#include <gui/view.h>
#include <input/input.h>
#include <gui/elements.h>

typedef struct AuRolljamMainView AuRolljamMainView;
typedef void (*AuRolljamMainViewCallback)(uint32_t event, void* context);

AuRolljamMainView* au_rolljam_main_view_alloc(void);
void au_rolljam_main_view_free(AuRolljamMainView* instance);
View* au_rolljam_main_view_get_view(AuRolljamMainView* instance);
void au_rolljam_main_view_set_callback(
    AuRolljamMainView* instance,
    AuRolljamMainViewCallback callback,
    void* context);
