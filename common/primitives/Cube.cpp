#include "Cube.h"

Cube() : bbox(Point(-1., -1., -1.), Point(1., 1., 1.)) {}

LList<Intersection> trace(const Ray *r, Renderer *rend) const = 0;
	LList<intersection> ret;
	ret.append_single(rend->new_intersection(this, r, bbox.tMin));
	ret.append_single(rend->new_intersection(this, r, bbox.tMax));
	
	return tmp;
}

bool Cube::trace_limited(const Ray *r, Intersection *ret, Renderer *rend, Intersection * out) const{
	out->edit(this, r, bbox.tMin);
	return true;
}

Surface *Cube::get_surface(const Point *p, Ray *r) const;
