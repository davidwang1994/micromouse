#include "motor.h"


//Defines the 2 motors in use
Motor leftMotor(D3, D2, D1);
Motor rightMotor(D6, D5, D4);


//Stops the motors without breaking. Breaking may or may not be necessary pending test; just assume its mostly a complete stop for now
void stop(){
    leftMotor.stop();
    rightMotor.stop();
    leftEncoder.getPulses();
}

/*
Set motors to turn in place with given speed setting. -1.0 to 1.0. Positive is RIGHT. Negative is LEFT.
*/
void turn(float speed) {
    if (!locked){
    leftMotor.speed(speed);
    rightMotor.speed(-speed);
    }
}

//Set motors to drive forwards or backwards at given speed. Speed is a float between -1.0 and 1.0, positive for forwards and negative for backwards
void drive(float speed){
    if (!locked){
    leftMotor.speed(speed);
    rightMotor.speed(speed);
    }
}

//Set motors to drive approximately the given distance at given speed, without breaking 
//Asynchronous, with callback on termination
void drive(float speed, int distance, void(*callback)(void)){
    if (!locked){
        resetEncoders();
        DriveLock lock (distance, speed, callback);
        locked = true;
        motorInOperation.attach(&lock, &DriveLock::drive, 0.001); //Checks every milisecond
    }
}



//Defines a motor and its basic methods
Motor::Motor(PinName _pwm, PinName _fwd, PinName _rev):
        pwm(_pwm), fwd(_fwd), rev(_rev) {
            
    pwm.period(0.001);
    pwm = 0; 
    fwd = 0;
    rev = 0;
}
void Motor::speed(float speed) {
    fwd = speed > 0.0f;
    rev = speed < 0.0f;
    pwm = abs(speed);  //The one and only way to control power output with PWM is by setting the duty cycle, aka relative width. PFM is modulation of frequency, PWM is width.
}
void Motor::stop() {
    speed(0.0);
}


