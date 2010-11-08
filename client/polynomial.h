/**
 * \file
 * Solving polynomials.
 * Only quadratics for now.
 */
#ifndef BONSAI_CLIENT_POLYNOMIAL_H
#define BONSAI_CLIENT_POLYNOMIAL_H

float inside_interval(float val, float lowerBound, float upperBound);

unsigned quadratic_solve(const float coefs[], float roots[]);

float quadratic_first_root_in_interval(const float coefs[],
	float lowerBound, float upperBound);

#endif
