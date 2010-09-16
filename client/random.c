#define _XOPEN_SOURCE 500

#include "random.h"

#include <stdlib.h>
#include <float.h>

#ifndef DISABLE_SSE
#include <emmintrin.h>
#endif

#if FLT_MANT_DIG == 24 && FLT_RADIX == 2 && !defined(DISABLE_FAST_RANDOM_NUMBER)

union uint_to_float{
	unsigned i;
	float f;
};

/**
 * PRNG state for the fast float generator.
 */
static unsigned mirand = 1;

/**
 * Get random float from interval <#from, #to>.
 * This is based on the sfrand generator by RGBA.
 * http://rgba.org/articles/sfrand/sfrand.htm
 */
float random_number(float from, float to){
	mirand *= 16807;
	union uint_to_float a;
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

#ifndef DISABLE_SSE

/** 
 * PRNG state for the SSE based 2 floats generator.
 * \todo Check these initialization values!!
 */
static __m128i mirand2;

/**
 * Return two random floats from 0 to 1 in positions 0 and 2.
 */
__m128 random_number2(){
	mirand2 = _mm_mul_epu32(mirand2, _mm_set1_epi32(16807));

	__m128i tmp;

	tmp = _mm_and_si128(mirand2, _mm_set1_epi32(0x007fffff));
	tmp = _mm_or_si128(tmp, _mm_set1_epi32(0x3f800000));

	__m128 ret = _mm_castsi128_ps(tmp);

	ret = _mm_sub_ps(ret, _mm_set1_ps(1));

	return ret;
}

/**
 * Initialize the PRNG state for SSE random generator
 */
void init_random(){
	mirand2 = _mm_set_epi32(0, 1, 0, 2);
}

#else

void init_random(){

}

#endif
