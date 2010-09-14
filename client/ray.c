#include "ray.h"

#ifndef DISABLE_SSE
#include <xmmintrin.h>
#endif

/**
 * Fill in the values for a ray starting in #origin and going through #point.
 * \see ray_prepare()
 */
void ray_from_points(struct ray *r, vector_t origin, const vector_t point){
	r->origin = origin;
	r->direction = vector_normalize(vector_substract(point, origin));
#ifndef DISABLE_SSE
	r->invDirection.m = _mm_rcp_ps(r->direction.m);
#else
	for(int i = 0; i < 3; ++i){
		r->invDirection.f[i] = 1 / r->direction.f[i];
	}
#endif
}

/**
 * Transform the ray.
 * \return The transformed length of the transformed direction vector before
 * normalization. This value is used to transform intersection distances from
 * object space to world space.
 */
float ray_transform(const struct ray * restrict r,
	const struct transform *restrict t,
	struct ray *restrict ret){

	ret->origin = vector_transform(r->origin, t);
	ret->direction = vector_transform_direction(r->direction, t);

#ifndef DISABLE_SSE
	float length = vector_length(ret->direction);
	vector_t dir = vector_divide(ret->direction, length);

	ret->direction = dir;
	
	for(int i = 0; i < 3; ++i){
		ret->invDirection.f[i] = 1 / dir.f[i];
	}

	return length;
#else
	vector_t length;
	length.m = __mm_sqrt_ps(vector_dot_internal(r->direction.m, r->direction.m));
	r->direction.m = _mm_div_ps(v.m, length.m);

	r-invDirection.>m = _mm_rcp_ps(r->direction.m)

	return length.x;
#endif
}
