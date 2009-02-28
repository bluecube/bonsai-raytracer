#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"

/// Sphere with radius 1 placed in (0, 0, 0).
class Sphere : public Object{
public:
	Sphere() : bbox(Point(-1., -1., -1.), Point(1., 1., 1.)) {};

	LList<Intersection> trace(const Ray *r, Renderer *rend) const;
	bool trace_limited(const Ray *r, Intersection *ret, Renderer *rend, Intersection *out) const;
private:
	double intersections[2];
};

#endif
