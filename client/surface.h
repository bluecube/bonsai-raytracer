#ifndef BONSAI_CLIENT_SURFACE_H
#define BONSAI_CLIENT_SURFACE_H

#include "photon.h"
#include "vector.h"

struct surface{
	/**
	 * Brdf of the surface.
	 * \param point Point on the object, in object space.
	 * \param wavelength Wavelength of the light ray being examined.
	 * \param normal Normal vector of the surface in point.
	 * \param in Direction of the ray viewing the light source.
	 * \param out Direction of the ray bounced from the surface.
	 *
	 * \note All of the vectors must be normalized.
	 */
	float (*brdf)(vector_t point, wavelength_t wavelength,
		vector_t normal, vector_t in, vector_t out);
};

#endif
