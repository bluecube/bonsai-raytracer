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
	for(int i = 0; i < 3; ++i){
		struct vector tmp;
		vector_set(
			t->p[0 + i],
			t->p[3 + i],
			t->p[6 + i],
			&tmp);
		float box_size = vector_length(&tmp);
		b->p[0].p[i] = t->p[9 + i] + box_size;
		b->p[1].p[i] = t->p[9 + i] - box_size;
	}

	bounding_box_fix_order(b);
}

/**
 * Find out if a ray is intersecting the sphere and return the first positive
 * intersection distance or a negative number.
 * \todo Check if using the bounding box first wouldn't help.
 */
static float get_intersection(struct object *sphere, const struct ray *r){
	float coefs[3];

	coefs[0] = vector_length_squared(&(r->origin));
	coefs[1] = vector_dot(&(r->origin), &(r->direction));
	coefs[2] = 1;

	float results[2];

	unsigned nroots = quadratic_solve(coefs, results);

	if(nroots == 0){
		return -1;
	}else if(nroots == 1){
		return results[0];
	}else if(results[0] > 0){
		return results[0];
	}else{
		return results[1];
	}
}

static void get_normal(struct object *sphere, const struct vector *v, struct vector *normal){
	*normal = *v;
}

struct object *sphere_new(const struct transform *t){
	struct object *o = object_new(t);
	
	get_bounding_box(t, &(o->boundingBox));

	o->get_intersection = get_intersection;
	o->get_normal = get_normal;

	return o;
}
