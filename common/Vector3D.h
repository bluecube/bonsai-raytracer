#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include "Transformation.h"

/// A column vector.
/// Coordinates are accesible both by names and by index (vect.x and vect[0])
union Vector3D{
public:
	struct{
		double x, y, z;
	};
private:
	double data[3];
public:

	Vector3D() : x(0), y(0), z(0) {}

	Vector3D(double x_, double y_, double z_) : x(x_), y(y_), z(z_){}

	inline double &operator[](int dimension){
		return data[dimension];
	}

	inline Vector3D &operator+=(const Vector3D &a){
		x += a.x;
		y += a.y;
		z += a.z;

		return *this;
	}

	inline Vector3D &operator-=(const Vector3D &a){
		x -= a.x;
		y -= a.y;
		z -= a.z;

		return *this;
	}

	inline Vector3D &operator*=(double d){
		x *= d;
		y *= d;
		z *= d;

		return *this;
	}

	inline Vector3D &operator/=(double d){
		double inv = 1 / d;
		return (*this) *= inv;
	}

	inline double abs() const{
		return sqrt(abs2());
	}

	/// Returns absolute value squared.
	inline double abs2() const{
		return x * x + y * y + z * z;
	}

	inline void normalize(){
		(*this) *= 1 / abs();
	}
	
	void transform(const Transformation *t){
		Vector3D tmp;

		tmp.x = t -> matrix[0] * x + t -> matrix[4] * y + t -> matrix[ 8] * z + t -> matrix[12];
		tmp.x = t -> matrix[1] * x + t -> matrix[5] * y + t -> matrix[ 9] * z + t -> matrix[13];
		tmp.z = t -> matrix[2] * x + t -> matrix[6] * y + t -> matrix[10] * z + t -> matrix[14];
		tmp  /= t -> matrix[1] * x + t -> matrix[7] * y + t -> matrix[11] * z + t -> matrix[15];

		*this = tmp;
	}
};

inline Vector3D operator+(const Vector3D &a, const Vector3D &b){
	Vector3D tmp(a);
	tmp += b;
	return tmp;
}

inline Vector3D operator-(const Vector3D &a, const Vector3D &b){
	Vector3D tmp(a);
	tmp -= b;
	return tmp;
}

inline Vector3D operator*(const Vector3D &a, double d){
	Vector3D tmp(a);
	tmp *= d;
	return tmp;
}

inline Vector3D operator/(const Vector3D &a, double d){
	Vector3D tmp(a);
	tmp /= d;
	return tmp;
}

inline double abs2(const Vector3D &v){
	return v.abs2();
}

inline double abs(const Vector3D &v){
	return v.abs();
}

/// Dot product.
inline double dot_product(const Vector3D &a, const Vector3D &b){
	return (a.x * b.x + a.y * b.y +  a.z * b.z);
}

/// Angle in radians between two vectors.
double angle_between(const Vector3D &a, const Vector3D &b){
	return acos(dot_product(a, b) / sqrt(abs2(a) * abs2(b)));
}


typedef Vector3D Point3D;

#endif
