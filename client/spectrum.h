#ifndef BONSAI_CLIENT_SPECTRUM_H
#define BONSAI_CLIENT_SPECTRUM_H

#include "color.h"

/**
 * The lowest wavelength representable.
 */
#define SPECTRUM_MIN_WAVELENGTH 380

/**
 * The highest wavelength representable.
 */
#define SPECTRUM_MAX_WAVELENGTH 750

/**
 * Number of bins used to store the spectrum.
 */
#define SPECTRUM_BIN_COUNT 256

/**
 * Wavelength width of a spectrum bin.
 */
#define SPECTRUM_BIN_WIDTH \
	((SPECTRUM_MAX_WAVELENGTH - SPECTRUM_MIN_WAVELENGTH) / SPECTRUM_BIN_COUNT)

#define SPECTRUM_WAVELENGTH_TO_BIN_IDX(wavelength) \
	((int)(((wavelength) - SPECTRUM_MIN_WAVELENGTH) / SPECTRUM_BIN_WIDTH))

/**
 * Structure representing a light spectum.
 */
struct spectrum{
	float bins[SPECTRUM_BIN_COUNT];
};

/**
 * A single wavelength.
 * This is associated with every ray being cast.
 */
struct wavelength{
	float wavelength;
	float energy;
};

void spectrum_black(struct spectrum *s);
void spectrum_init_random_wavelength(struct wavelength *w);
void spectrum_add_wavelength(struct spectrum *s, const struct wavelength *w);

void spectrum_to_color(const struct spectrum *s, struct color *c);

#endif
