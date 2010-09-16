#ifndef BONSAI_CLIENT_RANDOM_H
#define BONSAI_CLIENT_RANDOM_H

#ifndef DISABLE_SSE
#include <emmintrin.h>
#endif

float random_number(float from, float to);
int random_int(int from, int to);

#ifndef DISABLE_SSE
__m128 random_number2();
#endif

void init_random();

#endif
