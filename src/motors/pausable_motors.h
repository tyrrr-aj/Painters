#ifndef PAUSABLE_MOTORS_H
#define PAUSABLE_MOTORS_H

#include "motors.h"
#include "freertos/semphr.h"

class PausableMotors: public Motors {
    public:
        PausableMotors();
        void drive(int speed);
		void coast();
		void stop();
		void rightMotor(int speed);
		void leftMotor(int speed);

        void pause();
        void resume();
        void freeze();
        void unfreeze();

    private:
        int currentRightMotorSpeed;
        int currentLeftMotorSpeed;
        SemaphoreHandle_t motorsControlSemaphore;
};

#endif