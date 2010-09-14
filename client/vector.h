/**
 * \file
 * Vectors and operations on them.
 * Only includes the correct version with SSE enabled/disabled.
 */

#ifndef BONSAI_CLIENT_VECTOR_H
#define BONSAI_CLIENT_VECTOR_H

#ifdef DISABLE_SSE
#include "vector-nosse.h"
#else
#include "vector-sse.h"
#endif

#endif
