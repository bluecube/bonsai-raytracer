#define _XOPEN_SOURCE 500

#include "random.h"

#include <stdlib.h>
#include <float.h>

#if FLT_MANT_DIG == 24 && FLT_RADIX == 2 && !defined(DISABLE_FAST_RANDOM_NUMBER)

union ulong_to_float{
	unsigned long i;
	float f;
};

static unsigned mirand = 1;

/**
 * Get random float from interval <#from, #to>.
 * This is based on the sfrand generator by RGBA.
 * http://rgba.org/articles/sfrand/sfrand.htm
 */
float random_number(float from, float to){
	mirand *= 16807;
	union ulong_to_float a;
	a.i = (mirand & 0x007fffff) | 0x3f800000;
	return (a.f - 1.0) * (to - from) + from;
}

#else

#warning "Using fallback version of the random float generator."

/**
 * Get random float from interval <#from, #to>.
 * Fallback version.
 */
float random_number(float from, float to){
	return random() * (to - from) / RAND_MAX + from;
}

#endif

/**
 * Get a random integer from interval <#from, #to>.
 */
int random_int(int from, int to){
	return (random() % (to - from)) + from;
}

