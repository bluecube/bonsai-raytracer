#ifndef BONSAI_CLIENT_KD_TREE_H
#define BONSAI_CLIENT_KD_TREE_H

#include "object.h"

struct kd_tree{
	/**
	 * Index of the splitting axis
	 * The splitting plane has equation $$ \vec{X_{axis}} = coord $$.
	 */
	int axis;

	/**
	 * Coordinate of the splitting plane.
	 * The splitting plane has equation $$ \vec{X_{axis}} = coord $$.
	 */
	float coord;

	/**
	 * KD-tree with objects in front of the spliting plane.
	 * For every point of every object in this tree
	 * $$ \vec{X_{axis}} \gt coord $$ holds.
	 */
	struct kd_tree *front;
	/**
	 * KD-tree with objects in behind of the spliting plane.
	 * For every point of every object in this tree
	 * $$ \vec{X_{axis}} < coord $$ holds.
	 */
	struct kd_tree *back;

	/**
	 * Linked list of objects which arent placed in any of the subtrees.
	 * This will be used for objects intersecting the splitting plane
	 * and for objects not yet placed in the tree.
	 */
	struct object *objs;
};


#endif
