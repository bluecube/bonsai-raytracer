#include "ray.h"

/**
 * Fill in the values for a ray starting in #origin and going through #point.
 * \see ray_prepare()
 */
void ray_from_points(struct ray *r, vector_t origin, const vector_t point){
	r->origin = origin;
	r->direction = vector_substract(point, origin);

	ray_prepare(r);
}

/**
 * Normalize direction, compute inverse direction.
 * \return Length of the direction vector before normalization.
 */
float ray_prepare(struct ray *r){
	float length = vector_length(r->direction);
	vector_t dir = vector_divide(r->direction, length);

	r->direction = dir;

	for(int i = 0; i < 3; ++i){
		r->invDirection.f[i] = 1 / dir.f[i];
	}

	return length;
}

/**
 * Transform the ray.
 * \return The transformed length of the transformed direction vector before
 * normalization. This value is used to transform intersection distances from
 * object space to world space.
 * \todo Maybe there's a space for a little performance improvement?
 */
float ray_transform(const struct ray * restrict r,
	const struct transform *restrict t,
	struct ray *restrict ret){

	ret->origin = vector_transform(r->origin, t);
	ret->direction = vector_transform_direction(r->direction, t);

	float ratio = ray_prepare(ret);

	ret->lowerBound = r->lowerBound * ratio;
	ret->upperBound = r->upperBound * ratio;

	return ratio;
}
