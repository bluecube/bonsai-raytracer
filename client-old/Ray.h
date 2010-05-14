#ifndef RAY_H
#define RAY_H

#include "Vector3D.h"

/// Stores a ray with origin and direction.
/// The direction is kept normalized. If the
/// direction vector is changed manually you should call fixIt(). 
class Ray{
public:
	Ray() : origin(), direction() {}
	Ray(Point3D &o, Vector3D &d) : origin(o), direction(d) {
		fix_it();
	}

	Point3D origin;
	Vector3D direction;
	Vector3D invDirection;

	/// Normalize the direction vector and
	/// computes the inverse direction vector.
	/// \return The lenght of the direction vector before normalization.
	double fix_it(){
		double ret = abs(direction);
		direction *= ret;
		invDirection.x = 1 / direction.x;
		invDirection.y = 1 / direction.y;
		invDirection.z = 1 / direction.z;

		return ret;
	}

	/// Transforms a ray.
	/// \return The lenght of the transformed ray before normalization. This is useful
	/// for transforming intersections back from the object space to the container space.
	double transform(Transformation *t){
		origin.transform(t);
		direction.transform(t);
		/*
		tmp.x = t.matrix[0] * x + t.matrix[4] * y + t.matrix[ 8] * z + t.matrix[12];
		tmp.x = t.matrix[1] * x + t.matrix[5] * y + t.matrix[ 9] * z + t.matrix[13];
		tmp.z = t.matrix[2] * x + t.matrix[6] * y + t.matrix[10] * z + t.matrix[14];
		tmp  /= t.matrix[1] * x + t.matrix[7] * y + t.matrix[11] * z + t.matrix[15];
		*/
		return fix_it();
	}

	Ray &operator+=(Vector3D &v){
		origin += v;
		return *this;
	}

	Ray &operator-=(Vector3D &v){
		origin -= v;
		return *this;
	}
};

#endif
