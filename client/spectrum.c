#include "spectrum.h"

#include <string.h>

#include "random.h"

/**
 * Initialize the spectrum to black (no radiation).
 */
void spectrum_black(struct spectrum *s){
	memset(s->bins, 0, sizeof(s->bins[0]) * SPECTRUM_BIN_COUNT);
}

/**
 * Initialize the wavelength to zero energy and random wavelength.
 */
void spectrum_init_random_wavelength(struct wavelength *w){
	w->energy = 0;
	w->wavelength =
		random_number(SPECTRUM_MIN_WAVELENGTH, SPECTRUM_MAX_WAVELENGTH);
}

/**
 * Add the wavelength value to a spectrum.
 */
void spectrum_add_wavelength(struct spectrum *s, const struct wavelength *w){
	s->bins[SPECTRUM_WAVELENGTH_TO_BIN_IDX(w->wavelength)] += w->energy;
}

/**
 * Convert spectrum to a color.
 * \todo This really needs a better sensor response model.
 * The current one is just a poor quick and dirty hack.
 */
void spectrum_to_color(const struct spectrum *s, struct color *c){

	color_black(c);
	
	int blueCutoff = SPECTRUM_WAVELENGTH_TO_BIN_IDX(486);
	for(int i = 0; i < blueCutoff; ++i){
		c->b += s->bins[i];
	}

	int greenCutoff = SPECTRUM_WAVELENGTH_TO_BIN_IDX(605);
	for(int i = blueCutoff; i < greenCutoff; ++i){
		c->g += s->bins[i];
	}

	for(int i = greenCutoff; i < SPECTRUM_BIN_COUNT; ++i){
		c->r += s->bins[i];
	}
}
