#ifndef BONSAI_CLIENT_RENDERER_H
#define BONSAI_CLIENT_RENDERER_H

#include "color.h"
#include "scene.h"

/**
 * Group of rows to render.
 */
struct renderer_chunk{
	unsigned top;
	unsigned height;
};

void renderer_render(const struct scene *s, const struct renderer_chunk *chunk,
	struct color *pixmap);

#endif
