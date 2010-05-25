#include "random.h"

#include <stdlib.h>

/**
 * Get random float from interval <#from, #to>.
 */
float random_float(float from, float to){
	return random() * (from - to) / RAND_MAX + from;
}
