#include "kd_tree.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "util.h"

/**
 * Cost of traversing a KD-tree node relative to a mean cost of
 * intersecting an object (super magical constant).
 * Used for SAH when building a tree.
 * \todo Use some value based on measurements rather than this wild guess.
 */
#define TRAVERSAL_COST 0.5

/**
 * A list of objects with count, pointer to the 
 * terminating NULL pointer and a bounding box.
 * Helper data structure for tree build.
 */
struct object_list{
	struct object *head;
	struct object **end;
	unsigned count;

	struct bounding_box box;
};

/**
 * Empty the object list.
 */
static void object_list_empty(struct object_list *l){
	l->head = NULL;
	l->end = &(l->head);
	l->count = 0;

	bounding_box_empty(&(l->box));
}

/**
 * Append an object to a last position of an object list.
 */
static void object_list_append(struct object_list *l, struct object *o){
	o->next = NULL;

	*(l->end) = o;
	l->end = &(o->next);

	++(l->count);
	bounding_box_expand_box(&(l->box), &(o->boundingBox));

}

/**
 * Append items of #l2 to the back of #l1.
 */
static void object_list_append_list(struct object_list *l1, struct object_list *l2){
	if(l2->count > 0){
		*(l1->end) = l2->head;
		l1->end = l2->end;
		l1->count += l2->count;

		bounding_box_expand_box(&(l1->box), &(l2->box));
	}

	object_list_empty(l2);
}

/**
 * Build a object list from ordinary linked list of objects.
 */
static void object_list_build(struct object_list *l, struct object *o){
	object_list_empty(l);
	while(o != NULL){
		struct object *tmp = o->next;
		object_list_append(l, o);
		o = tmp;
	}
}

/**
 * Set all the items of a kd tree to zero or NULL.
 * Doesn't deallocate anything.
 */
void kd_tree_init(struct kd_tree *t){
	t->axis = 0;
	t->coord = 0;
	t->front = t->back = NULL;
	t->objs = NULL;
}

/**
 * Free all memory used by the kd tree (nodes and objects),
 * except the memory used by *t.
 */
void kd_tree_empty(struct kd_tree *t){
	if(t->front){
		kd_tree_empty(t->front);
		free(t->front);
	}
	if(t->back){
		kd_tree_empty(t->back);
		free(t->back);
	}

	while(t->objs){
		struct object *tmp = t->objs;
		t->objs = t->objs->next;

		tmp->destroy(tmp);
	}
}

/**
 * Compute a first intersection of a scene and a ray with a distance
 * from the ray origin inside the closed interval
 * \f$ <lowerBound, upperBound> \f$, 
 * \return Distance to the intersection (in world coordinates),
 * This value is aways inside the closed interval \f$ <lowerBound, upperBound> \f$, 
 * or NAN if there was no intersection found.
 * \param t KD-tree node in which to look for intersections.
 * \param r Ray.
 * \param lowerBound Lower bound of the intersection distance.
 * \param upperBound Upper bound of the intersection distance.
 * \param[out] found Pointer to pointer to the found object.
 * If no intersection was found, then its value is left unchanged.
 * \pre lowerBound <= upperBound
 */
float kd_tree_ray_intersection(const struct kd_tree *t, const struct ray *r,
	float lowerBound, float upperBound, struct object **result){

	if(t == NULL){
		return NAN;
	}

	assert(lowerBound <= upperBound);

	float distance = NAN;

	// First try to find intersections of the unsorted objects
	for(struct object *o = t->objs; o != NULL; o = o->next){
		float tmp = object_ray_intersection(o, r, lowerBound, upperBound);

		if(!isnan(tmp)){
			distance = tmp;
			*result = o;
			upperBound = tmp;
		}

	}
	
	// Find out where the ray intersects the splitting plane
	int axis = t->axis;
	float splitDistance =
		(t->coord - r->origin.p[axis]) * r->invDirection.p[axis];

	struct kd_tree *first;
	struct kd_tree *second;
	if(r->direction.p[axis] > 0){
		first = t->back;
		second = t->front;
	}else{
		first = t->front;
		second = t->back;
	}

	if(upperBound <= splitDistance){
		float tmp = kd_tree_ray_intersection(first, r, lowerBound,
			upperBound, result);
		if(!isnan(tmp)){
			distance = tmp;
		}
	}else if(lowerBound >= splitDistance){
		float tmp = kd_tree_ray_intersection(second, r, lowerBound,
			upperBound, result);
		if(!isnan(tmp)){
			distance = tmp;
		}
	}else{
		float tmp;
		tmp = kd_tree_ray_intersection(first, r, lowerBound,
			splitDistance, result);
		if(!isnan(tmp)){
			distance = tmp;
		}else{
			tmp = kd_tree_ray_intersection(second, r, splitDistance,
				upperBound, result);
			if(!isnan(tmp)){
				distance = tmp;
			}
		}
	}

	return distance;
}

/**
 * Calculate the SAH cost of a given split.
 * \param wholeBox A box containing all of the objects.
 * \param frontBox A box containing all objects that fall in front of the
 * splitting plane.
 * \param frontCount Number of objects in front of the plane.
 * \param backBox A box containing all objects that fall behind the plane.
 * \param backCount Number of objects behind the plane.
 * \param intersectingCount Number of objects intersecting the splitting plane.
 *
 * \note Uses the TRAVERSAL_COST constant.
 *
 * \todo Try giving 20% bonus for clipping away empty space?
 */
static float sah_split_cost(struct bounding_box *wholeBox,
	struct bounding_box *frontBox,
	unsigned frontCount,
	struct bounding_box *backBox,
	unsigned backCount,
	unsigned intersectingCount){
	
	float wholeArea = bounding_box_area(wholeBox);

	// probabilities of hitting 
	float pFront = bounding_box_area(frontBox) / wholeArea;
	float pBack = bounding_box_area(backBox) / wholeArea;

	float cost = TRAVERSAL_COST +
		intersectingCount + pFront * frontCount + pBack * backCount;

	return cost;
}

/**
 * Calculate the cost of not splitting the list of objects
 * (keeping the current node as a leaf).
 * Since the intersection cost is 1, this function becomes a little too ... simple.
 */
static inline float not_split_cost(unsigned count){
	return count;
}

/**
 * Split the given list on a plane.
 * Outputs three lists of objects that are in front of the plane,
 * intersect it or are behind the plane.
 */
static void split_at(struct object_list *objs, int axis, float position,
	struct object_list *front, struct object_list *back,
	struct object_list *intersecting){

	object_list_empty(front);
	object_list_empty(back);
	object_list_empty(intersecting);

	struct object *o = objs->head;
	while(o != NULL){
		struct object *tmp = o->next;

		if(o->boundingBox.p[0].p[axis] >= position){
			object_list_append(front, o);
		}else if(o->boundingBox.p[1].p[axis] <= position){
			object_list_append(back, o);
		}else{
			object_list_append(intersecting, o);
		}

		o = tmp;
	}

	object_list_empty(objs);
}

/**
 * Calculate the SAH cost of a split when only given the list of objects and
 * then bounding box. Splits the list, calls sah_split_cost() and then merges
 * the list back together (in different order).
 */
static float sah_split_cost_wrapped(struct object_list *objs, int axis, float position){

	struct object_list front, back, intersecting;

	struct bounding_box objsBoxCopy = objs->box;

	split_at(objs, axis, position,
		&front, &back, &intersecting);

	float cost = sah_split_cost(&objsBoxCopy,
		&(front.box), front.count,
		&(back.box), back.count,
		intersecting.count);
	
	object_list_append_list(&front, &intersecting);
	object_list_append_list(&front, &back);

	*objs = front;

	return cost;
}

/**
 * Recursively build a KD-tree in #t from objects in #objs using the surface
 * area heuristic.
 * \return Depth of the tree constructed.
 */
static unsigned kd_tree_build_rec(struct kd_tree *t, struct object_list *objs){

	float bestCost = not_split_cost(objs->count);
	bool bestIsNoSplit = true;
	
	kd_tree_init(t);
	
	// For every object try using all six bounding box faces 
	// as a kd-tree splitting planes.
	for(struct object *o = objs->head; o != NULL; o = o->next){
		for(int axis = 0; axis < DIMENSIONS; ++axis){
			for(int i = 0; i < 2; ++i){
				float position = o->boundingBox.p[i].p[axis];

				float cost = sah_split_cost_wrapped(
					objs, axis, position);

				if(cost < bestCost){
					bestCost = cost;
					bestIsNoSplit = false;
					t->axis = axis;
					t->coord = position;
				}
			}
		}
	}

	if(bestIsNoSplit){
		t->objs = objs->head;

		return 1;
	}else{
		struct object_list front, back, intersecting;
		
		split_at(objs, t->axis, t->coord,
			&front, &back, &intersecting);
		
		t->front = checked_malloc(sizeof(*(t->front)));
		unsigned depthFront = kd_tree_build_rec(t->front, &front);

		t->back = checked_malloc(sizeof(*(t->back)));
		unsigned depthBack = kd_tree_build_rec(t->back, &back);

		t->objs = intersecting.head;

		if(depthFront > depthBack){
			return 1 + depthFront;
		}else{
			return 1 + depthBack;
		}
	}
}

/**
 * Fill a KD-tree t from objects in linked list objs.
 * This is not meant to be blazing fast.
 * Assumes that #t has no allocated memory.
 */
void kd_tree_build(struct kd_tree *t, struct object *objs){
	printf("Building the KD-tree...\n");

	struct object_list list;
	object_list_build(&list, objs);

	unsigned depth = kd_tree_build_rec(t, &list);

	printf("Finished. Tree height is %u.\n", depth);
}
