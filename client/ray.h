#ifndef BONSAI_CLIENT_RAY_H
#define BONSAI_CLIENT_RAY_H

#include "vector.h"

/**
 * A ray.
 */
struct ray{
	/** Origin of the ray. */
	struct vector origin;

	/** Normalized direction of the ray. */
	struct vector direction;
};

#endif
