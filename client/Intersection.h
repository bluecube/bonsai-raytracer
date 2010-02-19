#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Object.h"
#include "Ray.h"

/// A class representing a intersection of a ray with the object.
/// Intersections are allocated in a pool contained in a renderer.
/// To avoid overloaded new and delete the Renderer::new_intersection()
/// is used.
class Intersection{
public:
	/// Create the intersection with set parameters.
	/// \see edit().
	Intersection(Object *obj, Ray *ray, double dist);

	/// Edit the intersection properties.
	/// \arg obj Object that was hit -- it may be later changed if the
	/// object is in a container that overides the surface.
	/// \arg r Ray that this intersection is on. Transformed to the
	/// obj's space.
	/// \arg dist Distance from the origin of the ray (in obj's space).
	void edit(Object *obj, Ray *ray, double dist);

	Surface &get_surface(){
		return o -> get_surface(p, r);
	}

	double get_distance(){
		return distance;
	}

	Intersection *next;

	/// Object that owns the surface.
	Object *o;

	/// This is the coordinate in the o's space.
	Point3D coord;

	/// Distance along the ray. It's first filled in the o's space and then is
	/// transformed back to the camera's space.
	double d;
};

#endif
