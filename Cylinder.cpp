#include "Cylinder.h"

LList<Intersection> Cylinder::trace(const Ray *r, Renderer *rend) const{
	LList<Intersection> ret;
	if(r.invDirection.z >= 0){
		intersection[0] = (-1. - r.origin.z) * r.invDirection.z;
		intersection[1] = ( 1. - r.origin.z) * r.invDirection.z;
	}else{
		intersection[0] = ( 1. - r.origin.z) * r.invDirection.z;
		intersection[1] = (-1. - r.origin.z) * r.invDirection.z;
	}

	double dotproduct = r.direction.x * r.origin.x + r.direction.z * r.origin.z;
	double D = dotproduct * dotproduct - r.origin.x * r.origin.x - r.origin.z * r.origin.z + 1;

	if(d < 0)
		return ret;
	else{
		D = sqrt(D);
		double t;
		t = -dotproduct - D;
		if(t > intersection[0])
			intersection[0] = t;

		t = -dotproduct + D;
		if(t < intersection[1])
			intersection[1] = t;

		if(intersection[0] <= intersection[1])
			return IntersectionList(intersection, 2);
		else
			return IntersectionList(0, 0);

		return ret;
	}
}
