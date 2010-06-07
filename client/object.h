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
	 * Transformation from object coordinates to world coordinates.
	 */
	struct transform invTransform;
	
	struct object *next;

	/**
	 * Get the first positive intersection point of the object.
	 * \return Distance to the intersection (in object coordinates),
	 * or a negative number if there is no intersection or all intersections
	 * are negative.
	 * Bounding box is not checked before this method is called.
	 */
	float (*get_intersection)(struct object *o, const struct ray *r);

	/**
	 * Get the normal vector of the object in a point #v.
	 * The normal returned doesn't have to be normalized.
	 * \pre #v is close to the surface of the object.
	 */
	void (*get_normal)(struct object *o, const struct vector *v, struct vector *normal);
};

struct object *object_new(const struct transform *t);

#endif
