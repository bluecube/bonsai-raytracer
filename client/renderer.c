#include "renderer.h"

#include <math.h>

#include "kd_tree.h"
#include "measurements.h"
#include "object.h"
#include "random.h"
#include "ray.h"
#include "photon.h"

#define MAX_DEPTH 16

/**
 * Render a single ray from a camera.
 * \param s Scene to render.
 * \param r Ray being cast.
 * \param wavelength
 * \param depth How deep are we in the recursion?
 * \return Energy of the ray.
 */
static float render_ray(const struct scene *s, struct ray *r, wavelength_t wavelength, int depth){
	if(depth > MAX_DEPTH){
		return 0;
	}

	struct object *obj;

	MEASUREMENTS_RAY_SCENE_INTERSECTION();
	float distance = kd_tree_ray_intersection(&(s->tree), r, 0, INFINITY, &obj);

	if(isnan(distance)){
		// If the ray didn't hit anything, it stays black.
		return 0;
	}

	vector_t point = vector_add(r->origin, vector_multiply(r->direction, distance));
	vector_t normal = obj->get_normal(obj, vector_transform(point, &(obj->invTransform)));
	normal = vector_normalize(vector_transform_direction(normal, &(obj->transform)));

	float energy = 0;

	if(object_is_light_source(obj)){
		energy = (obj->light.energy)(point, wavelength, normal, r->direction);
	}

	// chose an outgoing direction of the bounce
	/// \todo Rewrite this.
	vector_t outDirection;
	float cosine;
	do{
		outDirection = vector_random_on_sphere();
		cosine = vector_dot(normal, outDirection);
	}while(vector_dot(normal, outDirection) < 0);

	float coef = (obj->surface.brdf)(point, wavelength, normal, r->direction, outDirection);
	coef *= cosine;

	struct ray newRay;
	ray_from_direction(&newRay, point, outDirection);

	energy += coef * render_ray(s, &newRay, wavelength, depth + 1);

	return energy;
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

	float yy = inc * ((float)(s->height) / 2 - chunk->top);

	float focus = s->focus / s->focalLength;

#if MEASUREMENTS_WITH_WARMUP
	MEASUREMENTS_WARMUP();
#endif
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

				p.energy = render_ray(s, &r, p.wavelength, 0);
				
				photon_add_to_color(&p, pixmap);
#else
				struct object *obj;
				MEASUREMENTS_RAY_SCENE_INTERSECTION();
				kd_tree_ray_intersection(&(s->tree), &r, &obj);
				pixmap->r += measurementsObjectIntersectionCounter;
				pixmap->g += measurementsTreeTraversalCounter;
#endif
			}
			color_scale(pixmap, 1.0f / s->raysPerPx);

			//printf("x = %i y = %i pixmap->r = %.2f\n", x, y ,pixmap->r);
			++pixmap;

			xx += inc;
		}
		yy -= inc;
	}

	MEASUREMENTS_PRINT();
}
