#include "object.h"

#include <stdlib.h>

#include "util.h"

/**
 * Initialize an object.
 * \note This function only fills invTransform.
 */
void object_init(struct object *o, const struct transform *t){
	transform_invert(t, &(o->invTransform));
}

/**
 * Free the memory of struct object.
 */
void object_destroy(struct object *o){
	free(o);
}

/**
 * Caluculate the intersection of an object and a ray with
 * distance within the given bounds.
 * \note The diffrence between this function and the virtual method
 * object::get_intersection is that this method works in the world
 * coordinates.
 */
float object_ray_intersection(struct object *o, const struct ray *r,
	float lowerBound, float upperBound){

	struct ray transformed;
	float ratio = ray_transform(r, &(o->invTransform), &transformed);

	lowerBound *= ratio;
	upperBound *= ratio;

	return (o->get_intersection(o, &transformed, lowerBound, upperBound)) / ratio;
}
