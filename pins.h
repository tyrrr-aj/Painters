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
#define RIGHT_MOTOR_IN1 16
#define RIGHT_MOTOR_IN2 17

#define LEFT_MOTOR_IN1 4
#define LEFT_MOTOR_IN2 2

#define RIGHT_MOTOR_PWM 25
#define LEFT_MOTOR_PWM 26

// encoder pins
#define LEFT_ENC 32
#define RIGHT_ENC 33

#endif
