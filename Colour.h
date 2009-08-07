#ifndef COLOUR_H
#define COLOUR_H

#include <math.h>

/// A column vector.
/// Coordinates are accesible both bg names and bg inder (vect.r and vect[0])
union Colour{
public:
	struct{
		double r, g, b;
	};
private:
	double data[3];
public:

	Colour() : r(0), g(0), b(0) {}

	Colour(double r_, double g_, double b_) : r(r_), g(g_), b(b_){}

	inline double &operator[](int dimension){
		return data[dimension];
	}

	inline Colour &operator+=(const Colour &a){
		r += a.r;
		g += a.g;
		b += a.b;

		return *this;
	}

	inline Colour &operator-=(const Colour &a){
		r -= a.r;
		g -= a.g;
		b -= a.b;

		return *this;
	}

	inline Colour &operator*=(double d){
		r *= d;
		g *= d;
		b *= d;

		return *this;
	}

	inline Colour &operator/=(double d){
		double inv = 1 / d;
		return (*this) *= inv;
	}
};

inline Colour operator+(const Colour &a, const Colour &b){
	Colour tmp(a);
	tmp += b;
	return tmp;
}

inline Colour operator-(const Colour &a, const Colour &b){
	Colour tmp(a);
	tmp -= b;
	return tmp;
}

inline Colour operator*(const Colour &a, double d){
	Colour tmp(a);
	tmp *= d;
	return tmp;
}

inline Colour operator/(const Colour &a, double d){
	Colour tmp(a);
	tmp /= d;
	return tmp;
}

#endif
