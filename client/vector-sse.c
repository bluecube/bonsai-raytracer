#ifndef DISABLE_SSE

#include "vector-sse.h"

#include <math.h>

#include "random.h"
#include "transform.h"

/**
 * Transform a point with a transformation.
 */
vector_t vector_transform(vector_t v, const struct transform *t){
	vector_t ret;
	__m128 tmp;
	
	tmp = _mm_shuffle_ps(v.m, v.m, _MM_SHUFFLE(0, 0, 0, 0));
	ret.m = _mm_mul_ps(tmp, t->row[0].m);

	tmp = _mm_shuffle_ps(v.m, v.m, _MM_SHUFFLE(1, 1, 1, 1));
	ret.m = _mm_add_ps(ret.m, _mm_mul_ps(tmp, t->row[1].m));

	tmp = _mm_shuffle_ps(v.m, v.m, _MM_SHUFFLE(2, 2, 2, 2));
	ret.m = _mm_add_ps(ret.m, _mm_mul_ps(tmp, t->row[2].m));

	ret.m = _mm_add_ps(ret.m, t->row[3].m);

	return ret;
}


/**
 * Transform a direction with a transformation.
 */
vector_t vector_transform_direction(vector_t v, const struct transform *t){
	vector_t ret;
	__m128 tmp;
	
	tmp = _mm_shuffle_ps(v.m, v.m, _MM_SHUFFLE(0, 0, 0, 0));
	ret.m = _mm_mul_ps(tmp, t->row[0].m);

	tmp = _mm_shuffle_ps(v.m, v.m, _MM_SHUFFLE(1, 1, 1, 1));
	ret.m = _mm_add_ps(ret.m, _mm_mul_ps(tmp, t->row[1].m));

	tmp = _mm_shuffle_ps(v.m, v.m, _MM_SHUFFLE(2, 2, 2, 2));
	ret.m = _mm_add_ps(ret.m, _mm_mul_ps(tmp, t->row[2].m));

	return ret;
}

/**
 * Get a random point inside a circle using rejection sampling.
 * The circle lies in the Z plane and has radius r.
 */
vector_t vector_random_in_circle(float r){
	float x, y;

	do{
		x = random_number(-r, r);
		y = random_number(-r, r);
	}while(x * x + y * y > r * r);

	return vector_set(x, y, 0);
}

#endif
