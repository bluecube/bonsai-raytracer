#ifndef BONSAI_CLIENT_BOUNDING_BOX_H
#define BONSAI_CLIENT_BOUNDING_BOX_H

#include "vector.h"

/**
 * Axis-aligned box.
 * Nonempty box has p1[i] < p2[i] for i = X, Y, Z.
 */
struct bounding_box{
	vector p1;
	vector p2;
};

double bounding_box_is_nonempty(struct bounding_box *b);
double bounding_box_area(struct bounding_box *b);
double bounding_box_volume(struct bounding_box *b);

#endif
