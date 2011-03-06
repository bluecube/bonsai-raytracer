#include "../test_utils.h"

#include <polynomial.h>
#include <stdlib.h>

// polynomial.h doesn't define headers for its internal functions.
void polynomial_deflate(float coefs[], size_t degree, float root);
float polynomial_newton_step(const float coefs[], size_t degree, float estimate);

int compare(const void *a, const void *b){
	const float *aa = a;
	const float *bb = b;
	if(*aa < *bb){
		return -1;
	}else if(*aa > *bb){
		return 1;
	}else{
		return 0;
	}
}

/**
 * Compares two arrays as if they were multisets.
 * Sorts both set1 and set2.
 */
int arrays_as_sets(float set1[], float set2[], unsigned size){
	qsort(set1, size, sizeof(float), compare);
	qsort(set2, size, sizeof(float), compare);

	for(int i = 0; i < size; ++i){
		if(!IS_CLOSE(set1[i], set2[i])){
			return 0;
		}
	}

	return 1;
}

void test_deflate(){
	float poly[] = {1, -3, 2};
		// x^2 - 3 * x + 2 = (x - 1) * (x - 2)
	polynomial_deflate(poly, 2, 1); // remove the "1" root
	assert(IS_CLOSE(poly[1] / poly[0], -2));
	
	float poly2[] = {2, -3, 9, -8};
		// 2 * x^3 - 3 * x^2 + 9 * x - 8 = (x - 1) * (2 * x^2 - x + 8)
	polynomial_deflate(poly2, 3, 1); // remove the "1" root
	assert(IS_CLOSE(poly2[1] / poly2[0], -0.5));
	assert(IS_CLOSE(poly2[2] / poly2[0], 4));
}

void test_newton(){
	float poly[] = {2, -3, 9, -8};
	assert(IS_CLOSE(polynomial_newton_step(poly, 3, 1), 1));
	assert(IS_CLOSE(polynomial_newton_step(poly, 3, 0), .8888888888888888));
	assert(IS_CLOSE(polynomial_newton_step(poly, 3, 5), 3.356589147286822));
	assert(IS_CLOSE(polynomial_newton_step(poly, 3, -100), -66.49164315530697));

	float poly2[] = {1, 1, -5, 3};
	assert(IS_CLOSE(polynomial_newton_step(poly2, 3, 0), .6));
	assert(IS_CLOSE(polynomial_newton_step(poly2, 3, -4), -3.285714285714286));
}

void test_quadratic(){
	float roots[2];

	float poly[] = {1, -5, 0};
	float expected_roots[] = {0, 5};
	unsigned count = quadratic_solve(poly, roots);
	assert(count == 2);
	assert(arrays_as_sets(roots, expected_roots, count));

	assert(IS_CLOSE(quadratic_first_root_in_interval(poly, 3, INFINITY), 
		expected_roots[1]));
	assert(IS_CLOSE(quadratic_first_root_in_interval(poly, 0, INFINITY), 
		expected_roots[0]));
	assert(IS_CLOSE(quadratic_first_root_in_interval(poly, 0, 1), 
		expected_roots[0]));
	assert(isnan(quadratic_first_root_in_interval(poly, 0.5, 1)));
	assert(isnan(quadratic_first_root_in_interval(poly, -INFINITY, -5)));
	
	float poly2[] = {1, -2, 1};
	count = quadratic_solve(poly2, roots);
	assert(count == 1);
	assert(IS_CLOSE(roots[0], 1.0));
	
	float poly3[] = {1, -1, 5};
	count = quadratic_solve(poly3, roots);
	assert(count == 0);
}

void test_cubic(){
	float roots[3];

	float poly[] = {1, -6, 11, -6};
	float expected_roots[] = {1, 2, 3};
	unsigned count = cubic_solve(poly, roots);
	assert(count == 3);
	assert(arrays_as_sets(roots, expected_roots, count));

	float poly2[] = {1, -7, 15, -9};
	float expected_roots2[] = {1, 3};
	count = cubic_solve(poly2, roots);
	assert(count == 2);
	assert(arrays_as_sets(roots, expected_roots2, count));
	
	float poly3[] = {1, -1, 1, -1};
	count = cubic_solve(poly3, roots);
	assert(count == 1);
	assert(IS_CLOSE(roots[0], 1));

	// \todo: Test first_root_in_interval
}

int main(){
	test_deflate();
	test_newton();
	test_quadratic();
	test_cubic();
	
	return 0;
}
