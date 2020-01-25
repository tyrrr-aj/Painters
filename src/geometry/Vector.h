#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

#include "Point.h"

class Point;

class Vector
{
public:
	double X;
	double Y;
	
	Vector();
	Vector(Point);
	Vector(double,double);
	
	double length();
	Vector getNormalVector();
	
	double distance(Vector other);
	double scalar_product(Vector other);
	
	bool operator!=(Vector otherVector);
	Vector operator+(Vector other) const;
	Vector operator-(Vector other) const;
	Vector operator*(double scalar) const;
};

#endif