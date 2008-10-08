#include "math.h"
#include "Sphere.h"

Intersection *Sphere::trace(const Ray *r, Renderer *rend) const{
	double dotproduct = ray.direction * ray.origin;
	double D = dotproduct * dotproduct - abspow2(ray.origin) + 1;

	if(d < 0)
		return 0;
	else{
		D = sqrt(D);
		
		Intersection *tmp = rend -> new_intersection(this, r, - dotproduct - D);
		tmp -> next = rend -> new_intersection(this, r, - dotproduct + D);

		return tmp;

	}
}

Intersection *Sphere::trace_limited(const Ray *r, Renderer *rend) const{
	double dotproduct = ray.direction * ray.origin;
	double D = dotproduct * dotproduct - abspow2(ray.origin) + 1;

	if(d < 0)
		return 0;
	else{
		D = sqrt(D);
		
		return rend -> new_intersection(this, r, - dotproduct - D);

	}
}

Surface &Sphere::get_surface(const Point &p) const;
