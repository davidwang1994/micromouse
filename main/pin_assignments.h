#include "motor.h"
#include "encoder.h"
#include "ir.h"
#include "gyro.h"
#include "buzzer.h"

//IO

extern Serial pc;

DigitalIn userButton(PC_10);

//Buzzer buzzer(PA_2);    //PA_2 might have conflict as it is the default serial TX on nucleo boards
Buzzer buzzer(PA_2);



DigitalOut led1(PB_15);
DigitalOut led2(PB_14);
DigitalOut led3(PB_13);
DigitalOut ledMf(PC_6);


//Motor
Motor leftMotor(PC_7, PC_8);
Motor rightMotor(PB_9, PB_8);


//Sensors
Gyro gyro(PB_0, PB_1);

Encoder leftEncoder(D7, D8);
Encoder rightEncoder(D6, D5);

IRSensor rightIR(PH_0, PA_4);
IRSensor rightDiagonalIR(PH_0, PA_4);
IRSensor rightFrontIR(PC_0, PA_6);
IRSensor leftFrontIR(PC_1, PA_7);
IRSensor leftDiagonalIR(PC_4, PC_4);
IRSensor leftIR(PC_3, PC_5);



