/**
 * \file
 * Vectors and operations on them.
 *
 * We're using column vectors.
 */

#ifndef BONSAI_CLIENT_VECTOR_H
#define BONSAI_CLIENT_VECTOR_H

#define X 0
#define Y 1
#define Z 2

#define DIMENSIONS 3

struct vector{
	float p[DIMENSIONS];
};

void vector_add(const struct vector *v1, const struct vector *v2, struct vector *ret);
float vector_dot(const struct vector *v1, const struct vector *v2);
float vector_length(const struct vector *v);
float vector_length_squared(const struct vector *v);
void vector_multiply(const struct vector *v1, float f, struct vector *ret);
void vector_divide(const struct vector *v1, float f, struct vector *ret);
void vector_normalize(const struct vector *v, struct vector *ret);
void vector_set_normalized(float x, float y, float z, struct vector *ret);
void vector_set(float x, float y, float z, struct vector *ret);

// ...

#endif
