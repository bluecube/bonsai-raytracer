#ifndef BONSAI_CLIENT_OBJECT_H
#define BONSAI_CLIENT_OBJECT_H

#include "bounding_box.h"
#include "transform.h"

/**
 * Base of a object.
 */
struct object{
	/**
	 * Bounding box in world coordinates.
	 */
	struct bounding_box boundingBox;

	/**
	 * Transformation from world coordinates to object coordinates.
	 */
	transform t;
	
	struct object *next;
};

#endif
