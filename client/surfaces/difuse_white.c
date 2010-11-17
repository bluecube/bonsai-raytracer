#include "surfaces/difuse_white.h"

/**
 * BRDF of the simplest surface.
 */
static float brdf(vector_t point, wavelength_t wavelength,
	vector_t normal, vector_t in, vector_t out){
	
	(void)point;
	(void)wavelength;
	(void)normal;
	(void)in;
	(void)out;

	return 1;
}

void difuse_white_init(struct object *o){
	o->surface.brdf = brdf;
}

