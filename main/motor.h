#ifndef MOTOR_H
#define MOTOR_H

#include "mbed.h"
#include "encoder.h"
#include "gyro.h"






//Stops the motors without breaking. Breaking may or may not be necessary pending test; just assume its mostly a complete stop for now
void stop();


//Set motors to turn in place with given speed setting. -1.0 to 1.0. Positive is RIGHT. Negative is LEFT.
void turn(float speed);


//Set motors to drive forwards or backwards at given speed. Speed is a float between -1.0 and 1.0, positive for forwards and negative for backwards
void drive(float speed);

//Set motors to drive approximately the given distance at given speed, without breaking 
//Asynchronous, with callback on termination
void drive(float speed, int distance, void(*callback)(void));



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
    
    //Shorthand for setting speed
    void operator= (float speed) {
        Motor::speed(speed);
    }
};




extern Motor leftMotor;
extern Motor rightMotor;


extern Ticker motorInOperation; //Ticker to for async driving
extern bool locked; //Motor is locked for driving, or not --- alternatively could just use global state without need for lock object (maybe).


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
            stop();
            motorInOperation.detach();
            if (_callback != 0){
                _callback();
            }
        }
    }
};

#endif
