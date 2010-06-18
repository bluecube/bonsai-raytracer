#ifndef BONSAI_CLIENT_SCENE_H
#define BONSAI_CLIENT_SCENE_H

#include "kd-tree.h"
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

	struct kd_tree root;
};

void scene_destroy(struct scene *s);

float scene_ray_intersection(const struct scene *s, const struct ray *r,
	struct object **found);

#endif
