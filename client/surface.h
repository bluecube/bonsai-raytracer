#ifndef BONSAI_CLIENT_SURFACE_H
#define BONSAI_CLIENT_SURFACE_H

#include "photon.h"
#include "vector.h"

/**
 * Structure representing a sampled direction of an outgoing ray from a surface.
 */
struct outgoing_direction{
	vector_t direction;

	/**
	 * This is what the BRDF would return (from 0 to 1), multiplied by the factor
	 * correcting for eventual importance sampling.
	 */
	float weight;
};

struct surface{
	/**
	 * Sample a random reflected ray.
	 * \param point Point on the object, in object space.
	 * \param wavelength Wavelength of the light ray being examined.
	 * \param normal Normal vector of the surface in point.
	 * \param in Direction of the ray viewing the surface.
	 * \return A sampled direction and it's weighting factor.
	 * Already containing the cosine factor.
	 *
	 * \note All of the vectors must be normalized.
	 */
	struct outgoing_direction (*sample)(vector_t point, wavelength_t wavelength,
		vector_t normal, vector_t in);
};

#endif
