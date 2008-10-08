#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

class Transformation{
	Transformation();

	void add_rotation(double x, double y, double z);
	void add_translation(double x, double y, double z);
	void add_scale(double x, double y, double z);
	void add_transformation(const Transformation &t);

	Transformation &operator*=(const Transformation &t){
		add_transformation(t)
	}

	inline double operator[](int i) const{
		return matrix[i];
	}

	void invert();
private:
	double matrix[16];

	void multiply(Transformation a, Transformation b, double *out);
};

#endif
