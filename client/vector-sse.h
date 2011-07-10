/**
 * \file
 * Vectors and operations on them.
 *
 * We're using row vectors.
 */

#ifndef BONSAI_CLIENT_VECTOR_SSE_H
#define BONSAI_CLIENT_VECTOR_SSE_H

#include <xmmintrin.h>
#include <emmintrin.h>
#include <math.h>

#define VECTOR_SIZE 4

struct transform;

/**
 * 3D vector.
 * The w coordinate is mostly ignored.
 */
typedef union{
	float f[VECTOR_SIZE];
	struct{
		float x;
		float y;
		float z;
		float w;
	};
	__m128 m;
} vector_t;

static inline vector_t vector_add(vector_t v1, vector_t v2);
static inline vector_t vector_substract(vector_t v1, vector_t v2);
static inline float vector_dot(vector_t v1, vector_t v2);
static inline float vector_length(vector_t v);
static inline float vector_length_squared(vector_t v);
static inline vector_t vector_multiply(vector_t v1, float f);
static inline vector_t vector_divide(vector_t v, float f);
static inline vector_t vector_normalize(vector_t v);
static inline vector_t vector_set(float x, float y, float z);
static inline vector_t vector_set_normalized(float x, float y, float z);

static inline __m128 vector_dot_internal(__m128 m1, __m128 m2);

vector_t vector_transform(vector_t v, const struct transform *t);
vector_t vector_transform_direction(vector_t v, const struct transform *t);

vector_t vector_random_in_circle();
vector_t vector_random_on_sphere();

/**
 * Sum two vectors, return output.
 */
vector_t vector_add(vector_t v1, vector_t v2){
	vector_t ret;

	ret.m = _mm_add_ps(v1.m, v2.m);

	return ret;
}

/** 
 * Substract v2 from v1, return output.
 */
vector_t vector_substract(vector_t v1, vector_t v2){
	vector_t ret;

	ret.m = _mm_sub_ps(v1.m, v2.m);

	return ret;
}

/**
 * Dot product of two vetcors, return output.
 * Ignores the w component.
 */
float vector_dot(vector_t v1, vector_t v2){
	vector_t tmp;
	tmp.m = vector_dot_internal(v1.m, v2.m);

	return tmp.x;
}

/**
 * Absolute value of a vector.
 * Ignores the w component.
 */
static inline float vector_length(vector_t v){
	return sqrt(vector_length_squared(v));
}

/**
 * Return vector_length(v) * vector_length(v); faster.
 */
static inline float vector_length_squared(vector_t v){
	vector_t tmp;
	tmp.m = vector_dot_internal(v.m, v.m);

	return tmp.x;
}

/**
 * Multiply a vector by a scalar.
 */
static inline vector_t vector_multiply(vector_t v, float f){
	vector_t ret;

	__m128 tmp = _mm_set1_ps(f);
	ret.m = _mm_mul_ps(v.m, tmp);

	return ret;
}

/**
 * Divide a vector by a scalar.
 */
static inline vector_t vector_divide(vector_t v, float f){
	vector_t ret;

	__m128 tmp = _mm_set1_ps(f);
	ret.m = _mm_div_ps(v.m, tmp);

	return ret;
}

/**
 * Change the absolute value of a vector to 1.
 */
static inline vector_t vector_normalize(vector_t v){
	vector_t ret;

	__m128 length = vector_dot_internal(v.m, v.m);
	ret.m = _mm_mul_ps(v.m, _mm_rsqrt_ps(length));

	return ret;
}

/**
 * Set the components of a vector.
 */
static inline vector_t vector_set(float x, float y, float z){
	vector_t ret;
	ret.m = _mm_setr_ps(x, y, z, 0);
	return ret;
}

/**
 * Set components and normalize the vector.
 */
static inline vector_t vector_set_normalized(float x, float y, float z){
	return vector_normalize(vector_set(x, y, z));
}

/**
 * Calculate the dot product of two 3D vectors.
 * Uses SSE, but ignores the w component of vectors.
 * The resulting vector has the x, y and z components set to the
 * dot product.
 */
static inline __m128 vector_dot_internal(__m128 m1, __m128 m2){
	__m128 tmp1 = _mm_mul_ps(m1, m2);
	__m128 tmp2 = tmp1;
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(3, 0, 2, 1));
	tmp1 = _mm_add_ps(tmp1, tmp2);
	tmp2 = _mm_shuffle_ps(tmp2, tmp2, _MM_SHUFFLE(3, 1, 0, 2));
	return _mm_add_ps(tmp1, tmp2);
}

#endif
