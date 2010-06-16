#include "renderer.h"

#include <math.h>

#include "measurements.h"
#include "object.h"
#include "random.h"
#include "ray.h"
#include "photon.h"

/**
 * Render a single ray from a camera.
 * \param s Scene to render.
 * \param r Ray being cast.
 * \param[in,out] Wavelength of the ray. The w->wavelength is an input, and w->value is returned.
 * \todo Most of the rendering work should be here. Now there is only a trivial shading used here.
 */
static void render_ray(struct scene *s, struct ray *r, struct photon *p){
	struct object *obj;
	float distance = scene_ray_intersection(s, r, &obj);

	if(distance < 0){
		// If the ray didn't hit anything, the ray stays black.
		return;
	}

	p->energy = 0.5;
}

/**
 * Start rendering a scene.
 * \param s Scene to render.
 * \param y0 First row to be rendered.
 * \param y1 First row after the block to be rendered.
 * \param pixmap Pointer to pixel map with result.
 * Must be large enough to hold #y0 - #y1 rows.
 * \todo Lens simulation!
 */
void renderer_render(struct scene *s, unsigned y0, unsigned y1, struct color *pixmap){
	MEASUREMENTS_WARMUP();
	MEASUREMENTS_START();

	float inc = 1 / (s->width - 1);

	float yy = inc * (y0 - s->height / 2);
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
				
				struct photon p;
				photon_random_init(&p);

				render_ray(s, &r, &p);
				
				photon_add_to_color(&p, pixmap);
			}
			++pixmap;

			xx += inc;
		}
		yy += inc;
	}

	MEASUREMENTS_PRINT();
}
