/**
 * \file
 * Implementation of a sphere object.
 * \see /doc/sphere.wxm for the calculations.
 */

#include "objects/sphere.h"

#include "polynomial.h"
#include "util.h"
#include "vector.h"

/**
 * Calculate the minimum bounding box.
 */
static void get_bounding_box(const struct transform *t, struct bounding_box *b){
	for(int i = 0; i < 3; ++i){
		vector_t tmp = vector_set(
			t->row[0].f[i],
			t->row[1].f[i],
			t->row[2].f[i]);
		float box_size = vector_length(tmp);
		b->p[0].f[i] = t->row[3].f[i] - box_size;
		b->p[1].f[i] = t->row[3].f[i] + box_size;
	}

	bounding_box_fix_order(b);
}

/**
 * Get the first intersection point of the sphere that is in the interval
 * \f$ <lowerBound, upperBound> \f$.
 * \return Distance to the intersection (in object coordinates),
 * or NAN if there is no intersection in the interval.
 */
static float get_intersection(struct object *o, const struct ray *r,
	float lowerBound, float upperBound){

	(void)o;

	float coefs[3];

	coefs[0] = vector_length_squared(r->origin) - 1;
	coefs[1] = 2 * vector_dot(r->origin, r->direction);
	coefs[2] = 1;

	return quadratic_first_root_in_interval(coefs, lowerBound, upperBound);
}

/**
 * Get the normal vector of the object in a point #v.
 * \pre #v is close to the surface of the object.
 */
static vector_t get_normal(struct object *o, vector_t pt){
	(void)o;

	return pt;
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
