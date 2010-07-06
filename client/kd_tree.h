#ifndef BONSAI_CLIENT_KD_TREE_H
#define BONSAI_CLIENT_KD_TREE_H

#include "object.h"

#include "stdint.h"

/**
 * KD-tree node.
 * The tree is stored in the depth first layout.
 * and the front links are ommited.
 * Optimized to be as small as possible.
 */
struct kd_tree_node{
	union{
		struct{
			_Bool leaf : 1;

			/** 
			 * If this flag is true, then the front subtree
			 * was more probable (and now follows this node in the
			 * nodes list), otherwise it was the back subtree.
			 */
			_Bool frontMoreProbable : 1;

			/**
			 * Index of the less probable child node.
			 * The more probable child always immediately follows
			 * the current node. 0 means that the subtree is missing.
			 */
			unsigned int lessProbableIndex : 28;

			/** Axis of the split. */
			unsigned int axis : 2;

			/** Coordinate of the split */
			float coord;
		} __attribute__((packed));
		struct{
			/** Padding */
			unsigned int x : 1;

			/* Number of objects in the leaf. */
			unsigned int count : 31;

			/**
			 * First object.
			 * Index into the array of objects.
			 */
			uint32_t first;
		} __attribute__((packed));
	} __attribute__((packed));
} __attribute__((packed));

struct kd_tree{
	struct kd_tree_node *nodes;
	struct object *objects;
};

void kd_tree_destroy(struct kd_tree *t);

float kd_tree_ray_intersection(const struct kd_tree *tree,
	const struct ray *r, float lowerBound, float upperBound,
	struct object **result);

void kd_tree_build(struct kd_tree *tree, struct wrapped_object *objs);


#endif
