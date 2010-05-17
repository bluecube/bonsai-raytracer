#include "bounding_box.h"

/**
 * Is volume of the box is greater than zero.
 */
double bounding_box_is_nonempty(bounding_box *b){
	return b->p1[X] < b->p2[X] &&
		b->p1[Y] < b->p2[Y] &&
		b->p1[Z] < b->p2[Z];
}

/**
 * Calculate the volume of a bounding box.
 * The volume may be negative if the box has incorrectly (?)
 * ordered vertices.
 */
double bounding_box_volume(bounding_box *b){
	return (b->p2[X] - b->p1[X]) *
		(b->p2[Y] - b->p1[Y]) *
		(b->p2[Z] - b->p1[Z]);
}

/**
 * Calculate the surface area of a bounding box.
 * The area may be negative if the box has incorrectly (?)
 * ordered vertices.
 */
double bounding_box_area(bounding_box *b){
	float x = b->p2[X] - b->p1[X];
	float y = b->p2[Y] - b->p1[Y];
	float z = b->p2[Z] - b->p1[Z];

	return 2 * ((x * y) + (y * z) + (x * z));
}
