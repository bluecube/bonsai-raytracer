/** \file
 * Inline implementations of the vector functions.
 */

#ifndef BONSAI_CLIENT_VECTOR_INLINES_H
#define BONSAI_CLIENT_VECTOR_INLINES_H

#include <math.h>

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
