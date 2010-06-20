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
 * \return Length of the direction vector before normalization.
 */
float ray_prepare(struct ray *r){
	struct vector tmp;

	float ret = vector_length(&r->direction);
	vector_divide(&(r->direction), 1 / ret, &tmp);

	r->direction = tmp;

	for(int i = 0; i < DIMENSIONS; ++i){
		r->invDirection.p[i] = 1 / tmp.p[i];
	}

	return ret;
}

/**
 * Transform the ray.
 * \return The transformed length of the transformed direction vector before
 * normalization. This value is used to transform intersection distances from
 * object space to world space.
 * \todo Maybe there's a space for a little performance improvement?
 */
float ray_transform(const struct ray *r, const struct transform *t, struct ray *ret){
	vector_transform(&(r->origin), t, &(ret->origin));
	vector_transform_direction(&(r->direction), t, &(ret->direction));
	return ray_prepare(ret);
}
