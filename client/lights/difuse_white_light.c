#include "lights/difuse_white_light.h"

/**
 * Power function
 */
static float energy(vector_t point, wavelength_t wavelength,
	vector_t normal, vector_t in){
	
	(void)point;
	(void)wavelength;
	(void)normal;
	(void)in;

	return 1;
}

void difuse_white_light_init(struct object *o){
	o->light.energy = energy;
}

