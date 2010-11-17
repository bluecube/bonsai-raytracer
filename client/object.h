#ifndef BONSAI_CLIENT_OBJECT_H
#define BONSAI_CLIENT_OBJECT_H

#include <stdbool.h>
#include <stdlib.h>

#include "bounding_box.h"
#include "light.h"
#include "ray.h"
#include "surface.h"

#include "transform.h"

/**
 * Geometric object with a surface.
 * \note Don't forget to add items that need to be aligned
 * together to the front.
 */
struct object{
	/**
	 * Bounding box in world coordinates.
	 */
	struct bounding_box boundingBox;


	/**
	 * Transformation from object coordinates to world coordinates.
	 * \todo Is it necessary to store this?
	 */
	struct transform transform;

	/**
	 * Transformation from world coordinates to object coordinates.
	 */
	struct transform invTransform;
	
	/**
	 * Get the first intersection point of the object that is in the interval
	 * \f$ <lowerBound, upperBound> \f$.
	 * \return Distance to the intersection (in object coordinates),
	 * or NAN if there is no intersection in the interval.
	 * Bounding box is not checked before this method is called.
	 */
	float (*get_intersection)(struct object *o, const struct ray *r,
		float lowerBound, float upperBound);

	/**
	 * Get the normal vector of the object in a point in object space #pt.
	 * The normal returned doesn't have to be normalized.
	 * \pre #v is close to the surface of the object.
	 */
	vector_t (*get_normal)(struct object *o, vector_t pt);


	struct surface surface;
	struct light light;
} __attribute__ ((aligned (16)));

/**
 * Structure used when building the tree.\
 * Only a object with a pointer to the next one.
 */
struct wrapped_object{
	struct object o;

	struct wrapped_object *next;
};


void object_init(struct object *o, const struct transform *t);
void object_destroy(struct object *o);

float object_ray_intersection(struct object *o, const struct ray *r,
	float lowerBound, float upperBound);

static inline bool object_is_light_source(const struct object *o){
	return o->light.energy != NULL;
}

#endif
