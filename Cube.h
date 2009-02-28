#ifndef CUBE_H
#define CUBE_H

#include "Object.h"

/// Axis aligned cube (-1, -1, -1) - (1, 1, 1).
class Cube : public Object{
public:
	Cube();

	LList<Intersection> trace(const Ray *r, Renderer *rend) const;
	bool trace_limited(const Ray *r, Intersection *ret, Renderer *rend, Intersection * out) const;
};

#endif
