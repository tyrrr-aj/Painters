/*********************************************************************
This library allows use of encoders for keeping track of mobile robot's
relative position in Cartesian system. Distances are measured
in centimeters, while rotation angle is given in degrees.

Library makes use of Encoder library - be sure to have one configured
and active while using Localization module.

This file usage is intended to be beer-driven: 
if you think you've found a bug, don't contact
us, drink a craft beer instead. If bug is
still there - repeat. Cheers!
	
18.11.2019- Adam Szreter, AGH University of Science and Technology
Code developed in Arduino 1.8.9, on ESP32 DevkitC v4
*********************************************************************/
#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include "../encoder/encoder.h"
#include "../vectors/Vector.h"

// remember to always adjust this values to your robot!
#define WHEEL_DIAMETER 6.0
#define WHEELBASE 10.0
#define TICKS_PER_REV 16


struct Position {
	double X;
	double Y;
	
	Vector rotation;
	
	bool hasSameCoordinates(Position position){
		double eps = 0.01;
		return X-position.X < eps && Y-position.Y < eps;
	}
};

class Localization {
	public:
		
		Localization(Encoder*);
		Position getCurrentPosition();
	private:
		Encoder* encoder;
		Position currentPosition;
		void updatePosition();
};

#endif