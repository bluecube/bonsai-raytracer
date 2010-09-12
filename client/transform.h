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

struct transform{
	vector_t row[4];
};

void transform_invert(const struct transform *t, struct transform *ret);

void transform_identity(struct transform *t);

#endif
