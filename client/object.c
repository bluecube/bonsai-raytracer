#include "object.h"

#include <stdlib.h>

#include "util.h"

/**
 * Return a new object.
 * Because object is abstract, the returned structure will have
 * some uninitialized fields.
 * \note This function fills the fields invTransform and destroy.
 */
struct object *object_new(const struct transform *t){
	struct object *o = checked_malloc(sizeof(struct object));

	transform_invert(t, &(o->invTransform));

	o->destroy = object_destroy;

	return o;
}

/**
 * Default implementation of object destruction.
 * Only free the memory of struct object.
 */
void object_destroy(struct object *o){
	free(o);
}
