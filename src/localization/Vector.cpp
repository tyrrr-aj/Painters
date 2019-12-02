#include "Vector.h"

Vector::Vector()
{
	X = 0;
	Y = 0;
}

Vector::Vector(double X, double Y)
{
	this->X = X;
	this->Y = Y;
}

double Vector::length()
{
	return sqrt(X*X + Y*Y);
}

Vector Vector::getNormalVector()
{
	return Vector(this->X / length() , this->Y / length());
}

bool Vector::operator!=(Vector otherVector)
{
	double eps = 0.01;
	if(fabs(this->X - otherVector.X) > eps || fabs(this->Y - otherVector.Y) > eps)
		return true;
	else
		return false;
}