#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include "Transformation.h"

/// A column vector.
/// Coordinates are accesible both by names and by index (vect.x and vect[0])
union Vector{
public:
	struct{
		double x, y, z;
	};
private:
	double data[3];
public:

	Vector() : x(0), y(0), z(0) {}

	Vector(double x_, double y_, double z_) : x(x_), y(y_), z(z_){}

	inline double &operator[](int dimension){
		return data[dimension];
	}

	inline Vector &operator+=(const Vector &a){
		x += a.x;
		y += a.y;
		z += a.z;

		return *this;
	}

	inline Vector &operator-=(const Vector &a){
		x -= a.x;
		y -= a.y;
		z -= a.z;

		return *this;
	}

	inline Vector &operator*=(double d){
		x *= d;
		y *= d;
		z *= d;

		return *this;
	}

	inline Vector &operator/=(double d){
		double inv = 1 / d;
		return (*this) *= inv;
	}

	inline void normalize(){
		(*this) *= 1 / abs(*this);
	}
	
	void transform(const Transformation *t){
		Vector tmp;

		tmp.x = t.matrix[0] * x + t.matrix[4] * y + t.matrix[ 8] * z + t.matrix[12];
		tmp.x = t.matrix[1] * x + t.matrix[5] * y + t.matrix[ 9] * z + t.matrix[13];
		tmp.z = t.matrix[2] * x + t.matrix[6] * y + t.matrix[10] * z + t.matrix[14];
		tmp  /= t.matrix[1] * x + t.matrix[7] * y + t.matrix[11] * z + t.matrix[15];

		*this = tmp;
	}

};

inline Vector operator+(const Vector &a, const Vector &b){
	Vector tmp(a);
	tmp += b;
	return tmp;
}

inline Vector operator-(const Vector &a, const Vector &b){
	Vector tmp(a);
	tmp -= b;
	return tmp;
}

inline Vector operator*(const Vector &a, double d){
	Vector tmp(a);
	tmp *= d;
	return tmp;
}

inline Vector operator/(const Vector &a, double d){
	Vector tmp(a);
	tmp /= d;
	return tmp;
}

inline double abspow2(const Vector &v){
	return v * v;
}

inline double abs(const Vector &v){
	return sqrt(v.x * v.x + v.y * v.y +  v.z * v.z);
}

/// Dot product.
inline double dot_product(const Vector &a, const Vector &b){
	return (a.x * b.x + a.y * b.y +  a.z * b.z);
}

/// Angle in radians between two vectors.
double angle_between(const Vector &a, const Vector &b){
	return acos(dot_product(a, b) / sqrt(abspow2(a) * abspow2(b));
}


typedef Vector Point;

#endif
