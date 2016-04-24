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



// Motor Controller Code

// 85mm diameters
// wheel diameter: 24mm
// 42.5mm
// Turn circumference 85pi
// Wheel circumference 24pi
// 0.885 rotations/90degrees
// 0.885 * 40/8 * 512 * 2 =
// 4531 encoder units/90 degrees
        
        
// Calculations as below:
// Wheel Diameter = (approx.) 0.945
// Gear Ratio: 1 : 5 -> 1 rotation of motor = 5 rotations of wheel
// Distance the robot goes = (1/5)(2.969) = 0.5938
// Encoder CountsPerRotation = 16 (This value has to be verified.)
// center of wheel = 85 mm = 3.346 inches
// 3.346 * pi = 10.512 inches
// (10.512)(26.95) = 283.298  ***NEED TO ADJUST DUE TO SLIP AND DRAG
// CountsPerDegree = 283.3 / 360 = 0.787

Timer timeWithinCutoff = 0;     //Done turning if within angleCutoff for longer than timeCutoff
#define P_CONSTANT  1
#define D_CONSTANT  1
#define P_PERCENT2_CONSTANT 1
#define COUNTS_PER_ROTATION 16
#define WHEEL_SIZE  3.346
#define PI 3.141593
// Constants to define to calc. Will be refactored.
const float gearRatio = 8.0/40.0;       
const float inchesPerRotation = PI * WHEEL_SIZE * gearRatio;
const float countsPerInch = COUNTS_PER_ROTATION / inchesPerRotation;
const float countsPerDegree = 0.787;
const float DegreesPerCount = 1/countsPerDegree; 
float lastDiffP = 0.0f; 
float motor_speed = 0.0f;           
float degrees = 0.0f;
float diffPercent2P = 0.0f;

// Degrees -> positive
// Power -> controls direction
void _turn(){           
    int count = (degrees * countsPerDegree + 1/2);  // Adding offset for testing purposes.s
    // PID control code
    float diffP = 90 - DegreesPerCount * getEncoderDistance();  //How many more degrees
    float diffD = lastDiffP - diffP;    //current trend
    //Mainly controlling speed linearly by actual distance left. Derivative is to balance overshoots.
    //Percent^2 is used to ensure that small turns receive enough power to start. May or may not be necessary.
    diffPercent2P = sqrt(diffP / degrees);
    motor_speed = diffP * P_CONSTANT + diffD * D_CONSTANT + diffPercent2P * P_PERCENT2_CONSTANT;
    
    leftMotor.speed(motor_speed);
    rightMotor.speed(-motor_speed);

    drive_ticker.detach();
    leftMotor.stop();
    rightMotor.stop();
    
}
void turnRight(){
    degrees = 90;
    resetEncoders();
    drive_ticker.attach(&_turn, 0.01);
}

// Controls the left turn. Uses the PID and the 
// encoder to ensure precision.
void turnLeft(){
    degrees = -90;
    resetEncoders();
    drive_ticker.attach(&_turn, 0.01);
} 

/* Turn control function using the gyro */
/*
//All wheel turn PD control
//Remains in this state until finish
void turn(int degreesToTurn){
    Sensor gyro; //Gets gyro reading
    int goalDegrees = gyro + degreesToTurn; //current degrees + signed degrees to turn
    Timer timeWithinCutoff = 0 //Done turning if within angleCutoff for longer than timeCutoff
    
    while(1){
        if (abs(gyro - goalDegrees) < angleCutoff)
        {
            if (timeWithinCutoff > timeCutoff) //If stayed within valid range for long enough
                break; //Stops turning
        }
        else 
        {
            timeWithinCutoff = 0; //Outside of angleCutoff, reset timer
        }
        
        diffP = goalDegrees - gyro; //How many more degrees
        diffD = lastDiffP - diffP; //current trend
        //Mainly controlling speed linearly by actual distance left. Derivative is to balance overshoots.
        
        diffPercent2P = (diffP / degreesToTurn)^2 //How much more as a percent, squared
        //Percent^2 is used to ensure that small turns receive enough power to start. May or may not be necessary.
        
        motorSpeed = diffP * P_constant diffD * D_constant + diffPercent2P * P_percent2_constant; 
        
        
        if (motorSpeed > maxMotorSpeed)
        { 
            //Raw motor speed was computed, which likely exceeds max initially for longer turns
            motorSpeed = maxMotorSpeed; 
        }
        
        setLeftPwm(motorSpeed);
        setRightPwm(-motorSpeed);       
        lastDiffP = diffP;
    }
} */

