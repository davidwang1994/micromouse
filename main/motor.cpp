#include "motor.h"



//Defines a motor and its basic methods
Motor::Motor(PinName _pwm_pin, PinName _dir):
        pwm_pin(_pwm_pin), dir(_dir){
            
    pwm_pin.period(0.001);
    pwm_pin = 0; 
    dir = 0;
}

//Sets motor speed
void Motor::speed(float speed) {
    if (speed < 0.0f){ //Backwards
        if (speed < -1.0f){
            speed = -1.0f;
        }
        dir = 1;
        pwm_pin = speed + 1.0f; // Inverts it so 1 is off and 0 is on
    } else { //Forwards 
        if (speed > 1.0f){
            speed = 1.0f;
        }     
        dir = 0;
        pwm_pin = speed;
    }
}

//Sets motor speed to 0
void Motor::stop() {
    speed(0.0);
}




