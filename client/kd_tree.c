#include "kd_tree.h"

#include <assert.h>
#include <stdlib.h>
#include <math.h>

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
