#include "surfaces/lambertian.h"

#include "random.h"
#include "transform.h"

#include <assert.h>

/**
 * BRDF of the simplest surface.
 * \todo Measure & optimize.
 * \todo Importance sampling?
 */
static struct outgoing_direction sample(vector_t point, wavelength_t wavelength,
	vector_t normal, vector_t in){
	
	(void)point;
	(void)wavelength;
	(void)in;

/*
	vector_t sample = vector_random_in_circle();
	sample.z = sqrt(1 - vector_length_squared(sample));

	struct transform basis;
	transform_make_orthonormal_basis(&basis, normal);

	struct outgoing_direction ret;
	ret.direction = vector_transform_direction(sample, &basis);
	ret.weight = 1;
*/

	struct outgoing_direction ret;

	float len;
	float dot;
	do{
		ret.direction.x = random_number(-1, 1);
		ret.direction.y = random_number(-1, 1);
		ret.direction.z = random_number(-1, 1);

		len = vector_length_squared(ret.direction);
		dot = vector_dot(ret.direction, normal);
	}while(len > 1 || dot < 0);
	len = sqrt(len);
	vector_multiply(ret.direction, 1 / len);
	ret.weight = 2 * dot;

	return ret;
}

void lambertian_init(struct object *o){
	o->surface.sample = sample;
}

