/*
Notes: 
- About IR unit conversion: 
	 hasWallXDistanceAway can be determined by values within given range (cutoffs precomputed)
	 PID does not require extremely accurate approximations, as long as the relative values for left/right are same. Raw data would work, as will inverse graph with cutoff
	 PID against forwards wall should be fine with an invers graph with cutoff 
	


*/

#include "mbed.h"
#define CELL_DISTANCE   16384 //Encoder units, TO BE TESTED
//#include "drive_control.h"
//#include "pin_assignments.cpp"
#include "IO.h"
#include "motor.h"
#include "encoder.h"
#include "ir.h"

//IO
//DigitalIn user_button(USER_BUTTON);
//Serial pc(PA_2, PA_3); //D0, D1

//Encoder and motor channels are mirrowed for left/right to ensure positive is forwards. 
//If in the end it is all negative, 
Motor leftMotor(D9, D10); 
Motor rightMotor(D12, D11); 
Encoder leftEncoder(D7, D8); 
Encoder rightEncoder(D6, D5); 

//Sensors
//Gyro gyro(A0);
IRSensor leftIR1(D15, A1);
IRSensor leftIR2(D14, A1);
IRSensor leftIR3(D13, A1);
IRSensor rightIR1(D15, A2);
IRSensor rightIR2(D14, A2);
IRSensor rightIR3(D13, A2);

//Control code for exploration. Callbacks are run after each finishes if given
void turnLeft();
void turnRight();
void turnAround();
void driveCell();

//Control code for speed runs. Callbacks are run after each finishes if given
/**void turnLeftSpeedRun(void(*callback)(void) = NULL);
void turnRightSpeedRun(void(*callback)(void) = NULL);
void driveCellSpeedRun(void(*callback)(void) = NULL);
void driveCellDiagonalSpeedRun(void(*callback)(void) = NULL);


//Internal per sample functions
void _turnLeft();
void _turnRight();
void _turnAround();
void _driveCell();
void _turnLeftSpeedRun();
void _turnRightSpeedRun();
void _driveCellSpeedRun();
void _driveCellDiagonalSpeedRun(); **/

// Motor Controller Code
    // 85mm diameters
    // 40:8 gear ratio
    // wheel diameter: 24mm
    // 42.5mm
    // Turn circumference 85pi
    // Wheel circumference 24pi
    // 0.885 rotations/90degrees
    // 0.885 * 40/8 * 512 * 2 =
    // 4531 encoder units/90 degrees
Timer timeWithinCutoff = 0;   //Done turning if within angleCutoff for longer than timeCutoff
// Constants to define to calc. Will be refactored.
const float countsPerRotation = 1400; // To be verified
const float gearRatio = 40.0/8.0;   // 40:8 gear ratio  
const float wheelSize = 24.0;     // wheel diameter = 24mm
const float pi = 3.141593;
const float inchesPerRotation = pi * wheelSize * gearRatio;
const float countsPerInch = countsPerRotation / inchesPerRotation;
const float countsPerDegree = 25;   // Will be refactored during testing. Assume 25 degrees.

// * Find the angle precision using encoders
// Degrees -> positive
// Power -> controls direction
void turn(float degrees, int power){
  int count;            
  count = (degrees * countsPerDegree + 1/2);  // Adding offset for testing purposes.s
  resetEncoders();
  
  leftMotor.speed(power);
  rightMotor.speed(-power);
  
  while (getEncoderDistance() < count){
    // if the precision is reached, break out of the loop
    // Do nothing
  }
  leftMotor.stop();
  rightMotor.stop();
  
}
void turnRight(){
  
  turn(90, 100);
}

// Controls the left turn. Uses the PID and the 
// encoder to ensure precision.
void turnLeft(){
  turn(90, -100);
}




//Everything is in the same folder because I had them in that way on mbed. 
//motor/encoder/pin_assignemnt and this file are the only ones fully tested


DigitalIn user_button(USER_BUTTON);
Serial pc(PA_2, PA_3); //D0, D1

InterruptIn leftChannelA(D3);
InterruptIn rightChannelA(D5);

Ticker ticker;

void leftGotPulse(){
    pc.printf("Left: %i\r\n", leftEncoder);
}

void rightGotPulse(){
    pc.printf("Right: %i\r\n", rightEncoder);
}

float speed;

void printStatus(){
    pc.printf("\r\nLeft: %i\r\n", leftEncoder);
    pc.printf("Right: %i\r\n", rightEncoder);
    pc.printf("Motor Speed: %.2f\r\n", speed);
}

int main() {
  pc.printf("\r\nHi! Built with keil\r\n");
  
  //leftChannelA.rise(&leftGotPulse);
  //rightChannelA.rise(&rightGotPulse);
  
  bool pressed = false;
  int motorState = 0;
  speed = 0.0f;
  
  ticker.attach(&printStatus, 0.5);
  enableIR();
  while(1){
    if (user_button == 0 && !pressed){
        pressed = true;
        
        
        switch (motorState){
            case 0: speed = 0.0f; break;
            case 1: speed = 0.1f; break;
            case 2: speed = 0.5f; break;
            case 3: speed = 1.0f; break;
            case 4: speed = 0.0f; break;
            case 5: speed = -0.1f; break;
            case 6: speed = -0.5f; break;
            case 7: speed = -1.0f; motorState = -1; break;
        }
        motorState++;
    //    drive(speed);
    }
    else if (user_button == 1){
        pressed = false;
    }
  }
}
