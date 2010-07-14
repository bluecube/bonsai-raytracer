/**
 * \file
 * Implementation of a sphere object.
 * \see /doc/sphere.wxm for the calculations.
 */

#include "sphere.h"

#include "polynomial.h"
#include "util.h"
#include "vector.h"

/**
 * Calculate the minimum bounding box.
 */
static void get_bounding_box(const struct transform *t, struct bounding_box *b){
	for(int i = 0; i < DIMENSIONS; ++i){
		struct vector tmp;
		vector_set(&tmp,
			t->p[0 + i],
			t->p[3 + i],
			t->p[6 + i]);
		float box_size = vector_length(&tmp);
		b->p[0].p[i] = t->p[9 + i] - box_size;
		b->p[1].p[i] = t->p[9 + i] + box_size;
	}

	bounding_box_fix_order(b);
}

/**
 * Get the first intersection point of the sphere that is in the interval
 * \f$ <lowerBound, upperBound> \f$.
 * \return Distance to the intersection (in object coordinates),
 * or NAN if there is no intersection in the interval.
 */
float get_intersection(struct object *o, const struct ray *r,
	float lowerBound, float upperBound){

	float coefs[3];

	coefs[0] = vector_length_squared(&(r->origin)) - 1;
	coefs[1] = 2 * vector_dot(&(r->origin), &(r->direction));
	coefs[2] = 1;

	return quadratic_first_root_in_interval(coefs, lowerBound, upperBound);
}

/**
 * Get the normal vector of the object in a point #v.
 * \pre #v is close to the surface of the object.
 */
static void get_normal(struct object *sphere, const struct vector *v, struct vector *normal){
	*normal = *v;
}

/**
 * Initialize a sphere.
 * Initializes the underlying object structure and calculates the transformed
 * bounding box.
 */
void sphere_init(struct object *o, const struct transform *t){
	object_init(o, t);

	get_bounding_box(t, &(o->boundingBox));

	o->get_intersection = get_intersection;
	o->get_normal = get_normal;
}
