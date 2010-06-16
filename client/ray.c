#include "ray.h"

/**
 * Fill in the values for a ray starting in #origin and going through #point.
 * \see ray_prepare()
 */
void ray_from_points(struct ray *r, const struct vector *origin,
	const struct vector *point){
	r->origin = *origin;
	vector_substract(point, origin, &(r->direction));

	ray_prepare(r);
}

/**
 * Normalize direction, compute inverse direction.
 */
void ray_prepare(struct ray *r){
	struct vector tmp;
	vector_normalize(&(r->direction), &tmp);

	r->direction = tmp;

	for(int i = 0; i < DIMENSIONS; ++i){
		r->invDirection.p[i] = 1 / tmp.p[i];
	}

	
}

/**
 * Transform the ray.
 * \todo Maybe there's a space for a little performance improvement?
 */
void ray_transform(const struct ray *r, const struct transform *t, struct ray *ret){
	vector_transform(&(r->origin), t, &(ret->origin));
	vector_transform(&(r->direction), t, &(ret->direction));
	ray_prepare(ret);
}
