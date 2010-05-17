#ifndef BONSAI_CLIENT_BOUNDING_BOX_H
#define BONSAI_CLIENT_BOUNDING_BOX_H

#include "vector.h"

/**
 * Axis-aligned box.
 * Nonempty box has p1[i] < p2[i] for i = X, Y, Z.
 */
typedef struct{
	vector p1;
	vector p2;
} bounding_box;

double bounding_box_is_nonempty(bounding_box *b);
double bounding_box_area(bounding_box *b);
double bounding_box_volume(bounding_box *b);

#endif
