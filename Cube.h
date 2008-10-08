#ifndef CUBE_H
#define CUBE_H

#include "Object.h"

/// Axis aligned cube (-1, -1, -1) - (1, 1, 1).
class Cube : public Object{
public:
	Cube();

	virtual Intersection *trace(const Ray *r, Renderer *rend) const = 0;
	virtual Intersection *trace_limited(const Ray *r, Renderer *rend) const = 0;

private:
	virtual Surface *get_surface(const Point *p, const Ray *r) const = 0;
};

#endif
