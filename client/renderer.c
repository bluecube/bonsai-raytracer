#include "renderer.h"

#include <math.h>

#include "color.h"
#include "random.h"
#include "ray.h"

void renderer_render_ray(struct scene *s, struct ray *r, struct color *result){
	
}

/**
 * Start rendering a scene.
 * \param s Scene to render.
 * \param y0 First row to be rendered.
 * \param y1 First row after the block to be rendered.
 * \param pixmap Pointer to pixel map with result.
 * Must be large enough to hold #y0 - #y1 rows.
 */
void renderer_render(struct scene *s, unsigned y0, unsigned y1, struct color *pixmap){
	float inc = 1 / (s->width - 1);

	float yy = inc * (y0 - s->height / 2);
	float weight = 1 / s->raysPerPx;
	for(unsigned y = y0; y < y1; ++y){
		float xx = -0.5;
		for(unsigned x = 0; x < s->width; ++x){
			color_black(pixmap);
			for(unsigned i = 0; i < s->raysPerPx; ++i){
				struct ray r;

				vector_set(0, 0, 0, &(r.origin));
				vector_set_normalized(
					xx + random_number(0, inc),
					yy + random_number(0, inc),
					s->focalLength,
					&r.direction);
				
				struct color result;
				renderer_render_ray(s, &r, &result);

				color_add(pixmap, &result);
			}
			color_scale(pixmap, weight);
			++pixmap;

			xx += inc;
		}
		yy += inc;
	}
}
