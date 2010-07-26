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
 * Calculate the intersection of an object and a ray.
 * First triest to find out if the ray intersects the bounding box.
 * \note The diffrence between this function and the virtual method
 * object::get_intersection is that this method works in the world
 * coordinates.
 */
float object_ray_intersection(struct object *o, const struct ray *r){
	if(isnan(bounding_box_ray_intersection(&(o->boundingBox), r))){
		return NAN;
	}

	struct ray transformed;
	float ratio = ray_transform(r, &(o->invTransform), &transformed);

	return o->get_intersection(o, &transformed) / ratio;
}
