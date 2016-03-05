<<<<<<< HEAD:motor-encoder-other/pin_assignments.cpp
#ifndef PIN_ASSIGNMENTS_H
#define PIN_ASSIGNMENTS_H


#include "IO.h"
#include "motor.h"
#include "encoder.h"
#include "ir.h"

//IO
//DigitalIn user_button(USER_BUTTON);
//Serial pc(PA_2, PA_3); //D0, D1

//Encoder and motor channels are mirrowed for left/right to ensure positive is forwards. 
//If in the end it is all negative, 
Motor leftMotor(D9, D10, D11); 
Motor rightMotor(D12, D13, D14); 
Encoder leftEncoder(D7, D8); 
Encoder rightEncoder(D6, D5); 


//Sensors
//Gyro gyro(A0);

IRSensor leftIR1(D15, A1);
//IRSensor leftIR2(D14, A1);
//IRSensor leftIR3(D13, A1);
//IRSensor rightIR1(D15, A2);
//IRSensor rightIR2(D14, A2);
//IRSensor rightIR3(D13, A2);


#endif
=======
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
>>>>>>> 110ec1ffbc98eaa40298e8309535cf94c691c1a9:motor-encoder-other/pin_assignments.h
