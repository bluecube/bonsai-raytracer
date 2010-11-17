#ifndef BONSAI_CLIENT_LIGHT_H
#define BONSAI_CLIENT_LIGHT_H

#include "photon.h"
#include "vector.h"

struct object;

struct light{
	/**
	 * Power of the emited light.
	 * \param point Point on the object, in object space.
	 * \param wavelength Wavelength of the light ray being examined.
	 * \param normal Normal vector of the surface in point.
	 * \param in Direction of the ray viewing the light source.
	 *
	 * \note All of the vectors must be normalized.
	 */
	float (*energy)(vector_t point, wavelength_t wavelength,
		vector_t normal, vector_t in);
};

void no_light_init(struct object *obj);

#endif
