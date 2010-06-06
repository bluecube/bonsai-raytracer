#include "bounding_box.h"

/**
 * Set #ret to a minimal bounding box containing the union
 * of #b1 and #b2.
 * \note Assumes that the box is nonempty (see bounding_box_is_nonempty()).
 */
void bounding_box_union(const struct bounding_box *b1,
	const struct bounding_box *b2,
	struct bounding_box *ret){
	
}

void bounding_box_intersection(const struct bounding_box *b1,
	const struct bounding_box *b2,
	struct bounding_box *ret);

void bounding_box_transform(const struct bounding_box *b,
	const struct transform *t,
	struct bounding_box *ret){

}

/**
 * Verify that p[0].p[i] < p[1].p[i] for i = X, Y, Z and
 * eventually fix it.
 */
void bounding_box_fix_order(struct bounding_box *b){
	for(int i = 0; i < DIMENSIONS; ++i){
		if(b->p[0].p[i] > b->p[1].p[i]){
			float tmp = b->p[0].p[i];
			b->p[0].p[i] = b->p[1].p[i];
			b->p[1].p[i] = tmp;
		}
	}
}

/**
 * Calculate the volume of a bounding box.
 * The volume may be negative if the box has incorrectly (?)
 * ordered vertices.
 */
double bounding_box_volume(const struct bounding_box *b){
	return (b->p[1].p[X] - b->p[0].p[X]) *
		(b->p[1].p[Y] - b->p[0].p[Y]) *
		(b->p[1].p[Z] - b->p[0].p[Z]);
}

/**
 * Calculate the surface area of a bounding box.
 * The area may be negative if the box has incorrectly (?)
 * ordered vertices.
 */
double bounding_box_area(const struct bounding_box *b){
	float x = b->p[1].p[X] - b->p[0].p[X];
	float y = b->p[1].p[Y] - b->p[0].p[Y];
	float z = b->p[1].p[Z] - b->p[0].p[Z];

	return 2 * ((x * y) + (y * z) + (x * z));
}
