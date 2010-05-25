#include "transform.h"

/**
 * Transform vector #v to #ret using transformation #t.
 *
 * ret = t * v
 */
void transform_vector(transform t, vector v, vector ret){
	for(int i = 0; i < 3; ++i){
		ret[i] = t[3 + 4 * i];
		for(int j = 0; j < 3; ++j){
			ret[i] += v[j] * t[j + 4 * i];
		}
	}
}

