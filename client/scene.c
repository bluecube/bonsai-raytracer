#include "scene.h"

#include <stdlib.h>

#include "measurements.h"

/**
 * Initialize an empty scene, with all fields zero, doesn't deallocate any memory.
 */
void scene_init(struct scene *s){
	s->raysPerPx = 0;

	s->focalLength = 0;
	s->apertureDiameter = 0;

	s->width = s->height = 0;

	s->tree.nodes = NULL;
	s->tree.objects = NULL;
}

/**
 * Free all the memory used by the scene except the top level structure.
 */
void scene_empty(struct scene *s){
	kd_tree_destroy(&(s->tree));
}

/**
 * Set the objects stored in the scene.
 * Deallocates memory from objs.
 */
void scene_set_objects(struct scene *s, struct wrapped_object *objs){
	kd_tree_build(&(s->tree), objs);
}
