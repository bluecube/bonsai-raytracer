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

typedef float vector[DIMENSIONS];

void vector_add(vector *v1, vector *v2, vector *ret);
float vector_dot(vector *v1, vector *v2);
void vector_multiply(vector *v1, float f, vector *ret);
void vector_divide(vector *v1, float f, vector *ret);
void vector_normalize(vector *v, vector *ret);
void vector_set_normalized(float x, float y, float z, vector *ret);

// ...

#endif
