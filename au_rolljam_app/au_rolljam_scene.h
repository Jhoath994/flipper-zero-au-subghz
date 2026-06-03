#pragma once

#include <gui/scene_manager.h>

/* Generate scene id enum and total count */
#define ADD_SCENE(prefix, name, id) AuRolljamScene##id,
typedef enum {
#include "au_rolljam_scene_config.h"
    AuRolljamSceneNum,
} AuRolljamScene;
#undef ADD_SCENE

extern const SceneManagerHandlers au_rolljam_scene_handlers;

/* Generate scene on_enter handler declarations */
#define ADD_SCENE(prefix, name, id) void prefix##_scene_##name##_on_enter(void*);
#include "au_rolljam_scene_config.h"
#undef ADD_SCENE

/* Generate scene on_event handler declarations */
#define ADD_SCENE(prefix, name, id) \
    bool prefix##_scene_##name##_on_event(void* context, SceneManagerEvent event);
#include "au_rolljam_scene_config.h"
#undef ADD_SCENE

/* Generate scene on_exit handler declarations */
#define ADD_SCENE(prefix, name, id) void prefix##_scene_##name##_on_exit(void* context);
#include "au_rolljam_scene_config.h"
#undef ADD_SCENE
