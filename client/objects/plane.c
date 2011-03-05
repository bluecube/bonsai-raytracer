/**
 * \file
 * Implementation of a infinite plane object.
 */

#include "objects/plane.h"

#include "polynomial.h"
#include "util.h"
#include "vector.h"

#define PLANE_NORMAL vector_set(0, 1, 0)

/**
 * Get the intersection point of the plane if it is in the interval
 * \f$ <lowerBound, upperBound> \f$.
 * \return Distance to the intersection (in object coordinates),
 * or NAN if there is no intersection in the interval.
 */
static float get_intersection(struct object *o, const struct ray *r,
	float lowerBound, float upperBound){

	(void)o;

	return inside_interval(- r->origin.y * r->invDirection.y, lowerBound, upperBound);
}

/**
 * Get the normal vector of the object in a point #v.
 * \pre #v is close to the surface of the object.
 */
static vector_t get_normal(struct object *o, vector_t pt){
	(void)o;
	(void)pt;

	return PLANE_NORMAL;
}

/**
 * Calculate the minimum bounding box.
 * \todo Better comparison of floats?
 */
static void get_bounding_box(const struct transform *t, struct bounding_box *b){
	vector_t transformedNormal = vector_transform_direction(PLANE_NORMAL, t);
	vector_t transformedPoint = vector_transform(vector_set(0, 0, 0), t);

	for(int i = 0; i < 3; ++i){
		if(transformedNormal.f[(i + 1) % 3] == 0 && transformedNormal.f[(i + 2) % 3] == 0){
			b->p[0].f[i] = transformedPoint.f[i];
			b->p[1].f[i] = transformedPoint.f[i];
		}else{
			b->p[0].f[i] = -INFINITY;
			b->p[1].f[i] = INFINITY;
		}
	}
}

/**
 * Initialize an infinite plane.
 * Initializes the underlying object structure and calculates the transformed
 * bounding box.
 */
void plane_init(struct object *o, const struct transform *t){
	object_init(o, t);

	get_bounding_box(t, &(o->boundingBox));

	o->get_intersection = get_intersection;
	o->get_normal = get_normal;
}
