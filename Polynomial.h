#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

class Polynomial{
public:
	Polynomial(int degree_ = 0);
	Polynomial(double &constant);
	Polynomial(Polynomial &p);

	void setSize(int degree_)

	double operator()(double x){
		return evaluate(x);
	}

	double evaluate(double x);

	void derivate();

	void solve(); //!!!!

	int degree();

	/// Return a coefficient of x**i.
	/// Doesn't check boudns.
	double &operator[](int i){
		return coefs[i];
	}

	Polynomial &operator+=(Polynomial &p);
	Polynomial &operator-=(Polynomial &p);
	Polynomial &operator+=(double &d){
		coefs[0] += d;
		return *this;
	}
	Polynomial &operator-=(Polynomial &p){
		coefs[0] -= d;
		return *this;
	}
	Polynomial &operator-();
	Polynomial &operator*=(Polynomial &p);
	Polynomial &operator/=(Polynomial &p);
	Polynomial &operator*=(double d){
		for(int i = 0; i < n; ++i)
			coefs[i] *= d;

		return *this;
	}
	Polynomial &operator/=(double d){
		return operator*=(1 / d);
	}

	friend Polynomial &operator+(Polynomial &a, Polynomial &b);
	friend Polynomial &operator-(Polynomial &a, Polynomial &b);
	friend Polynomial &operator*(Polynomial &a, Polynomial &b);
	friend Polynomial &operator+(Polynomial &a, double b);
	friend Polynomial &operator-(Polynomial &a, double b);
	friend Polynomial &operator*(Polynomial &a, double b);
	friend Polynomial &operator/(Polynomial &a, double b);
private:
	double *coefs;
	int n;
};


inline Polynomial &operator+(Polynomial &a, Polynomial &b){
	Polynomial tmp(a);
	tmp += b;
	return tmp;
}

inline Polynomial &operator-(Polynomial &a, Polynomial &b){
	Polynomial tmp(a);
	tmp -= b;
	return tmp;
}

inline Polynomial &operator+(Polynomial &a, double b){
	Polynomial tmp(a);
	tmp += b;
	return tmp;
}

inline Polynomial &operator-(Polynomial &a, double b){
	Polynomial tmp(a);
	tmp -= b;
	return tmp;
}

inline Polynomial &operator*(Polynomial &a, Polynomial &b){
	Polynomial tmp(a);
	tmp *= b;
	return tmp;
}

inline Polynomial &operator*(Polynomial &a, double b){
	Polynomial tmp(a);
	tmp *= b;
	return tmp;
}

inline Polynomial &operator/(Polynomial &a, double b){
	Polynomial tmp(a);
	tmp /= b;
	return tmp;
}

#endif
