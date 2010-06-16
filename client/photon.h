#ifndef BONSAI_CLIENT_SPECTRUM_H
#define BONSAI_CLIENT_SPECTRUM_H

#include "color.h"

/**
 * The lowest wavelength representable.
 * \note This value has to match the table in spectrum.c !
 */
#define PHOTON_MIN_WAVELENGTH 375

/**
 * The highest wavelength representable.
 * \note This value has to match the table in spectrum.c !
 */
#define PHOTON_MAX_WAVELENGTH 800

/**
 * A single photon group.
 * This is associated with every ray being cast.
 */
struct photon{
	int wavelength;
	float energy;
};

void photon_random_init(struct photon *p);
void photon_add_to_color(const struct photon *p, struct color *c);

#endif
