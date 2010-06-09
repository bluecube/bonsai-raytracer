#include "color.h"

/**
 * Set all channels to 0.
 */
void color_black(struct color *c){
	c->r = 0;
	c->g = 0;
	c->b = 0;
}

/**
 * Calculate c += c1 .
 */
void color_add(struct color *c, struct color *c1){
	c->r += c1->r;
	c->g += c1->g;
	c->b += c1->b;
}

/**
 * Calculate c *= f.
 */
void color_scale(struct color *c, float f){
	c->r *= f;
	c->g *= f;
	c->b *= f;
}
