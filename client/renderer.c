#include "renderer.h"

#include <assert.h>
#include <math.h>

#include "kd_tree.h"
#include "measurements.h"
#include "object.h"
#include "photon.h"
#include "random.h"
#include "ray.h"
#include "surface.h"

#define MAX_DEPTH 16

/**
 * Pointer to a light source.
 * \todo This is part of an ugly hack and should either
 * go through some massive refactoring or be removed completely.
 */
static struct object *light;

/**
 * Sample a point on a light source.
 * Returns point in camera coordinates.
 * \todo This is part of an ugly hack and should either
 * go through some massive refactoring or be removed completely.
 */
static vector_t sample_light(){
	vector_t position;
	float len;
	do{
		position.x = random_number(-1, 1);
		position.y = random_number(-1, 1);
		position.z = random_number(-1, 1);

		len = vector_length_squared(position);
	}while(len > 1);

	return vector_transform(position, &(light->transform));
}

/**
 * Render a single ray from a camera.
 * \param s Scene to render.
 * \param r Ray being cast.
 * \param wavelength
 * \param depth How deep are we in the recursion?
 * \return Energy of the ray.
 */
static float render_ray(const struct scene *s, struct ray *r, wavelength_t wavelength, int depth, bool *hitLight){
	if(depth > MAX_DEPTH){
		return 0;
	}

	struct object *obj = NULL;

	MEASUREMENTS_RAY_SCENE_INTERSECTION();
	float distance = kd_tree_ray_intersection(&(s->tree), r, 0, INFINITY, &obj);

	if(isnan(distance)){
		// If the ray didn't hit anything, it stays black.
		return 0;
	}

	vector_t pointInCameraSpace = vector_add(r->origin, vector_multiply(r->direction, distance));
	vector_t pointInObjectSpace = vector_transform(pointInCameraSpace, &(obj->invTransform));
	vector_t normalInObjectSpace = obj->get_normal(obj, pointInObjectSpace);
	vector_t normalInCameraSpace = vector_normalize(vector_transform_direction(normalInObjectSpace, &(obj->transform)));

#if DOT_PRODUCT_SHADING
	float dot = -vector_dot(normalInCameraSpace, r->direction);
	if(dot < 0)
		return 0;
	return dot;
#else
	/// \todo Surface properties shouldn't be in camera space.

	float energy = 0; // the resulting energy 

	*hitLight = object_is_light_source(obj);
	if(*hitLight){
		energy = (obj->light.energy)(pointInCameraSpace, wavelength, normalInCameraSpace, r->direction);
	}

	// cast the two other rays - BRDF and DLS:

	// BRDF ray:
	float brdfEnergy = 0;
	bool brdfHitLight = false;
	struct ray brdfRay;

	struct outgoing_direction sample =
		(obj->surface.sample)(pointInCameraSpace, wavelength, normalInCameraSpace, r->direction);
	ray_from_direction(&brdfRay, pointInCameraSpace, sample.direction);
	brdfEnergy = sample.weight * render_ray(s, &brdfRay, wavelength, depth + 1, &brdfHitLight);

	// DLS ray:
	float dlsEnergy;
	bool dlsHitLight;
	struct ray dlsRay;

	vector_t dlsSample = sample_light();
	ray_from_points(&dlsRay, pointInCameraSpace, dlsSample);
	distance = kd_tree_ray_intersection(&(s->tree), &dlsRay, 0, INFINITY, &obj);
	vector_t lightPointInCameraSpace = vector_add(dlsRay.origin, vector_multiply(dlsRay.direction, distance));
	dlsHitLight = object_is_light_source(obj) &&
		(distance + 0.001 >= vector_length(vector_substract(dlsSample, lightPointInCameraSpace)));
		
	if(dlsHitLight){
		float weight = vector_dot(dlsRay.direction, normalInCameraSpace) / (distance * distance);
		if(weight < 0){
			weight = 0;
		}
		dlsEnergy = weight * (obj->light.energy)(dlsSample, wavelength, vector_set(0, 0, 0), dlsRay.direction);
			// the normal (zero vector) is unused in this implementation so i don't calculate it
	}

	// now there is dlsRay with dlsEnergy and brdfRay with brdfEnergy

	float brdfProb = 1 / (2 * 3.1415926);
	float dlsProb = distance / (2 * 3.1415926);

	if(dlsHitLight){
		energy += dlsEnergy * dlsProb / (dlsProb + brdfProb);
	}
	
	if(brdfHitLight){
		energy += brdfEnergy * brdfProb / (dlsProb + brdfProb);
	}else{
		energy += brdfEnergy;
	}

	return energy;
#endif
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
	unsigned ymax = chunk->top + chunk->height;

	// How many meters per pixel.
	float inc = s->sensorWidth / (float)(s->width - 1);

	float yy = inc * ((float)(s->height) / 2 - chunk->top);

	float focus = s->focus / s->focalLength;

	// another part of ugly hack here:
	struct ray tmpRay;
	ray_from_direction(&tmpRay, vector_set(0, 0, 0), vector_set(0, 0, s->focalLength));
	kd_tree_ray_intersection(&(s->tree), &tmpRay, 0, INFINITY, &light);

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

				vector_t lensPoint = vector_multiply(vector_random_in_circle(), s->apertureDiameter);

				ray_from_points(&r, lensPoint, focusPoint);

#ifndef MEASUREMENTS_KD_TREE_STATS
				struct photon p;
				photon_random_init(&p);

				bool x;

				p.energy = render_ray(s, &r, p.wavelength, 0, &x);

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
