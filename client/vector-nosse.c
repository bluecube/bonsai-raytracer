#ifdef DISABLE_SSE

#include "vector-nosse.h"

#include <math.h>

#include "random.h"
#include "transform.h"

/**
 * Sum two vectors, return output.
 */
vector_t vector_add(vector_t v1, vector_t v2){
	vector_t ret;

	for(int i = 0; i < VECTOR_SIZE; ++i){
		ret.f[i] = v1.f[i] + v2.f[i];
	}

	return ret;
}

/** 
 * Substract v2 from v1, return output.
 */
vector_t vector_substract(vector_t v1, vector_t v2){
	vector_t ret;

	for(int i = 0; i < VECTOR_SIZE; ++i){
		ret.f[i] = v1.f[i] - v2.f[i];
	}

	return ret;
}

/**
 * Dot product of two vetcors, return output.
 */
float vector_dot(vector_t v1, vector_t v2){
	float ret = 0;

	for(int i = 0; i < VECTOR_SIZE; ++i){
		ret += v1.f[i] * v2.f[i];
	}

	return ret;
}

/**
 * Return vector_length(v) * vector_length(v); faster.
 */
float vector_length_squared(vector_t v){
	float ret = 0;

	for(int i = 0; i < VECTOR_SIZE; ++i){
		ret += v.f[i] * v.f[i];
	}

	return ret;
}

/**
 * Multiply a vector by a scalar.
 */
vector_t vector_multiply(vector_t v, float f){
	vector_t ret;

	for(int i = 0; i < VECTOR_SIZE; ++i){
		ret.f[i] = v.f[i] * f;
	}

	return ret;
}

/**
 * Transform a point (vector with the fourth value equqal to 1) with a transformation.
 */
vector_t vector_transform(vector_t v, const struct transform *t){
	vector_t ret;

	for(int i = 0; i < VECTOR_SIZE; ++i){
		ret.f[i] = t->row[VECTOR_SIZE].f[i];
		for(int j = 0; j < VECTOR_SIZE; ++j){
			ret.f[i] += v.f[j] * t->row[j].f[i];
		}
	}

	return ret;
}


/**
 * Transform a direction (vector with the fourth value equqal to 0) with a transformation.
 */
vector_t vector_transform_direction(vector_t v, const struct transform *t){
	vector_t ret;

	for(int i = 0; i < VECTOR_SIZE; ++i){
		ret.f[i] = 0;
		for(int j = 0; j < VECTOR_SIZE; ++j){
			ret.f[i] += v.f[j] * t->row[j].f[i];
		}
	}

	return ret;
}

/**
 * Get a random point inside a circle using rejection sampling.
 * The circle lies in the Z plane and has radius r.
 */
vector_t vector_random_in_circle(){
	float x, y;

	do{
		x = random_number(-1, 1);
		y = random_number(-1, 1);
	}while(x * x + y * y > 1);

	return vector_set(x, y, 0);
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
