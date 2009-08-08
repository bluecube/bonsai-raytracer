#include "Transformation.h"

void Transformation::multiply(Transformation a, Transformation b, double *out){
	for(int i = 0; i < 16; ++i){
		int row = i & ~3;
		int col = i & 3;

		out[i] = 0;

		for(int j = 0; j < 4; ++j)
			out[i] += a.matrix[col + 4 * j] * b.matrix[j + row];
	}
}

void Transform::invert(){
	
	
}
