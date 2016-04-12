#include "motor.h"

bool locked = false;


Ticker motorInOperation; //Ticker to for async driving


void stop(){
		leftMotor.stop();
		rightMotor.stop();
}


void turn(float speed) {
		leftMotor.speed(speed);
		rightMotor.speed(-speed);
}

void drive(float speed){
		leftMotor.speed(speed);
		rightMotor.speed(speed);
}

void drive(float speed, int distance, void(*callback)(void)){
        resetEncoders();
        DriveLock lock (distance, speed, callback);
        drive(speed);
        motorInOperation.attach(&lock, &DriveLock::drive, 0.001); //Checks every milisecond
}

//Defines a motor and its basic methods
Motor::Motor(PinName _pwm, PinName _dir):
        pwm(_pwm), dir(_dir){
            
    pwm.period(0.001);
    pwm = 0; 
    dir = 0;
}
				
void Motor::speed(float speed) {
		if (speed < 0.0f){ //Backwards
				dir = 1;
				pwm = speed + 1.0f; // Inverts it so 1 is off and 0 is on
		}
		else { 
				//Forwards
				dir = 0;
				pwm = speed;
		}
}

void Motor::stop() {
    speed(0.0);
}




