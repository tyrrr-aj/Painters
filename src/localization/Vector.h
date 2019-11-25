#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

class Vector
{
public:
	double X;
	double Y;
	
	Vector();
	Vector(double,double);
	
	double length();
	Vector getNormalVector();
	
	bool operator!=(Vector otherVector);
};

#endif