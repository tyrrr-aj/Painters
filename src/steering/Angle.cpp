#include "Angle.h"

Angle::Angle()
{

}

Angle::Angle(Vector desiredTransition, Vector rotation)
{
	this->desiredTransition = desiredTransition;
	this->rotation = rotation;
}


double Angle::getRotation()
{
	double cosinus = rotationCosinus();
	if(rotationDirection() == angle_direction::DIRECTION::LEFT)
		return acos(cosinus);
	else
		return -acos(cosinus);
}

Angle Angle::operator=(Angle angle)
{
	this->desiredTransition = angle.desiredTransition;
	this->rotation = angle.rotation;
}


/*********************************************************************
All methods below are private.
*********************************************************************/


double Angle::rotationCosinus()
{
	double scalarProduct = rotation.X * desiredTransition.X + rotation.Y * desiredTransition.Y;
	return scalarProduct / (desiredTransition.length());		//rotation.length() == 1
}

angle_direction::DIRECTION Angle::rotationDirection()
{
	double vectorProduct = rotation.X * desiredTransition.Y - rotation.Y * desiredTransition.X;
	if(vectorProduct > 0)
		return angle_direction::DIRECTION::LEFT;
	else
		return angle_direction::DIRECTION::RIGHT;
}