#pragma once
class Vector
{

public:
	double X;
	double Y;
	
	Vector() {}
	Vector(double x, double y)
		:
		X(x),
		Y(y)
	{
	}

	void setVector(double x, double y);

	double getX() { return X; }
	double getY() { return Y; }
	
	bool operator!=(Vector vector);
};

