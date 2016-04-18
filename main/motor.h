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
 * The motor accepts 2 pins, one pwm and one non-pwm, controlled as in http://www.talkingelectronics.com/projects/H-Bridge/H-Bridge-1.html design 3.
 * Pins should be mirrowed across the 2 motors.
 * When both are same signal, brake.
 * The dir pin controls direction (0 forwards, 1 backward), while the pwm pin does pwm using the dir value as off and the opposite value as on
 *
 * Speed control logic flips the pwm period to set the baseline value to 1 when dir is one
 */
class Motor {
public:
	  
    Motor(PinName _pwm, PinName _dir);
    
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
		
private:
		PwmOut pwm_pin;
    DigitalOut dir;
};




extern Motor leftMotor;
extern Motor rightMotor;

<<<<<<< HEAD
=======

extern Ticker motorInOperation; //Ticker to for async driving
extern bool locked; //Motor is locked for driving, or not --- alternatively could just use global state without need for lock object (maybe).



>>>>>>> Austin
#endif

