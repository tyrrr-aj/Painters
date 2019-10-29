/*********************************************************************
This file binds abstract pins' names with physical pin numbers. This 
bindings are application-specific and depend throughfully on electrical
design of your robot; remember to adjust them and always keep them up to 
date.

29.10.2019- Adam Szreter, AGH University of Science and Technology
Code developed in Arduino 1.8.9, on ESP32 DevkitC v4
*********************************************************************/
#ifndef PINS_H
#define PINS_H

// motor pins
extern const int RIGHT_MOTOR_IN1 = 17;
extern const int RIGHT_MOTOR_IN2 = 16;

extern const int LEFT_MOTOR_IN1 = 2;
extern const int LEFT_MOTOR_IN2 = 4;

extern const int RIGHT_MOTOR_PWM = 25;
extern const int LEFT_MOTOR_PWM = 26;

#endif