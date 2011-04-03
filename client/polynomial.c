/**
 * Working with polynomials.
 * Polynomial is an array of floats, a[i] is the coefficient of \f$x^{degree - i}\f$.
 * Some of the functions are not static to make them simpler to test.
 */

#include "polynomial.h"

#include <math.h>

#include <gsl/gsl_poly.h>

#include <util.h>

#define EPSILON 0.0001
#define IS_CLOSE(X, Y) (fabsf((X) - (Y)) < EPSILON)

/**
 * Return val if it is inside the closed interval or NAN if it isn't.
 */
float inside_interval(float val, float lowerBound, float upperBound){
	return (val >= lowerBound && val <= upperBound) ? val : NAN;
}

/**
 * Find real roots of a polynomial of degree 2.
 * \param coefs Array of coefficients. Must have at least 3 fields,
 * coefs[i] is the coefficient of x^i.
 * \param[out] roots Array used for outputting the roots. The resulting roots
 * are in arbitrary order. Must have at least two fields.
 * \note coefs and roots may alias.
 * \return number of roots (0, 1 or 2).
 */
unsigned quadratic_solve(const float coefs[], float roots[]){
	float a = coefs[0];
	float b = coefs[1];
	float c = coefs[2];

	float d = b * b - 4 * a * c;

	if(d < 0){
		return 0;
	}else if(IS_CLOSE(d, 0)){
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
 * \todo http://stackoverflow.com/questions/4503849/quadratic-equation-in-ada/4504415#4504415
 * \return Root, or NAN if no such root exists.
 */
float quadratic_first_root_in_interval(const float coefs[],
	float lowerBound, float upperBound){

	float a = coefs[0];
	float b = coefs[1];
	float c = coefs[2];

	float d = b * b - 4 * a * c;

	if(d < 0){
		return NAN;
	}else if(IS_CLOSE(d, 0)){
		return inside_interval(-b / (2 * a), lowerBound, upperBound);
	}else{
		d = sqrtf(d);

		float root = (-b - d) / (2 * a);
		if(root >= lowerBound && root <= upperBound){
			return root;
		}

		return inside_interval((-b + d) / (2 * a), lowerBound, upperBound);
	}
}

/**
 * Find roots of a polynomial.
 * \param[in,out] coefs Coefficients of the polynomial.
 * These values are destroyed during the computation.
 * \param[out] roots Array that will contain the roots. This has to be large enough.
 * \return Number of roots found.
 * \todo There may be some trouble with multiple roots.
 */
unsigned cubic_solve(float coefs[], float roots[]){
	double r1, r2, r3;
	unsigned ret = gsl_poly_solve_cubic(
		coefs[1] / coefs[0], coefs[2] / coefs[0], coefs[3] / coefs[0],
		&r1, &r2, &r3);
	
	roots[0] = r1;
	roots[1] = r2;
	roots[2] = r3;

	return ret;
}

/**
 * Find a minimal real root of a given cubic polynomial that is inside a given
 * (closed) interval.
 * \param[in,out] coefs Coefficients of the polynomial.
 * These values are destroyed during the computation.
 * \return Root, or NAN if no such root exists.
 */
float cubic_first_root_in_interval(float coefs[],
	float lowerBound, float upperBound){

	float roots[3];
	unsigned numRoots = cubic_solve(coefs, roots);

	float best = NAN;

	for(unsigned i = 0; i < numRoots; ++i){
		if(roots[i] >= lowerBound &&
			roots[i] <= upperBound &&
			!(roots[i] >= best)){

			best = roots[i];
		}
	}

	return best;
}
