#include "bounding_box.h"

#include <math.h>

/**
 * Calculate a ray-bounding box intersection.
 * Uses slabs.
 * \return Intersection distance or NaN.
 * \todo maybe try Eisemann et al: Fast Ray/Axis-Aligned Bounding Box Overlap
 * Tests using Ray Slopes, or
 * http://www.flipcode.com/archives/SSE_RayBox_Intersection_Test.shtml
 */
float bounding_box_ray_intersection(const struct bounding_box * restrict b,
	const struct ray * restrict r, float lowerBound, float upperBound){

	float t1, t2;

	for(int i = 0; i < 3; ++i){
		unsigned positive = (r->invDirection.f[i] > 0);
		t1 = (b->p[1 - positive].f[i] - r->origin.f[i]) * r->invDirection.f[i];
		t2 = (b->p[positive].f[i] - r->origin.f[i]) * r->invDirection.f[i];

		if(t2 < lowerBound || t1 > upperBound)
			return NAN;

		if(t1 > lowerBound)
			lowerBound = t1;
		if(t2 < upperBound)
			upperBound = t2;
	}

	return lowerBound;
}

/**
 * Expand the bounding box to contain a point.
 */
void bounding_box_expand(struct bounding_box *b, vector_t pt){
	for(int i = 0; i < 3; ++i){
		b->p[0].f[i] = fminf(b->p[0].f[i], pt.f[i]);
		b->p[1].f[i] = fmaxf(b->p[1].f[i], pt.f[i]);
	}
}

/**
 * Expand #b1 to contain #b2 to a minimal bounding box containing the union
 * of #b1 and #b2.
 */
void bounding_box_expand_box(struct bounding_box *b1,
	const struct bounding_box *b2){
	
	for(int i = 0; i < 3; ++i){
		b1->p[0].f[i] = fminf(b1->p[0].f[i], b2->p[0].f[i]);
		b1->p[1].f[i] = fmaxf(b1->p[1].f[i], b2->p[1].f[i]);
	}
}

/**
 * Set #ret to a minimal bounding box containing the intersection
 * of #b1 and #b2.
 * \note May produce an empty box (but not a "correctly"
 * empty box -- it will still have a position; see bounding_box_empty()
 * and bounding_box_union()).
 */
void bounding_box_intersection(const struct bounding_box *b1,
	const struct bounding_box *b2,
	struct bounding_box *ret){

	for(int i = 0; i < 3; ++i){
		ret->p[0].f[i] = fmaxf(b1->p[0].f[i], b2->p[0].f[i]);
		ret->p[1].f[i] = fminf(b1->p[1].f[i], b2->p[1].f[i]);
	}
}

/**
 * Transform a bounding box.
 * Transformed box is still axis aligned (and thus may contain some
 * free space).
 */
void bounding_box_transform(const struct bounding_box *b,
	const struct transform *t,
	struct bounding_box *ret){

	bounding_box_empty(ret);

	// Enumerate all corners of the orignal box.
	for(int x = 0; x < 2; ++x){
		for(int y = 0; y < 2; ++y){
			for(int z = 0; z < 2; ++z){
				vector_t pt = vector_set(
					b->p[x].x,
					b->p[y].y,
					b->p[z].z);

				vector_t transformed = vector_transform(pt, t);

				bounding_box_expand(ret, transformed);
			}
		}
	}
}

/**
 * Verify that p[0].f[i] < p[1].f[i] for i = 0 .. 2 and eventually fix it.
 */
void bounding_box_fix_order(struct bounding_box *b){
	for(int i = 0; i < 3; ++i){
		if(b->p[0].f[i] > b->p[1].f[i]){
			float tmp = b->p[0].f[i];
			b->p[0].f[i] = b->p[1].f[i];
			b->p[1].f[i] = tmp;
		}
	}
}

/**
 * Calculate the surface area of a bounding box.
 * If the box has negative length of a side, the area is 0.
 */
float bounding_box_area(const struct bounding_box *b){
	float x = b->p[1].x - b->p[0].x;
	float y = b->p[1].y - b->p[0].y;
	float z = b->p[1].z - b->p[0].z;

	if(x < 0 || y < 0 || z < 0){
		return 0;
	}

	return 2 * ((x * y) + (y * z) + (x * z));
}

/**
 * Set the bounding box to be empty (from INFINITY to -INFINITY).
 * Box emptied with this method doesn't affect the result of an union.
 */
void bounding_box_empty(struct bounding_box *b){
	for(int i = 0; i < 3; ++i){
		b->p[0].f[i] = INFINITY;
		b->p[1].f[i] = -INFINITY;
	}
}
