#include "transform.h"

/**
 * Transform vector #v to #ret using transformation #t.
 */
void transform_vector(const struct transform *t, const struct vector *v,
	struct vector *ret);

void transform_invert(const struct transform *t, struct transform *ret);
