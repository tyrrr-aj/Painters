#include "pausable_motors.h"

PausableMotors::PausableMotors() {
    isPaused = false;
}

void PausableMotors::drive(int speed) {
	currentLeftMotorSpeed = speed;
    currentRightMotorSpeed = speed;
    if (!isPaused) {
        Motors::drive(speed);
    }
}

void PausableMotors::coast() {
	currentLeftMotorSpeed = 0;
    currentRightMotorSpeed = 0;
    if (!isPaused) {
        Motors::coast();
    }
}

void PausableMotors::stop() {
	currentLeftMotorSpeed = 0;
    currentRightMotorSpeed = 0;
    if (!isPaused) {
        Motors::stop();
    }
}

void PausableMotors::rightMotor(int speed) {
	currentRightMotorSpeed = speed;
    if (!isPaused) {
        Motors::rightMotor(speed);
    }
}

void PausableMotors::leftMotor(int speed) {
	currentLeftMotorSpeed = speed;
    if (!isPaused) {
        Motors::leftMotor(speed);
    }
}

void PausableMotors::pause() {
    isPaused = true;
    Motors::stop();
}

void PausableMotors::resume() {
    isPaused = false;
    Motors::rightMotor(currentRightMotorSpeed);
    Motors::leftMotor(currentLeftMotorSpeed);
}

void PausableMotors::freeze() {
    isPaused = true;
}

void PausableMotors::unfreeze() {
    isPaused = false;
}