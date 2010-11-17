#ifndef BONSAI_CLIENT_RAY_H
#define BONSAI_CLIENT_RAY_H

#include "transform.h"
#include "vector.h"

/**
 * A ray.
 */
struct ray{
	/** Origin of the ray. */
	vector_t origin;

	/** Normalized direction of the ray. */
	vector_t direction;

	vector_t invDirection;
};

float ray_transform(const struct ray *r, const struct transform *t,
	struct ray *ret);

void ray_from_points(struct ray *r, vector_t origin, vector_t point);
void ray_from_direction(struct ray *r, vector_t origin, vector_t dir);


#endif
