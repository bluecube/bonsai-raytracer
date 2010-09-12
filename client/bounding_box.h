#ifndef BONSAI_CLIENT_BOUNDING_BOX_H
#define BONSAI_CLIENT_BOUNDING_BOX_H

#include "ray.h"
#include "transform.h"
#include "vector.h"

/**
 * Axis-aligned box.
 * Nonempty box has p[0][i] < p[1][i] for i = X, Y, Z.
 */
struct bounding_box{
	vector_t p[2];
};

float bounding_box_ray_intersection(const struct bounding_box *b,
	const struct ray *r);
void bounding_box_expand(struct bounding_box *b, vector_t pt);
void bounding_box_expand_box(struct bounding_box *b1,
	const struct bounding_box *b2);

void bounding_box_transform(const struct bounding_box *b,
	const struct transform *t,
	struct bounding_box *ret);

void bounding_box_empty(struct bounding_box *b);

void bounding_box_fix_order(struct bounding_box *b);

float bounding_box_area(const struct bounding_box *b);

#endif
