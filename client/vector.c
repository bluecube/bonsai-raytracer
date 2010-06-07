#include "vector.h"

#include <math.h>

#include "transform.h"

/**
 * Add two vetors, store output in #ret.
 */
void vector_add(const struct vector *v1, const struct vector *v2,
	struct vector *ret){
	
	for(int i = 0; i < DIMENSIONS; ++i){
		ret->p[i] = v1->p[i] + v2->p[i];
	}
}

/**
 * Dot product of two vetcots, return output.
 */
float vector_dot(const struct vector *v1, const struct vector *v2){
	float ret = 0;

	for(int i = 0; i < DIMENSIONS; ++i){
		ret += v1->p[i] * v2->p[i];
	}

	return ret;
}

/**
 * Absolute value of a vector.
 */
float vector_length(const struct vector *v){
	return sqrt(vector_length_squared(v));
}

/**
 * Return vector_length(v) * vector_length(v); faster.
 */
float vector_length_squared(const struct vector *v){
	float ret = 0;

	for(int i = 0; i < DIMENSIONS; ++i){
		ret += v->p[i] * v->p[i];
	}

	return ret;
}

/**
 * Multiply a vector by a scalar.
 */
void vector_multiply(const struct vector *v1, float f, struct vector *ret){
	for(int i = 0; i < DIMENSIONS; ++i){
		ret->p[i] = v1->p[i] * f;
	}
}

/**
 * Divide a vector by a scalar.
 */
void vector_divide(const struct vector *v1, float f, struct vector *ret){
	vector_multiply(v1, 1 / f, ret);
}

/**
 * Change the absolute value of a vector to 1.
 */
void vector_normalize(const struct vector *v, struct vector *ret){
	vector_divide(v, vector_length(v), ret);
}

/**
 * Set the fields of a vector and normalize if.
 */
void vector_set_normalized(float x, float y, float z, struct vector *ret){
	struct vector tmp;
	vector_set(x, y, z, &tmp);
	vector_normalize(&tmp, ret);
}

/**
 * Set the fields of a vector.
 */
void vector_set(float x, float y, float z, struct vector *ret){
	ret->p[X] = x;
	ret->p[Y] = y;
	ret->p[Z] = z;
}

/**
 * Transform a vector with a transformation.
 */
void transform_vector(const struct vector *v, const struct transform *t,
	struct vector *ret){

	for(int i = 0; i < DIMENSIONS; ++i){
		ret->p[i] = t->p[9 + i];
		for(int j = 0; j < DIMENSIONS; ++j){
			ret->p[i] = v->p[j] * t->p[3 * j + i];
		}
	}
}

