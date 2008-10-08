#include "BoundingBox.h"

#include <math.h>

bool BoundingBox::has_intersection(Ray *r){
	double t1, t2;

	if((r -> invDirection).x >= 0){
		tMin = (a.x - (r -> origin).x) * (r -> invDirection).x;
		tMax = (b.x - (r -> origin).x) * (r -> invDirection).x;
	}else{
		tMin = (b.x - (r -> origin).x) * (r -> invDirection).x;
		tMax = (a.x - (r -> origin).x) * (r -> invDirection).x;
	}

	if((r -> invDirection).y >= 0){
		t1 = (a.y - (r -> origin).y) * (r -> invDirection).y;
		t2 = (b.y - (r -> origin).y) * (r -> invDirection).y;
	}else{
		t1 = (b.y - (r -> origin).y) * (r -> invDirection).y;
		t2 = (a.y - (r -> origin).y) * (r -> invDirection).y;
	}

	if(t1 < tMax || t2 > tMin)
		return false;
	
	if(t1 > tMin)
		tMin = t1;
	if(t2 < tMax)
		tMax = t2;
	
	if((r -> invDirection).z >= 0){
		t1 = (a.z - (r -> origin).z) * (r -> invDirection).z;
		t2 = (b.z - (r -> origin).z) * (r -> invDirection).z;
	}else{
		t1 = (b.z - (r -> origin).z) * (r -> invDirection).z;
		t2 = (a.z - (r -> origin).z) * (r -> invDirection).z;
	}

	if(t1 < tMax || t2 > tMin)
		return false;
	
	if(t1 > tMin)
		tMin = t1;
	if(t2 < tMax)
		tMax = t2;

	return tMin <= tMax;
}

/// Enlarges the bounding box to contain the box inner.
/// @arg inner Bounding boxto fit into this box.
void BoundingBox::enlarge(BoundingBox *inner){
	if(a.x > (inner -> a).x)
		a.x = (inner -> a).x;
	if(a.y > (inner -> a).y)
		a.y = (inner -> a).y;
	if(a.z > (inner -> a).z)
		a.z = (inner -> a).z;
	
	if(b.x < (inner -> b).x)
		b.x = (inner -> b).x;
	if(b.y < (inner -> b).y)
		b.y = (inner -> b).y;
	if(b.z < (inner -> b).z)
		b.z = (inner -> b).z;
}


/// Returns the surface area of the box.
double BoundingBox::surface(){
	Vector size(b);
	size -= a;

	if(size.x < 0) size.x = 0;
	if(size.y < 0) size.y = 0;
	if(size.z < 0) size.z = 0;

	return 2 * (size.x * size.y + size.y * size.z + size.x * size.z);
}
