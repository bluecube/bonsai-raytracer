#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Vector.h"
#include "Ray.h"
#include "Transformation.h"

class Cube;
class Container;

class BoundingBox{
	BoundingBox() : a(INF, INF, INF), b(-INF, -INF, -INF) {}
	
	/// Left, top and close corner of the box.
	/// (a.x < b.x && a.y < b.y && a.z < b.z if a and b are dehomogenized).
	Point a;

	/// Right, bottom and far corner of the box.
	/// (a.x < b.x && a.y < b.y && a.z < b.z if a and b are dehomogenized).
	Point b;

	bool has_intersection(Ray &r);

	void enlarge(BoundingBox &inner);

	double surface();
	
	void transform(Transformation *t);
private:
	/// Stores the first intersection point from the last call to intersects().
	/// Only valid if intersects() returned true.
	double tMin, tMax;

	friend class Cube;
	friend class Container;
};

#endif
