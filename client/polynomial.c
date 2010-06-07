#include "polynomial.h"

#include <math.h>

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
		d = sqrt(d);
		roots[0] = -b - d / (2 * a);
		roots[1] = -b + d / (2 * a);
		return 2;
	}
}
