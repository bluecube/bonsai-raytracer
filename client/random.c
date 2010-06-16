#include "random.h"

#include <stdlib.h>

/**
 * Get random float from interval <#from, #to>.
 */
float random_number(float from, float to){
	return random() * (from - to) / RAND_MAX + from;
}

/**
 * Get a random integer from interval <#from, #to>.
 */
int random_int(int from, int to){
	return random() * (from - to) / RAND_MAX + from;
}
