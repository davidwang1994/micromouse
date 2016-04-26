#ifndef PIN_ASSIGNMENTS_H
#define PIN_ASSIGNMENTS_H

#include "motor.h"
#include "encoder.h"
#include "ir.h"
#include "gyro.h"
#include "buzzer.h"





extern Serial pc;
AnalogIn battery(PA_3);

//Oh wait I can also spin the wheels for io settings... say speed
//Also need a way to store last valid maze map (at center or beginning) so can go back to it
//We can see if the mouse found the optimal path already and tell the mouse to begin speedrun by grabbing it and telling it to begin speedrun




DigitalIn userButton(PC_10);

//Buzzer buzzer(PA_2);    //PA_2 might have conflict as it is the default serial TX on nucleo boards
Buzzer buzzer(PA_2);

DigitalOut ledRed(PB_15);
DigitalOut ledYellow(PB_14);
DigitalOut ledGreen(PB_13);
DigitalOut ledMf(PC_6);


//Motor
Motor leftMotor(PC_8, PC_7); 
Motor rightMotor(PB_9, PB_8);


//Sensors
Gyro gyro(PB_0, PB_1);

Encoder leftEncoder(PB_3, PB_4); 
Encoder rightEncoder(PB_7, PB_6); 

IRSensor rightIR(PH_0, PA_4);
IRSensor rightDiagonalIR(PH_1, PA_5);
IRSensor rightFrontIR(PC_0, PA_6);
//IRSensor leftFrontIR(PC_1, PA_7);
IRSensor leftDiagonalIR(PC_4, PC_2);
IRSensor leftIR(PC_3, PC_5);


#endif
