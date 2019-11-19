#include "Vector.h"

void Vector::setVector(double x, double y)
{
	X = x;
	Y = y;
}

bool operator!=(Vector vector){
	double eps = 0.01;
	return X-vector.X > eps || Y-vector.Y > eps;
}