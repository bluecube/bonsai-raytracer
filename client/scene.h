#ifndef BONSAI_CLIENT_SCENE_H
#define BONSAI_CLIENT_SCENE_H

#include "kd_tree.h"
#include "object.h"

/**
 * Scene representation for rendering.
 * This contains some preprocessed values.
 */
struct scene{
	unsigned raysPerPx;

	float focalLength;
	float apertureDiameter;
	
	float focus;

	unsigned width;
	unsigned height;

	float sensorWidth;

	struct kd_tree tree;
};

void scene_init(struct scene *s);
void scene_empty(struct scene *s);

void scene_set_objects(struct scene *s, struct wrapped_object *objs);

float scene_ray_intersection(const struct scene *s, const struct ray *r,
	struct object **found);

#endif
