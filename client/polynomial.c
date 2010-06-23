#include "polynomial.h"

#include <math.h>

/**
 * Return val if it is inside the closed interval or NAN if it isn't.
 */
static float inside_interval(float val, float lowerBound, float upperBound){
	return (val >= lowerBound && val <= upperBound) ? val : NAN;
}

/**
 * Find real roots of a polynomial of degree 2.
 * \param[in] coefs Array of coefficients. Must have at least 3 fields,
 * coefs[i] is the coefficient of x^i.
 * \param[out] roots Array used for outputting the roots.
 * Must have at least two fields. Roots are returned in ascending order.
 * \return number of roots (0, 1 or 2).
 */
unsigned quadratic_solve(const float coefs[], float roots[]){
	float a = coefs[2];
	float b = coefs[1];
	float c = coefs[0];

	float d = b * b - 4 * a * c;

	if(d < 0){
		return 0;
	}else if(d == 0){
		roots[0] = -b / (2 * a);
		return 1;
	}else{
		d = sqrtf(d);
		roots[0] = (-b - d) / (2 * a);
		roots[1] = (-b + d) / (2 * a);
		return 2;
	}
}

/**
 * Find a minimal real root of a given quadratic polynomial that is inside a given
 * (closed) interval.
 * \param[in] coefs Array of coefficients. Must have at least 3 fields,
 * coefs[i] is the coefficient of x^i.
 * \return Root, or NAN if no such root exists.
 */
float quadratic_first_root_in_interval(const float coefs[],
	float lowerBound, float upperBound){

	float a = coefs[2];
	float b = coefs[1];
	float c = coefs[0];

	float d = b * b - 4 * a * c;

	if(d < 0){
		return NAN;
	}else if(d == 0){
		return inside_interval(-b / (2 * a), lowerBound, upperBound);
	}else{
		float root = (-b - d) / (2 * a);
		if(root >= lowerBound && root <= upperBound){
			return root;
		}

		return inside_interval((-b + d) / (2 * a), lowerBound, upperBound);
	}
}
