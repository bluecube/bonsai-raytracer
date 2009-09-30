#ifndef CYLINDER_H
#define CYLINDER_H

#include "Object.h"

/// Cylinder with radius 1 and height 2, center is placed in (0, 0, 0).
class Cylinder : public Object{
public:
	Cylinder() : bbox(Point(-1., -1., -1.), Point(1., 1., 1.));

	LList<Intersection> trace(const Ray *r, Renderer *rend) const;
	bool trace_limited(const Ray *r, Intersection *ret, Renderer *rend, Intersection *out) const;
private:
	double intersections[2];
};

#endif
