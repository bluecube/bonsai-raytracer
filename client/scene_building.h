#ifndef BONSAI_CLIENT_SCENE_BUILDING_H
#define BONSAI_CLIENT_SCENE_BUILDING_H

#include <json.h>

#include "scene.h"

struct scene *scene_build(struct json_object *obj);

#endif
