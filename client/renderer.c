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
 * \param[in,out] Wavelength of the ray.
 * The w->wavelength is an input, and w->value is returned.
 * \todo Most of the rendering work should be here.
 * Now there is only a trivial shading used here.
 */
static void render_ray(const struct scene *s, struct ray *r, struct photon *p){
	struct object *obj;

	MEASUREMENTS_RAY_SCENE_INTERSECTION();
	float distance = kd_tree_ray_intersection(&(s->root), r, 0, INFINITY, &obj);

	if(isnan(distance)){
		// If the ray didn't hit anything, the ray stays black.
		return;
	}

	p->energy = 1;;
}

/**
 * Start rendering a scene.
 * \param s Scene to render.
 * \param
 * \param pixmap Pointer to pixel map with result.
 * Must be large enough to hold #y0 - #y1 rows.
 * \todo Lens simulation!
 */
void renderer_render(const struct scene *s, const struct renderer_chunk *chunk,
	struct color *pixmap){
	int ymax = chunk->top + chunk->height;

	// How many meters per pixel.
	float inc = 1 / (float)(s->width - 1);

	float yy = inc * (chunk->top - (float)(s->height) / 2);

	struct vector filmPoint;
	struct vector lensCenter;

	vector_set(&lensCenter, 0, 0, 0);

	MEASUREMENTS_WARMUP();
	MEASUREMENTS_START();

	for(unsigned y = chunk->top; y < ymax; ++y){
		float xx = -0.5;
		for(unsigned x = 0; x < s->width; ++x){
			color_black(pixmap);
			for(unsigned i = 0; i < s->raysPerPx; ++i){
				struct ray r;

				vector_set(&filmPoint,
					- (xx + random_number(0, inc)),
					- (yy + random_number(0, inc)),
					- s->focalLength);

				ray_from_points(&r, &filmPoint, &lensCenter);
				
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
