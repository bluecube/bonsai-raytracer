#include "object.h"

#include <stdlib.h>
#include <math.h>

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
 * Calculate the intersection of an object and a ray with
 * distance within the given bounds.
 * First triest to find out if the ray intersects the bounding box.
 * \note The diffrence between this function and the virtual method
 * object::get_intersection is that this method works in the world
 * coordinates.
 */
float object_ray_intersection(struct object *o, const struct ray *r,
	float lowerBound, float upperBound){

	if(isnan(bounding_box_ray_intersection(
		&(o->boundingBox), r, lowerBound, upperBound))){
		return NAN;
	}

	struct ray transformed;
	float ratio = ray_transform(r, &(o->invTransform), &transformed);

	lowerBound *= ratio;
	upperBound *= ratio;

	return (o->get_intersection(o, &transformed, lowerBound, upperBound)) / ratio;
}
