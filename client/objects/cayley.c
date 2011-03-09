/**
 * \file
 * Implementation of cayley cubic surface.
 * \see http://mathworld.wolfram.com/CayleyCubic.html
 * \see doc/cayley.wxm
 */

#include "objects/cayley.h"

#include "polynomial.h"
#include "vector.h"

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
 */
static float get_intersection(struct object *obj, const struct ray *r,
	float lowerBound, float upperBound){

	(void)obj;

	float coefs[4];

	vector_t o = r->origin;
	vector_t d = r->direction;

	// these expressions are imported from maxima and regexed into C code
	coefs[0] =
		d.x * d.y * d.z;
	coefs[1] =
		d.x * d.y * o.z +
		d.x * o.y * d.z +
		o.x * d.y * d.z +
		d.y * d.z +
		d.x * d.z +
		d.x * d.y;
	coefs[2] =
		d.x * o.y * o.z +
		o.x * d.y * o.z +
		d.y * o.z +
		d.x * o.z +
		o.x * o.y * d.z +
		o.y * d.z +
		o.x * d.z + 
		d.x * o.y + 
		o.x * d.y;
	coefs[3] =
		o.x * o.y * o.z +
		o.y * o.z +
		o.x * o.z +
		o.x * o.y;
	
	return cubic_first_root_in_interval(coefs, lowerBound, upperBound);
}

/**
 * Get the normal vector of the object in a point #v.
 * \pre #v is close to the surface of the object.
 */
static vector_t get_normal(struct object *o, vector_t pt){
	(void)o;
	return vector_set(
		pt.y * pt.y * pt.z * pt.z,
		pt.x * pt.x * pt.z * pt.z,
		pt.x * pt.x * pt.y * pt.y);
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
