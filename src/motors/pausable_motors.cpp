#include "pausable_motors.h"

PausableMotors::PausableMotors() {
    motorsControlSemaphore = xSemaphoreCreateBinary();
}

void PausableMotors::drive(int speed) {
    xSemaphoreTake(motorsControlSemaphore, portMAX_DELAY);
	currentLeftMotorSpeed = speed;
    currentRightMotorSpeed = speed;
    Motors::drive(speed);
    xSemaphoreGive(motorsControlSemaphore);
}

void PausableMotors::coast() {
    xSemaphoreTake(motorsControlSemaphore, portMAX_DELAY);
	currentLeftMotorSpeed = 0;
    currentRightMotorSpeed = 0;
    Motors::coast();
    xSemaphoreGive(motorsControlSemaphore);
}

void PausableMotors::stop() {
    xSemaphoreTake(motorsControlSemaphore, portMAX_DELAY);
	currentLeftMotorSpeed = 0;
    currentRightMotorSpeed = 0;
    Motors::stop();
    xSemaphoreGive(motorsControlSemaphore);
}

void PausableMotors::rightMotor(int speed) {
    xSemaphoreTake(motorsControlSemaphore, portMAX_DELAY);
	currentRightMotorSpeed = speed;
    Motors::rightMotor(speed);
    xSemaphoreGive(motorsControlSemaphore);
}

void PausableMotors::leftMotor(int speed) {
    xSemaphoreTake(motorsControlSemaphore, portMAX_DELAY);
	currentLeftMotorSpeed = speed;
    Motors::leftMotor(speed);
    xSemaphoreGive(motorsControlSemaphore);
}

void PausableMotors::pause() {
    xSemaphoreTake(motorsControlSemaphore, portMAX_DELAY);
    Motors::stop();
}

void PausableMotors::resume() {
    Motors::rightMotor(currentRightMotorSpeed);
    Motors::leftMotor(currentLeftMotorSpeed);
    xSemaphoreGive(motorsControlSemaphore);
}

void PausableMotors::freeze() {
    xSemaphoreTake(motorsControlSemaphore, portMAX_DELAY);
}

void PausableMotors::unfreeze() {
    xSemaphoreGive(motorsControlSemaphore);
}