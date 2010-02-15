#include "Polynomial.h"

Polynomial::Polynomial(int degree){
	n = degree + 1;
	coefs = new double[n];
}

Polynomial::Polynomial(double constant){
	n = 1;
	coefs = new double[1];

	coefs[0] = constant;
}

/// Evaluate the polynomial in point x using Hohner's rule.
double Polynomial::evaluate(double x){
	double value = 0;

	for(int i = n - 1; i >= 0; --i){
		value = value * x + coefs[i];
	}

	return value;
}

void Polynomial::derivate(){
	for(int i = 1; i < n; ++i)
		coefs[i - 1] = coefs[i] * i;
	
	coefs[n - 1] = 0;
}

void Polynomial::solve(){

}

int Polynomial::degree(){
	for(int i = n - 1; i >= 0; --i)
		if(coefs[i] != 0)
			return i;
	
	return -1;
}

Polynomial &Polynomial::operator+=(Polynomial &p){
	setSize(n > p.n ? n : p.n - 1);

	for(int i = 0; i < n; ++i)
		coefs[i] += p[i];
	
	return *this;
}

Polynomial &Polynomial::operator-=(Polynomial &p){
	setSize(n > p.n ? n : p.n);

	for(int i = 0; i < n; ++i)
		coefs[i] -= p[i];
	
	return *this;
	
}

Polynomial &Polynomial::operator-(){
	for(int i = 0; i < n; ++i)
		coefs[i] = -coefs[i];
}

Polynomial &Polynomial::operator*=(Polynomial &p){
	double *tmp = new double[n + p.n - 1];

	for(int i = 0; i < n; ++i)
		for(int j = 0; j < p.n; ++j){
			tmp[i + j] = coefs[i] * p.coefs[j];
		}
	
	n += p.n - 1;
	delete[] coefs;
	coefs = tmp;
}
