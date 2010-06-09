/**
 * \file
 * Operations with colors.
 */

#ifndef BONSAI_CLIENT_COLOR_H
#define BONSAI_CLIENT_COLOR_H

struct color{
	float r;
	float g;
	float b;
};

void color_black(struct color *c);
void color_add(struct color *c, struct color *c1);
void color_scale(struct color *c, float f);

#endif
