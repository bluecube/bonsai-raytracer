/**
 * \file
 * Vectors and operations on them.
 * Version with SSE disabled.
 */

#ifndef BONSAI_CLIENT_VECTOR_NOSSE_H
#define BONSAI_CLIENT_VECTOR_NOSSE_H

#include <math.h>

struct transform;

/**
 * 3D vector.
 */
typedef union{
	float f[3];
	struct{
		float x;
		float y;
		float z;
	};
} vector_t;

vector_t vector_add(vector_t v1, vector_t v2);
vector_t vector_substract(vector_t v1, vector_t v2);
float vector_dot(vector_t v1, vector_t v2);
static inline float vector_length(vector_t v);
float vector_length_squared(vector_t v);
vector_t vector_multiply(vector_t v1, float f);
static inline vector_t vector_divide(vector_t v, float f);
static inline vector_t vector_normalize(vector_t v);
static inline vector_t vector_set(float x, float y, float z);
static inline vector_t vector_set_normalized(float x, float y, float z);

vector_t vector_transform(vector_t v, const struct transform *t);
vector_t vector_transform_direction(vector_t v, const struct transform *t);

vector_t vector_random_in_circle(float r);
vector_t vector_random_on_sphere();

/**
 * Absolute value of a vector.
 */
static inline float vector_length(vector_t v){
	return sqrt(vector_length_squared(v));
}

/**
 * Divide a vector by a scalar.
 */
static inline vector_t vector_divide(vector_t v, float f){
	return vector_multiply(v, 1 / f);
}

/**
 * Change the absolute value of a vector to 1.
 */
static inline vector_t vector_normalize(vector_t v){
	return vector_divide(v, vector_length(v));
}

/**
 * Set the components of a vector.
 */
static inline vector_t vector_set(float x, float y, float z){
	vector_t ret = {{x, y, z}};
	return ret;
}

/**
 * Set components and normalize the vector.
 */
static inline vector_t vector_set_normalized(float x, float y, float z){
	return vector_normalize(vector_set(x, y, z));
}

#endif
