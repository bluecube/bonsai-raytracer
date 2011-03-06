#include <assert.h>
#include <stdio.h>
#include <math.h>

#define EPSILON 0.001

#define IS_CLOSE(X, Y) (fabsf((X) - (Y)) < EPSILON)

#ifdef NDEBUG
#undef NDEBUG
#endif
