#ifndef BONSAI_CLIENT_OBJECT_H
#define BONSAI_CLIENT_OBJECT_H

#include "bounding_box.h"
#include "ray.h"
#include "transform.h"

/**
 * Base of a object.
 * If this is used as a part of a more complicated object,
 * it has to be the first item of the structure.
 */
struct object{
	/**
	 * Bounding box in world coordinates.
	 */
	struct bounding_box boundingBox;

	/**
	 * Transformation from world coordinates to object coordinates.
	 */
	struct transform invTransform;
	
	struct object *next;

	float (*intersection)(const struct ray *r);
	void (*get_normal)(const struct vector v, struct vector normal);
};

struct object *object_new(const struct transform *t);

#endif
