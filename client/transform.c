#include "transform.h"

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
 * No bound checks are done.
 */
static float det_2x2(const struct transform *t, int row1, int row2, int col1, int col2){
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
	
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 3; ++j){
			// swapped rows and columns!
			int row1 = (j == 0) ? 1 : 0;
			int row2 = (j == 2) ? 1 : 2;
			int col1 = (i == 0) ? 1 : 0;
			int col2 = (i == 2) ? 1 : 2;
		
			ret->row[i].f[j] = det_2x2(t, row1, row2, col1, col2) / det;
		}
	}
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
