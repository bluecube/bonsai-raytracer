/**
 * \file
 * Transformation matrices.
 *
 * Transformation matrix has three rows and four columns.
 * There is one more implicit row with values (0, 0, 0, 1)
 * and each vector has one more implicit 1.
 */

#ifndef BONSAI_CLIENT_TRANSFORM_H
#define BONSAI_CLIENT_TRANSFORM_H

#include "vector.h"

typedef float transform[4 * 3];

void transform_vector(transform t, vector v, vector ret);

#endif
