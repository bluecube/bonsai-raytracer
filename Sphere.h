#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"

/// Sphere with radius 1 placed in (0, 0, 0).
class Sphere : public Object{
public:
	Sphere() : bbox(Point(-1., -1., -1.), Point(1., 1., 1.)) {};

	Intersection *trace(const Ray *r, Renderer *rend) const;
	Intersection *trace_limited(const Ray *r, Renderer *rend) const;
private:
	Surface *get_surface(const Point *p, Ray *r) const;
	double intersections[2];
};

#endif
