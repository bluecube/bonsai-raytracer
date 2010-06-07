#include "scene.h"

#include <stdlib.h>

#include "object.h"

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
