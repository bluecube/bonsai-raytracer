#include "object.h"

#include "util.h"

/**
 * Return a new object.
 * Because object is abstract, the returned structure will have
 * some uninitialized fields.
 */
struct object *object_new(const struct transform *t){
	struct object *o = checked_malloc(sizeof(struct object));

	transform_invert(t, &(o->invTransform));

	return o;
}
