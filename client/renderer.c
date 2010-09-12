#include "renderer.h"

#include <math.h>

#include "kd_tree.h"
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
	float distance = kd_tree_ray_intersection(&(s->tree), r, 0, INFINITY, &obj);

	if(isnan(distance)){
		// If the ray didn't hit anything, the ray stays black.
		return;
	}

	p->energy = 1;
}

/**
 * Start rendering a scene.
 * \param s Scene to render.
 * \param
 * \param pixmap Pointer to pixel map with result.
 * Must be large enough to hold #y0 - #y1 rows.
 */
void renderer_render(const struct scene *s, const struct renderer_chunk *chunk,
	struct color *pixmap){
	int ymax = chunk->top + chunk->height;

	// How many meters per pixel.
	float inc = s->sensorWidth / (float)(s->width - 1);

	// the initial 
	float yy = inc * (chunk->top - (float)(s->height) / 2);

	float focus = s->focus / s->focalLength;

	MEASUREMENTS_WARMUP();
	MEASUREMENTS_START();

	for(unsigned y = chunk->top; y < ymax; ++y){
		float xx = - s->sensorWidth / 2;
		for(unsigned x = 0; x < s->width; ++x){
			color_black(pixmap);
			for(unsigned i = 0; i < s->raysPerPx; ++i){
				struct ray r;

				// filmPoint = {
				// 	-xx - random_number(0, inc),
				// 	-yy - random_number(0, inc),
				// 	-s->focalLength
				// 	}
				// focusPoint = - (lensCenter - filmPoint) * (s->focus / filmPoint.z)

				vector_t focusPoint = vector_set(
					focus * (xx + random_number(0, inc)),
					focus * (yy + random_number(0, inc)),
					s->focus);

				vector_t lensPoint = vector_random_in_circle(s->apertureDiameter);

				ray_from_points(&r, lensPoint, focusPoint);

#ifndef MEASUREMENTS_KD_TREE_STATS
				struct photon p;
				photon_random_init(&p);

				render_ray(s, &r, &p);
				
				photon_add_to_color(&p, pixmap);
#else
				struct object *obj;
				MEASUREMENTS_RAY_SCENE_INTERSECTION();
				kd_tree_ray_intersection(&(s->tree), &r, &obj);
				pixmap->r += measurementsObjectIntersectionCounter;
				pixmap->g += measurementsTreeTraversalCounter;
#endif
			}
			++pixmap;

			xx += inc;
		}
		yy += inc;
	}

	MEASUREMENTS_PRINT();
}
