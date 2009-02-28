#include "math.h"
#include "Sphere.h"

LList<Intersection> Sphere::trace(const Ray *r, Renderer *rend) const{
	double dotproduct = ray.direction * ray.origin;
	double d = dotproduct * dotproduct - abspow2(ray.origin) + 1;

	LList<Intersection> ret;

	if(d > 0)
		d = sqrt(d);
		
		ret.append_single(rend -> new_intersection(this, r, - dotproduct - d));
		ret.append_single(rend -> new_intersection(this, r, - dotproduct + d));
	}

	return ret;
}

bool Sphere::trace_limited(const Ray *r, Renderer *rend, Intersection *out) const{
	double dotproduct = ray.direction * ray.origin;
	double d = dotproduct * dotproduct - abspow2(ray.origin) + 1;

	if(d > 0)
		d = sqrt(d);

		double tmp = dotproduct - d;
		
		if(tmp < 0){
			tmp += 2 * d;
			if(tmp < 0)
				return INFINITY;
		}

		return tmp;
	}else
		return INFINITY;

	return ret;
}

Surface &Sphere::get_surface(const Point &p) const;
