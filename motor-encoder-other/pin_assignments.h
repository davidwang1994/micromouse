#ifndef __PIN_ASSIGNMENTS__
#define __PIN_ASSIGNMENTS__

#include "IO.h"
#include "motor.h"
#include "encoder.h"

//IO
//DigitalIn user_button(USER_BUTTON);
Serial pc(PA_2, PA_3); //D0, D1

//Encoder and motor channels are mirrowed for left/right to ensure positive is forwards. 
//If in the end it is all negative, 
Motor leftMotor(D9, D10, D11); 
Motor rightMotor(D12, D13, D14); 
Encoder leftEncoder(D7, D8); 
Encoder rightEncoder(D6, D5);


DigitalIn user_button(USER_BUTTON);

InterruptIn leftChannelA(D3);
InterruptIn rightChannelA(D5);

Ticker ticker;

// TODO assign pins
IRSensor frontIR1;
IRSensor frontIR2;
IRSensor leftIR;
IRSensor rightIR;
IRSensor rightIRDL;
IRSensor rightIRDR;


//Sensors
//Gyro gyro(A0);

#endif