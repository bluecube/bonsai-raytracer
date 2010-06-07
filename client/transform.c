#include "transform.h"

/**
 * Invert the transformation #t to #ret.
 * This is a matrix inversion, but it has to consider the "hidden"
 * column (0, 0, 0, 1).
 * This is not supposed to be too fast.
 */
void transform_invert(const struct transform *t, struct transform *ret);
