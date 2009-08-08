#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Vector3D.h"
#include "Ray.h"
#include "Transformation.h"

#include <math.h>

class Cube;
class Container;

class BoundingBox{
	BoundingBox() : a(INFINITY, INFINITY, INFINITY), b(-INFINITY, -INFINITY, -INFINITY) {}
	
	/// Left, top and close corner of the box.
	/// (a.x < b.x && a.y < b.y && a.z < b.z if a and b are dehomogenized).
	Point3D a;

	/// Right, bottom and far corner of the box.
	/// (a.x < b.x && a.y < b.y && a.z < b.z if a and b are dehomogenized).
	Point3D b;

	bool has_intersection(Ray *r);

	void enlarge(BoundingBox *inner);

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
