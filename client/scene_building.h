#ifndef BONSAI_CLIENT_SCENE_BUILDING_H
#define BONSAI_CLIENT_SCENE_BUILDING_H

#include <json.h>

#include "scene.h"

#define protocol_error(format, ...) \
	error_internal(0, NULL, 0, NULL, \
		"Protocol error (" format ")", ##__VA_ARGS__)

struct scene *scene_build(struct json_object *obj);

#endif
