#ifndef BONSAI_CLIENT_SCENE_H
#define BONSAI_CLIENT_SCENE_H

#include "object.h"

/**
 * Scene representation for rendering.
 * This contains some preprocessed values.
 */
struct scene{
	unsigned raysPerPx;

	float focalLength;
	float apertureDiameter;

	unsigned width;
	unsigned height;

	/**
	 * This is really just to put it all together to some working state.
	 * There's gonna be a kd-tree here :-).
	 * \todo KD-tree instead of a linked list.
	 */
	struct object *objs;
};

void scene_destroy(struct scene *s);

float scene_ray_intersection(const struct scene *s, const struct ray *r,
	struct object **found);

#endif
