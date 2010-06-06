#include "random.h"

#include <stdlib.h>

/**
 * Get random float from interval <#from, #to>.
 */
float random_number(float from, float to){
	return random() * (from - to) / RAND_MAX + from;
}
