#ifndef BONSAI_CLIENT_RAY_H
#define BONSAI_CLIENT_RAY_H

#include "vector.h"

#include "transform.h"

/**
 * A ray.
 */
struct ray{
	/** Origin of the ray. */
	vector_t origin;

	/** Normalized direction of the ray. */
	vector_t direction;

	vector_t invDirection;

	/** The lower bound of the distance along the ray. */
	float lowerBound;

	/** The upper bound of the distance along the ray. */
	float upperBound;
};

float ray_transform(const struct ray *r, const struct transform *t,
	struct ray *ret);

void ray_from_points(struct ray *r, vector_t origin, vector_t point);
float ray_prepare(struct ray *r);


#endif
