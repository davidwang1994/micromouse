#include "motor.h"


//Defines the 2 motors in use
Motor leftMotor(D3, D4, D5);
Motor rightMotor(D6, D7, D8);


void stop(){
    if (!locked){
        leftMotor.stop();
        rightMotor.stop();
    }
}


void turn(float speed) {
    if (!locked){
        leftMotor.speed(speed);
        rightMotor.speed(speed);
    }
}

void drive(float speed){
    if (!locked){
        leftMotor.speed(speed);
        rightMotor.speed(-speed);
    }
}

void drive(float speed, int distance, void(*callback)(void)){
    if (!locked){
        resetEncoders();
        DriveLock lock (distance, speed, callback);
        drive(speed);
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



void main(){
    drive(0.2);
    while(1){
        
    }
}

