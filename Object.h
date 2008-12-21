#ifndef OBJECT_H
#define OBJECT_H

#include "Surface.h"
#include "BoundingBox.h"
#include "Ray.h"
#include "Vector3D.h"
#include "LList.h"

class Renderer;
class Intersection;

/// Virtual class representing an object that can be displayed.
class Object{
public:
	/// Finds all intersections between the ray and the object.
	virtual LList<Intersection> trace(const Ray *r, Renderer *rend) const = 0;

	/// Finds the first positive intersection between the ray and the object.
	/// \return Distance of first positive intersection, or INFINITY there isn't any.
	virtual double trace_limited(const Ray *r, Renderer *rend) const = 0;

	/// Find out if the point is inside the object
	virtual bool is_inside(const Point3D *p) const = 0;

	/// Returns the bounding box of the object.
	/// The bounding box of the object _must_ be checked before calling
	/// the trace() or trace_limited() methods!
	inline const BoundingBox *get_bounding_box(){
		return &bbox;
	}

	/// Recursively rebuild the trees in containers.
	/// Bounding box and number of contained items
	/// only changes when when rebuild() is called.
	/// Has no meaning for non-container objects.
	virtual void rebuild(){}
protected:
	BoundingBox bbox;
	virtual Surface *get_surface(const Point3D *p, const Ray *r) const = 0;

	friend class Intersection;
};

#endif
