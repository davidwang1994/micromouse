#include "pin_assignments.h"


//Simply sets both motors to speed
void drive(float speed){
    leftMotor = speed;
    rightMotor = speed;
}

//Simply Sets both motors to speed in opposite directions. Turn right is positive.
void turn(float speed){
    leftMotor = speed;
    rightMotor = -speed;
}
void stop(){
    leftMotor = 0;
    rightMotor = 0;
}


#define CELL_DISTANCE       22566 // = 1024 * (40/8) * (18 / (3.1415*1.3))   encoder_units_per_revolution * gear_ratio * (cell_size / wheel_circumference)
#define MOUSE_LENGTH        8000    // encoder units, guess for now
#define DISTANCE_TO_NEXT_CELL       CELL_DISTANCE/2 + MOUSE_LENGTH/2  //Distance to drive to ensure mouse is fully in next cell 
#define DRIVE_CELL_DISTANCE_P       0
#define DRIVE_CELL_DISTANCE_D       0
#define DRIVE_CELL_POSITION_P       0
#define DRIVE_CELL_POSITION_D       0
#define ENCODER_TO_IR_CONVERSION_FACTOR     0

Ticker drive_ticker;

const float leftIRMiddleValue = 0;
const float rightIRMiddleValue = 0;


volatile int last_distance;
volatile float last_position_error;
volatile bool drive_cell_not_yet_next_cell;
volatile bool drive_cell_full_cell_passed;

void _drive_cell(){
    //If in next cell, update position and start running maze algorithm
    if (drive_cell_not_yet_next_cell && leftEncoder > DISTANCE_TO_NEXT_CELL && rightEncoder > DISTANCE_TO_NEXT_CELL){
        drive_cell_not_yet_next_cell = false;
        //in_next_cell = true; //Signal to main
    }
    
    int distance = CELL_DISTANCE - getEncoderDistance();

    //If passed distance, then done (no oversteer control)
    if (distance <= 0){
        //moving = false; //Signal to main
        drive_ticker.detach();
        leftMotor = 0;
        rightMotor = 0;
        return;
    }
    
    //PID for driving correct distance, including braking but not including oversteer control
    float motorSpeed = distance * DRIVE_CELL_DISTANCE_P + (distance - last_distance) * DRIVE_CELL_DISTANCE_D;
    if (motorSpeed > 1.0f){
        motorSpeed = 1.0f;
    }
    
    //PID for staying in middle
    float errorP, errorD;
    //Seeing both left and right wall
    if(has_left_wall() && has_right_wall()){ 
        errorP = rightIR - leftIR;
    }        
    //only sees left wall
    else if(has_left_wall()){
        errorP = 2 * (leftIRMiddleValue - leftIR);
    }
    //only sees right wall
    else if(has_right_wall()){
        errorP = 2 * (rightIRMiddleValue - rightIR);
    }
    //no walls, use encoder
    else {
        errorP = (rightEncoder - leftEncoder) * ENCODER_TO_IR_CONVERSION_FACTOR; 
    }
    
    //Get and update error
    errorD = errorP - last_position_error;
    last_position_error = errorP;
    
    //Get adjusted speeds 
    float motorSpeedAdjust = DRIVE_CELL_POSITION_P * errorP + DRIVE_CELL_POSITION_D * errorD;
    float leftMotorSpeed = motorSpeed + motorSpeedAdjust;
    if (leftMotorSpeed > 1.0f){
        leftMotorSpeed = 1.0f;
    }
    float rightMotorSpeed = motorSpeed - motorSpeedAdjust;
    if (rightMotorSpeed > 1.0f){
        rightMotorSpeed = 1.0f;
    }

    //Set speeds
    leftMotor = leftMotorSpeed;
    rightMotor = rightMotorSpeed;
}


//Drives one cell, updating current_position and calling update_maze when reached next cell, and calling get_next_move when drove full cell distance
void drive_cell(){
    resetEncoders();
    last_distance = 0;
    last_position_error = 0;
    drive_cell_not_yet_next_cell = true;
    drive_ticker.attach(&_drive_cell, 0.01); //10ms period, should be more accurate than necessary
}

