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



//Defines a motor and its basic methods
Motor::Motor(PinName _pwm_pin, PinName _dir):
        pwm_pin(_pwm_pin), dir(_dir){
            
    pwm_pin.period(0.001);
    pwm_pin = 0; 
    dir = 0;
}
				
void Motor::speed(float speed) {
		if (speed < 0.0f){ //Backwards
				dir = 1;
				pwm_pin = speed + 1.0f; // Inverts it so 1 is off and 0 is on
		}
		else { 
				//Forwards
				dir = 0;
				pwm_pin = speed;
		}
}

void Motor::stop() {
    speed(0.0);
}




