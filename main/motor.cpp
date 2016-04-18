#include "motor.h"
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


