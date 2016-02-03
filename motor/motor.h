#ifndef motor_H
#define motor_H


/*
A utility class for motor management and drive control.

Motors themselves support speed(float), coast(), stop(float) themselves.
See motordriver.h
*/

#include "motordriver.h"
#include "encoder.h"


Motor leftMotor(D0, D1, D2, 1); 
Motor rightMotor(D4, D5, D6, 1); 

const int SAMPLE_RATE = 1000; //per 1000 Microseconds

 
Ticker motorInOperation;
bool locked = 0; //Lock for async motorInOperation

//Lock class --- TODO: expand to motor and encoder lock since encoder cannot be reset.
class DriveLock {
    int _distance;
    float _speed;
    void(*_callback)(void);
public:
    DriveLock(int distance, float speed, void(*callback)(void)) : _distance(distance), _speed(speed), _callback(callback){}
	void drive(void) {
		if (getEncoderDistance() >= _distance){
			locked = false;
			motorInOperation.detach();
		}
	}
}