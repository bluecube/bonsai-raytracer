#include "light.h"

#include "object.h"

void no_light_init(struct object *obj){
	obj->light.energy = NULL;
}

