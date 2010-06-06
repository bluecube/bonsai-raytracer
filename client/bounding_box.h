#ifndef BONSAI_CLIENT_BOUNDING_BOX_H
#define BONSAI_CLIENT_BOUNDING_BOX_H

#include "vector.h"
#include "transform.h"

/**
 * Axis-aligned box.
 * Nonempty box has p[0][i] < p[1][i] for i = X, Y, Z.
 */
struct bounding_box{
	struct vector p[2];
};

void bounding_box_union(const struct bounding_box *b1,
	const struct bounding_box *b2,
	struct bounding_box *ret);
void bounding_box_intersection(const struct bounding_box *b1,
	const struct bounding_box *b2,
	struct bounding_box *ret);
void bounding_box_transform(const struct bounding_box *b,
	const struct transform *t,
	struct bounding_box *ret);

void bounding_box_fix_order(struct bounding_box *b);

double bounding_box_area(const struct bounding_box *b);
double bounding_box_volume(const struct bounding_box *b);

#endif
