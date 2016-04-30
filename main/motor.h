#ifndef MOTOR_H
#define MOTOR_H

#include "mbed.h"



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
    float read(){
        return _speed;
    }
    
private:
    volatile float _speed;
    PwmOut pwm_pin;
    DigitalOut dir;
};


extern Motor leftMotor;
extern Motor rightMotor;

#endif
