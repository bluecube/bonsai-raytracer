/**
 * \file
 * Vectors and operations on them.
 */

#ifndef BONSAI_CLIENT_VECTOR_H
#define BONSAI_CLIENT_VECTOR_H

#define X 0
#define Y 1
#define Z 2

typedef float vector[3];

void vector_add(vector *v1, vector *v2, vector *ret);
float vector_dot(vector *v1, vector *v2);
void vector_multiply(vector *v1, float f, vector *ret);
void vector_divide(vector *v1, float f, vector *ret);

// ...

#endif
