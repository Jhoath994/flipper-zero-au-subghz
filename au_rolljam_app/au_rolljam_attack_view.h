#pragma once

#include <gui/view.h>
#include <input/input.h>
#include <gui/elements.h>

#include "au_rolljam_state.h"

typedef struct AuRolljamAttackView AuRolljamAttackView;
typedef void (*AuRolljamAttackViewCallback)(uint32_t event, void* context);

AuRolljamAttackView* au_rolljam_attack_view_alloc(void);
void au_rolljam_attack_view_free(AuRolljamAttackView* instance);
View* au_rolljam_attack_view_get_view(AuRolljamAttackView* instance);
void au_rolljam_attack_view_set_callback(
    AuRolljamAttackView* instance,
    AuRolljamAttackViewCallback callback,
    void* context);
void au_rolljam_attack_view_set_state(
    AuRolljamAttackView* instance,
    RollJamState state,
    const char* status_text);
void au_rolljam_attack_view_set_codes_info(
    AuRolljamAttackView* instance,
    bool code_a_captured,
    bool code_b_captured,
    const char* proto_a,
    const char* proto_b);
