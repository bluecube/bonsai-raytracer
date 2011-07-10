#ifndef DISABLE_SSE

#include "vector-sse.h"

#include <math.h>

#include "random.h"
#include "transform.h"

/**
 * Transform a point with a transformation.
 * \todo This should be a inline calling vector_transform_direction.
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
 * The circle lies in the Z plane and has radius 1.
 */
vector_t vector_random_in_circle(){
	__m128 coords;
	__m128 dist;
	do{
		coords = random_number2();
		
		dist = _mm_mul_ps(coords, coords);
		dist = _mm_add_ss(dist, _mm_movehl_ps(dist, dist));
	}while(_mm_comige_ss(dist, _mm_set_ss(1)));

	vector_t ret;
	ret.m = _mm_shuffle_ps(coords, _mm_setzero_ps(), _MM_SHUFFLE(0, 0, 2, 0));

	return ret;
}

/**
 * Get a random point uniformly distributed on a unit sphere.
 */
vector_t vector_random_on_sphere(){
	float x, y, z, len;
	do{
		x = random_number(-1, 1);
		y = random_number(-1, 1);
		z = random_number(-1, 1);

		len = x * x + y * y + z * z;
	}while(len > 1);

	x /= len;
	y /= len;
	z /= len;

	return vector_set(x, y, z);
}

#endif

/** 
 * \function vector_random_on_sphere
 * \todo Vectorize this.
 */
