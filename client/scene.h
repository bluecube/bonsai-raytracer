#ifndef BONSAI_CLIENT_SCENE_H
#define BONSAI_CLIENT_SCENE_H

/**
 * Scene representation for rendering.
 * This contains some preprocessed values.
 */
struct scene{
	unsigned sampleCount;

	float focalLength;
	float apertureDiameter;

	unsigned width;
	unsigned height;

	/**
	 * This is really just to put it all
	 * together to some working state soon.
	 * There's gonna be a kd-tree here :-).
	 * \todo KD-tree instead of a linked list.
	 */
	struct object *objs;
};

void scene_destroy(struct scene *s);

#endif
