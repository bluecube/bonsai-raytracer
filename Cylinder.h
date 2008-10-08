#ifndef CYLINDER_H
#define CYLINDER_H

#include "Object.h"

/// Cylinder with radius 1 and height 2, center is placed in (0, 0, 0).
class Cylinder : public Object{
public:
	Cylinder() : bbox(Point(-1., -1., -1.), Point(1., 1., 1.));

	IntersectionList trace(const Ray &r) const;
	Surface &get_surface(const Point &p) const;
private:
	double intersections[2];
};

#endif
