#ifndef BONSAI_CLIENT_KD_TREE_H
#define BONSAI_CLIENT_KD_TREE_H

struct kd_tree{
	int axis;
	float coord;

	struct kd_tree *front;
	struct kd_tree *back;

	struct object *intersecting;
};


#endif
