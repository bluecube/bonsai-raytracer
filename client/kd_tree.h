#ifndef BONSAI_CLIENT_KD_TREE_H
#define BONSAI_CLIENT_KD_TREE_H

#include "object.h"

#include "stdint.h"

/**
 * KD-tree node.
 * The tree is stored in the depth first layout.
 * and the more probable links are ommited.
 * Optimized to be as small as possible.
 */
struct kd_tree_node{
	/**
	 * The various small fields.
	 * Use the KD_TREE_NODE_*_MASK and KD_TREE_NODE_*_SHIFT macros.
	 */
	uint32_t flags;

	union{
		/**
		 * Coordinate of the split
		 * For inner nodes.
		 */
		float coord;
		
		/**
		 * First object.
		 * Index into the array of objects.
		 * For leaves.
		 */
		uint32_t first;
	};
	//uint32_t padding1, padding2;
} __attribute__((packed));

/**
 * Leaf flag of the KD-tree.
 */
#define KD_TREE_NODE_LEAF_MASK 0x80000000

/**
 * Flag for inner nodes; if it is true then the front node ( >= coord)
 * is more probable and immediately follows this node.
 */
#define KD_TREE_NODE_FRONT_MORE_PROBABLE_MASK 0x40000000

/**
 * Index of the less probable child node.
 * \{
 */
#define KD_TREE_NODE_LESS_PROBABLE_INDEX_MASK 0x3ffffffc
#define KD_TREE_NODE_LESS_PROBABLE_INDEX_SHIFT 2
/** \} */

/**
 * The axis of the split in an inner node.
 * \{
 */
#define KD_TREE_NODE_AXIS_MASK 0x00000003
#define KD_TREE_NODE_AXIS_SHIFT 0
/** \} */

/**
 * Number of items in the leaf node.
 * \{
 */
#define KD_TREE_NODE_COUNT_MASK 0x7fffffff
#define KD_TREE_NODE_COUNT_SHIFT 0
/** \} */

struct kd_tree{
	struct kd_tree_node *nodes;
	struct object *objects;
};

void kd_tree_init(struct kd_tree *t);
void kd_tree_destroy(struct kd_tree *t);

float kd_tree_ray_intersection(const struct kd_tree *tree,
	const struct ray *r, float lowerBound, float upperBound,
	struct object **result);

void kd_tree_build(struct kd_tree *tree, struct wrapped_object *objs);


#endif
