#ifndef MOTOR_H
#define MOTOR_H

#include "mbed.h"
#include "encoder.h"

//The two motors leftMotor and rightMotor are defined at the bottom.

/**
 * Represents a PWM controlled motor
 */
class Motor {
public:
    PwmOut pwm;
    DigitalOut fwd;
    DigitalOut rev;
    
    Motor(PinName _pwm, PinName _fwd, PinName _rev);
    
    /** 
     * Set the speed of the motor. Float value between -1.0 and 1.0
     */
    void speed(float speed);
    
    /**
     * Stop motor, without breaking.
     */
    void stop(void);
};




extern Motor leftMotor;
extern Motor rightMotor;

Ticker motorInOperation; //Ticker to drive async
bool locked = false; //Motor is locked for driving, or not --- alternatively could just use global state without need for lock object.

//Lock class for driving --- TODO(maybe): expand to motor and encoder lock since encoder cannot be reset.
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
            if (_callback != 0){
                _callback();
            }
        }
    }
};

#endif