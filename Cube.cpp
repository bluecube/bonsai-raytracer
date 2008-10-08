#include "Cube.h"

Cube() : bbox(Point(-1., -1., -1.), Point(1., 1., 1.)) {}

Intersection *Cube::trace(const Ray *r, Renderer *rend) const{
	Intersection *tmp = rend -> new_intersection(this, r, bbox.tMin);
	tmp -> next = rend -> new_intersection(this, r, bbox.tMax);
	
	return tmp;
}

Intersection *Cube::trace_limited(const Ray *r, Renderer *rend) const{
	return rend -> new_ntersection(this, r, bbox.tMin);
}

Surface *Cube::get_surface(const Point *p, Ray *r) const;
