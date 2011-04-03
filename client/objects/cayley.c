/**
 * \file
 * Implementation of cayley cubic surface.
 * \see https://secure.wikimedia.org/wikipedia/en/wiki/File:CayleyCubic.png
 * \see http://mathworld.wolfram.com/CayleyCubic.html
 * \see http://www.mathcurve.com/surfaces/cayley/cayley.shtml
 * \see doc/cayley.wxm
 */

#include "objects/cayley.h"

#include "polynomial.h"
#include "vector.h"

#include <stdio.h>

/**
 * Calculate the minimum bounding box.
 * this is pretty simple, since it is infinite in all three dimensions ;-)
 */
static void get_bounding_box(const struct transform *t, struct bounding_box *b){
	(void)t;

	for(int i = 0; i < 3; ++i){
		b->p[0].f[i] = -INFINITY;
		b->p[1].f[i] = INFINITY;
	}
}

/**
 * Get the first intersection point of the cayley cubic if it is in the interval
 * \f$ <lowerBound, upperBound> \f$.
 * This function assumes that all the vectors in a ray have 1 in their w components.
 * \return Distance to the intersection (in object coordinates),
 * or NAN if there is no intersection in the interval.
 * \note In this function I added a sphere clipping that should
 * theoretically be done elsewhere (as a CSG operation) and differently,
 * but since CSG is not implemented yet ...
 */
static float get_intersection(struct object *obj, const struct ray *r,
	float lowerBound, float upperBound){

	(void)obj;

	float sphere[3];

	sphere[0] = 1;
	sphere[1] = 2 * vector_dot(r->origin, r->direction);
	sphere[2] = vector_length_squared(r->origin) - 16;

	if(quadratic_solve(sphere, sphere) != 2){
		return NAN;
	}

	/* Intersecting the sphere interval and the original limiting interval. */
	if(sphere[0] < sphere[1]){
		lowerBound = fmaxf(lowerBound, sphere[0]);
		upperBound = fminf(upperBound, sphere[1]);
	}else{
		lowerBound = fmaxf(lowerBound, sphere[1]);
		upperBound = fminf(upperBound, sphere[0]);
	}

	float coefs[4];

	vector_t o = r->origin;
	vector_t d = r->direction;

	coefs[0] = 2 * d.x * d.y * d.z;
	coefs[1] = 2 * (
		d.x * d.y * o.z +
		d.x * o.y * d.z +
		o.x * d.y * d.z) + 1;
	coefs[2] = 2 * (
		d.x * o.y * o.z +
		o.x * d.y * o.z +
		o.x * o.y * d.z +
		vector_dot(d, o));
	coefs[3] =
		2 * o.x * o.y * o.z +
		vector_length_squared(o) - 1;
	
	return cubic_first_root_in_interval(coefs, lowerBound, upperBound);
}

/**
 * Get the normal vector of the object in a point #v.
 * \pre #v is close to the surface of the object.
 */
static vector_t get_normal(struct object *o, vector_t pt){
	(void)o;
	return vector_add(pt, vector_set(
		pt.y * pt.z,
		pt.x * pt.z,
		pt.x * pt.y));
}

/**
 * Initialize Cayley surface.
 * Initializes the underlying object structure and calculates the transformed
 * bounding box.
 */
void cayley_init(struct object *o, const struct transform *t){
	object_init(o, t);

	get_bounding_box(t, &(o->boundingBox));

	o->get_intersection = get_intersection;
	o->get_normal = get_normal;
}
