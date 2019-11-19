#pragma once
class Vector
{
private:
	double coordinateX;
	double coordinateY;
public:
	Vector() {}
	Vector(double x, double y)
		:
		coordinateX(x),
		coordinateY(y)
	{
	}

	void setVector(double x, double y);

	double getX() { return coordinateX; }
	double getY() { return coordinateY; }
};

