#include "transform.h"

#include <stdio.h>
#include <string.h>

#include "util.h"

/**
 * Calculate a determinant of the top left 3x3 submatrix of a transformation.
 */
static float det_3x3_top_left(const struct transform *t){
	float det = 0;
	for(int i = 0; i < 3; ++i){
		det +=
			t->row[0].f[(i + 0) % 3] *
			t->row[1].f[(i + 1) % 3] *
			t->row[2].f[(i + 2) % 3];

		det -=
			t->row[0].f[(i + 3) % 3] *
			t->row[1].f[(i + 2) % 3] *
			t->row[2].f[(i + 1) % 3];
	}

	return det;
}

/**
 * Calculate a determinant of a 2x2 matrix obtained from #t by taking rows #row1 and #row2
 * and columns #col1 and #col2.
 * Row and column indices are zero based and must be specified in the correct order.
 * No checks are done.
 */
static float det_2x2(const struct transform *t, int row1, int col1, int row2, int col2){
	return
		t->row[row1].f[col1] * t->row[row2].f[col2] -
		t->row[row1].f[col2] * t->row[row2].f[col1];
}

/**
 * Invert the top left 3x3 submatrix using the adjugate matrix.
 * The translation part of the transformation is unchanged.
 * Calls error() if the matrix is not invertible.
 */
static void invert_top_left_3x3(const struct transform *t, struct transform *ret){
	float det = det_3x3_top_left(t);
	if(det == 0){
		error(0, "Transformation matrix is not invertible.");
	}
	
	ret->row[0].f[0] = det_2x2(t, 1, 1, 2, 2) / det;
	ret->row[0].f[1] = det_2x2(t, 0, 2, 2, 1) / det;
	ret->row[0].f[2] = det_2x2(t, 0, 1, 1, 2) / det;
	ret->row[1].f[0] = det_2x2(t, 1, 2, 2, 0) / det;
	ret->row[1].f[1] = det_2x2(t, 0, 0, 2, 2) / det;
	ret->row[1].f[2] = det_2x2(t, 0, 2, 1, 0) / det;
	ret->row[2].f[0] = det_2x2(t, 1, 0, 2, 1) / det;
	ret->row[2].f[1] = det_2x2(t, 0, 1, 2, 0) / det;
	ret->row[2].f[2] = det_2x2(t, 0, 0, 1, 1) / det;
}

/**
 * Invert the transformation #t to #ret.
 * Computes an inversion of the top left 3x3 submatrix and then separately
 * adds the inversion of the translation part.
 * Calls error() if the matrix is not invertible.
 * \note This is not supposed to be too fast.
 */
void transform_invert(const struct transform *t, struct transform *ret){
	invert_top_left_3x3(t, ret);

	// inversion of a translation matrix is a matrix with negated values in (3,0) to (3,2).
	// The following loops are a part of a multiplication of such matrix with the inverted transform
	// without translation.
	for(int i = 0; i < 3; ++i){
		ret->row[3].f[i] = 0;
		for(int j = 0; j < 3; ++j){
			ret->row[3].f[i] -= t->row[3].f[j] * ret->row[j].f[i];
		}
	}

	#if VECTOR_SIZE == 4
	ret->row[3].f[3] = 1;
	#endif
}

/**
 * Load a identity transformation.
 */
void transform_identity(struct transform *t){
	memset(t, 0, sizeof(struct transform));

	for(int i = 0; i < 3; ++i){
		t->row[i].f[i] = 1;
	}
}

/**
 * Fills rows of transformation t with orthonormal
 * basis of a space that transforms the Z axis to vector v.
 * The other two rows are chosen in some weird (but deterministic) way.
 * Assumes that v is already normalized.
 * \todo Refactor, measure and optimize.
 */
void transform_make_orthonormal_basis(struct transform *t, vector_t v){
	vector_t basex;

	// Test to see if the vector (1, 0 ,0) is too close to v.
	// v.x > 0.86 translates approximately to
	// vector_dot(v, vector_set(1, 0, 0)) > cos(PI/6)
	if(v.x > 0.86){
		basex = vector_set(0, 0, 1);
	}else{
		basex = vector_set(1, 0, 0);
	}

	vector_t basey = vector_set(
		v.y * basex.z - v.z * basex.y,
		v.z * basex.x - v.x * basex.z,
		v.x * basex.y - v.y * basex.x);

	basex = vector_set(
		v.y * basey.z - v.z * basey.y,
		v.z * basey.x - v.x * basey.z,
		v.x * basey.y - v.y * basey.x);
	
	t->row[0] = basex;
	t->row[1] = basey;
	t->row[2] = v;
	t->row[3] = vector_set(0, 0, 0);
}

/**
 * Pretty printing of transformation matrices for debugging purposes.
 */
void transform_print(const struct transform *t){


	printf("[");
	for(unsigned i = 0; i < 4; ++i){
		if(i != 0){
			printf(" ");
		}
		printf("[");
		for(unsigned j = 0; j < VECTOR_SIZE; ++j){
			printf("%6.2f ", t->row[i].f[j]);
			if(j != VECTOR_SIZE - 1){
				printf(", ");
			}
		}

		printf("]");
		if(i != 3){
			printf(",\n");
		}
	}
	printf("]\n");
}
