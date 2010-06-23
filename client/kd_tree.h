#ifndef BONSAI_CLIENT_KD_TREE_H
#define BONSAI_CLIENT_KD_TREE_H

#include "object.h"

struct kd_tree{
	/**
	 * Index of the splitting axis
	 * The splitting plane has equation \f$ \vec{X_{axis}} = coord \f$.
	 */
	int axis;
	
	/**
	 * Coordinate of the splitting plane.
	 * The splitting plane has equation \f$ \vec{X_{axis}} = coord \f$.
	 */
	float coord;
	
	/**
	 * KD-tree with objects in front of the spliting plane.
	 * For every point of every object in this tree
	 * \f$ \vec{X_{axis}} \ge coord \f$ holds.
	 */
	struct kd_tree *front;
	/**
	 * KD-tree with objects in behind of the spliting plane.
	 * For every point of every object in this tree
	 * \f$ \vec{X_{axis}} \le coord \f$ holds.
	 */
	struct kd_tree *back;
	
	/**
	 * Linked list of objects which arent placed in any of the subtrees.
	 * This will be used for objects intersecting the splitting plane
	 * and for objects not yet placed in the tree.
	 */
	struct object *objs;
};

void kd_tree_init(struct kd_tree *t);
void kd_tree_empty(struct kd_tree *t);

float kd_tree_ray_intersection(const struct kd_tree *t, const struct ray *r,
	float lowerBound, float upperBound, struct object **result);

void kd_tree_build(struct kd_tree *t, struct object *objs);


#endif
