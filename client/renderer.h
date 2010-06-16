#ifndef BONSAI_CLIENT_RENDERER_H
#define BONSAI_CLIENT_RENDERER_H

#include "color.h"
#include "scene.h"

void renderer_render(struct scene *s, unsigned y0, unsigned y1, struct color *pixmap);

#endif
