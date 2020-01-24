#include "Vector.h"

Vector::Vector()
{
	X = 0;
	Y = 0;
}

Vector::Vector(Point point) {
	X = point.X;
	Y = point.Y;
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

double Vector::scalar_product(Vector other) {
	return X * other.X + Y * other.Y;
}

double Vector::distance(Vector other) {
	return (X - other.X) * (X - other.X) + (Y - other.Y) * (Y - other.Y);
}

bool Vector::operator!=(Vector otherVector)
{
	double eps = 0.1;
	if(fabs(this->X - otherVector.X) > eps || fabs(this->Y - otherVector.Y) > eps)
		return true;
	else
		return false;
}

Vector Vector::operator+(Vector other) const {
	Vector add(this->X + other.X, this->Y + other.Y);
	return add;
}

Vector Vector::operator-(Vector other) const {
	Vector subtract(other.X - this->X, other.Y - this->Y);
	return subtract;
}

Vector Vector::operator*(double scalar) const {
	return Vector(this->X * scalar, this->Y * scalar);
}