#include "scene.h"

#include <stdlib.h>

#include "measurements.h"

/**
 * Free all memory allocated by a scene,
 * including all objects.
 */
void scene_destroy(struct scene *s){
	while(s->objs){
		struct object *tmp = s->objs->next;

		s->objs->destroy(s->objs);

		s->objs = tmp;
	}

	free(s);
}

/**
 * Compute a first nonnegative intersection of a scene and a ray.
 * \return Distance to the intersection (in object coordinates),
 * or a negative number if there is no intersection or all intersections
 * are negative.
 * \param s Scene.
 * \param r Ray.
 * \param[out] found Pointer to pointer to the found object.
 * If return value is negative, then this value is NULL.
 */
float scene_ray_intersection(const struct scene *s, const struct ray *r,
	struct object **found){

	// count the intersection
	MEASUREMENTS_RAY_SCENE_INTERSECTION();

	float bestDistance = -1;
	*found = NULL;

	for(struct object *obj = s->objs; obj != NULL; obj = obj->next){
		float distance = obj->get_intersection(obj, r);
		if(distance > 0 && (bestDistance < 0 || distance < bestDistance)){
			bestDistance = distance;
			*found = obj;

		}
	}

	return bestDistance;
}
