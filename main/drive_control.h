#ifndef DRIVE_CONTROL_H
#define DRIVE_CONTROL_H

#include "pin_assignments.h"

//States from main
//bool DONE_MOVING; //Mouse is driving or turning if true, false otherwise
//bool UPDATE_POSITION; //True when just drove into next cell 


//Methods for drive control
void drive_cell();
void speed_drive_cell(int cells);
//void speed_drive_cell_diagonal(int cells); //TODO: use ir for collision detection and gyro


//Methods for turn control
void turn_left(void(*done_callback)(void));
void turn_right(void(*done_callback)(void));
void turn_around(void(*done_callback)(void));
void speed_turn_left(void(*done_callback)(void));
void speed_turn_right(void(*done_callback)(void));
void speed_turn_left_diagonal(void(*done_callback)(void));
void speed_turn_right_diagonal(void(*done_callback)(void));






/* Source */


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
//Simply sets both motors to 0
void stop(){
    leftMotor = 0;
    rightMotor = 0;
}



/*
Some rough math
Fastest exploration max_velocity = 23000/sec (2 cells/sec)
Try to keep it at around 11500/sec 
 - need measure distance at steady velocity if possible

Each cell is 11550, less than double our mouse size - need actually be very, very slow in general

1/11550 ~= 8.66e-5

Realistic P for exploration: max_speed * 8.66e-5 (exactly 0 at full distance)
 
Realistic D for exploration: 
keep speed < 23000/sec, or 23000*PID_SAMPLE_PERIOD per cycle (like 1150 (half is 575))
Speed should contribute less, so D must be less than max_speed. Lets say at 1150 I want to halve speed and it is at max_speed currently, D = max_speed /2300. But again, speed != velocity, so very rough guesses

say max_speed = 0.1, 
P = 8.66e-6, likely more, float cant fit and definitely not frequency accuract
D = 4.35e-5, likely less, float barely fits
So divide final results by 10^6,
P = 8.66 
D = 43.5 



PID for position: 
realistic position off values are -2 to 2
realistic speed adjust is maybe 20% max_speed, 0.02. 
P = 0.01 , likely high

realisitic change per second is 2 if actually stable, so each cycle 0.01. Say at 0.01 I want to halve it (0.01/2), so factor is... 1/2? 
D = 0.5 , likely high by quite a bit


*/


#define PID_SAMPLE_PERIOD               0.005 //every 5 ms



#define CELL_DISTANCE                   11550//22566 // = 1024 * (40/8) * (18 / (3.1415*1.3))   encoder_units_per_revolution * gear_ratio * (cell_size / wheel_circumference)
#define CELL_DISTANCE_DIAGONAL          CELL_DISTANCE * 1.414214
#define MOUSE_LENGTH                    6300 // encoder units, guess for now
#define DISTANCE_TO_NEXT_CELL           CELL_DISTANCE/2 + MOUSE_LENGTH/2 //Distance to drive to ensure mouse is fully in next cell 
#define DISTANCE_TO_NEXT_CELL_DIAGONAL  CELL_DISTANCE_DIAGONAL/2 + MOUSE_LENGTH/2 //Distance to drive to ensure mouse is fully in next cell 

#define DRIVE_CELL_DISTANCE_P           8.66 //divided by 10e6
#define DRIVE_CELL_DISTANCE_D           43.5 //divided by 10e6
#define SPEED_DRIVE_CELL_DISTANCE_P     0 //divided by 10e6
#define SPEED_DRIVE_CELL_DISTANCE_D     0 //divided by 10e6
#define SPEED_DRIVE_CELL_DISTANCE_DIAGONAL_P 0 //divided by 10e6
#define SPEED_DRIVE_CELL_DISTANCE_DIAGONAL_D 0 //divided by 10e6
#define DRIVE_CELL_POSITION_P           0.01
#define DRIVE_CELL_POSITION_D           0.5
#define DRIVE_CELL_POSITION_DIAGONAL_P  0
#define DRIVE_CELL_POSITION_DIAGONAL_D  0
#define ENCODER_TO_IR_CONVERSION        0

#define LEFT_IR_MIDDLE_VALUE            4.387f
#define RIGHT_IR_MIDDLE_VALUE           4.634f
#define IR_OFFSET                       RIGHT_IR_MIDDLE_VALUE - LEFT_IR_MIDDLE_VALUE

Ticker drive_ticker;



bool not_diagonal;  //Is not driving diagonally 
int total_distance; //Total distance to drive
float cell_distance; //Length of 1 cell; differs if diagonal
float distance_P;   //PID constant for distance
float distance_D;   //PID constant for distance
float position_P;   //PID constant for position
float position_D;   //PID constant for position
volatile int next_update_distance;  //Next distance to call update
volatile int last_distance_left;    //
volatile float last_position_error;

void _drive_cell(){
    //If in next cell, update position and start running maze algorithm
    if (leftEncoder > next_update_distance && rightEncoder > next_update_distance){
        next_update_distance += cell_distance;
        UPDATE_POSITION = true; //Signal to main that it is in next cell and update current position 
    }
    
    int distance_left = total_distance - getEncoderDistance();
    
    //If passed distance, then done (no oversteer control)
    if (distance_left <= 0){
        DONE_MOVING = true; //Signal to main
        drive_ticker.detach();
        pc.printf("left_encoder_actual: %d\r\n", leftEncoder);
        pc.printf("right_encoder_actual: %d\r\n", rightEncoder);
        stop();
        return;
    }
    
    //P is for distance, D is for velocity. Less distance, lower the speed. Higher the velocity, lower the speed (actually acceleration) 
    float motorSpeed = (distance_left * distance_P + (distance_left - last_distance_left) * distance_D) / 1000000;
    //if (motorSpeed < 0){
        //motorSpeed = 0;
    //}
    last_distance_left = distance_left;
    
    //PID for staying in middle
    float errorP;
    bool has_left = has_left_wall();
    bool has_right = has_right_wall();
    
    //Seeing both left and right wall
    if(has_left && has_right){ 
        errorP = rightIR - leftIR - IR_OFFSET;
    }        
    //only sees left wall and not diagonal
    else if(has_left && not_diagonal){
        errorP = 2 * (LEFT_IR_MIDDLE_VALUE - leftIR);
    }
    //only sees right wall and not diagonal
    else if(has_right && not_diagonal){
        errorP = 2 * (RIGHT_IR_MIDDLE_VALUE - rightIR);
    }
    //no walls, use encoder
    else {
        errorP = (rightEncoder - leftEncoder) * ENCODER_TO_IR_CONVERSION; 
    }
    
    float motorSpeedAdjust = position_P * errorP + position_D * (errorP - last_position_error);
    
    //Set speeds
    leftMotor = motorSpeed + motorSpeedAdjust;
    rightMotor = motorSpeed - motorSpeedAdjust;
    
    last_position_error = errorP;   
}

//Initializes driving using preset constraints
void _drive_init(){
    resetEncoders();
    last_distance_left = 0;
    last_position_error = 0;
    DONE_MOVING = false;
    drive_ticker.attach(&_drive_cell, PID_SAMPLE_PERIOD); 
}

//Drives one cell, updating current_position and calling update_maze when reached next cell, and calling get_next_move when drove full cell distance
void drive_cell(){
    cell_distance = CELL_DISTANCE;
    distance_P = DRIVE_CELL_DISTANCE_P;
    distance_D = DRIVE_CELL_DISTANCE_D;
    position_P = DRIVE_CELL_POSITION_P;
    position_D = DRIVE_CELL_POSITION_D;
    total_distance = CELL_DISTANCE;
    next_update_distance = DISTANCE_TO_NEXT_CELL;
    _drive_init();
}


//Speed run function to drive straight for given number of cells. Updates maze in every cell as usual
void speed_drive_cell(int cells){
    cell_distance = CELL_DISTANCE;
    distance_P = SPEED_DRIVE_CELL_DISTANCE_P;
    distance_D = SPEED_DRIVE_CELL_DISTANCE_D;
    position_P = DRIVE_CELL_POSITION_P;
    position_D = DRIVE_CELL_POSITION_D;
    total_distance = CELL_DISTANCE * cells;
    next_update_distance = DISTANCE_TO_NEXT_CELL;
    _drive_init();
}


/*//Speed run function to drive straight for given number of cells. Updates maze in every cell as usual
void speed_drive_cell_diagonal(int cells){
    cell_distance = CELL_DISTANCE_DIAGONAL;
    distance_P = SPEED_DRIVE_CELL_DISTANCE_DIAGONAL_P;
    distance_D = SPEED_DRIVE_CELL_DISTANCE_DIAGONAL_D;
    position_P = DRIVE_CELL_POSITION_DIAGONAL_P;
    position_D = DRIVE_CELL_POSITION_DIAGONAL_D;
    total_distance = CELL_DISTANCE_DIAGONAL * cells;
    next_update_distance = DISTANCE_TO_NEXT_CELL_DIAGONAL;
    _drive_init();
}
*/




#define DEGREES_PER_COUNT       0.01 
#define TURN_P_CONSTANT          1
#define TURN_D_CONSTANT          1
#define SPEED_TURN_P_CONSTANT          1
#define SPEED_TURN_D_CONSTANT          1
volatile float lastDiffP;           
float degrees;
float turn_P_constant;
float turn_D_constant;
void(*_turn_callback)(void) = NULL;

//Internal turn function, called by turn_right/left/around
void _turn(){
    // PID control code
    float diffP = degrees - DEGREES_PER_COUNT * getEncoderDistance();  //How many more degrees
    
    //If reached there within 5 degrees, stop
    if ((degrees > 0 && diffP < 5) || (degrees < 0 && diffP > -5)){
        drive_ticker.detach();
        
        stop();
        
        //Updates direction mouse is facing
        current_direction = next_direction;
        
        //Callback (to drive) if not null
        if (_turn_callback){
            _turn_callback();
        }
        //Otherwise it can stop moving
        else {
            DONE_MOVING = true;
        }
        
        return;
    }
    
    turn(diffP * turn_P_constant + (lastDiffP - diffP) * turn_D_constant); 
    lastDiffP = diffP;
}

//Initializes the turn using preset constraints
void _turn_init(void(*done_callback)(void)){
    resetEncoders();
    DONE_MOVING = false;
    drive_ticker.attach(&_turn, PID_SAMPLE_PERIOD);
    _turn_callback = done_callback;
}

// Controls the right turn. Uses the PID and the 
// encoder to ensure precision.
void turn_right(void(*done_callback)(void) = NULL){
    turn_P_constant = TURN_P_CONSTANT;
    turn_D_constant = TURN_D_CONSTANT;
    degrees = 90;
    _turn_init(done_callback);
}

// Controls the left turn. Uses the PID and the 
// encoder to ensure precision.
void turn_left(void(*done_callback)(void) = NULL){
    turn_P_constant = TURN_P_CONSTANT;
    turn_D_constant = TURN_D_CONSTANT;
    degrees = -90;
    _turn_init(done_callback);
} 

// Controls the 180 turn. Uses the PID and the 
// encoder to ensure precision.
void turn_around(void(*done_callback)(void) = NULL){
    turn_P_constant = TURN_P_CONSTANT;
    turn_D_constant = TURN_D_CONSTANT;
    degrees = 180;
    _turn_init(done_callback);
}

// Controls the right turn. Uses the PID and the 
// encoder to ensure precision.
void speed_turn_right(void(*done_callback)(void) = NULL){
    turn_P_constant = SPEED_TURN_P_CONSTANT;
    turn_D_constant = SPEED_TURN_D_CONSTANT;
    degrees = 90;
    _turn_init(done_callback);
}

// Controls the left turn. Uses the PID and the 
// encoder to ensure precision.
void speed_turn_left(void(*done_callback)(void) = NULL){
    turn_P_constant = SPEED_TURN_P_CONSTANT;
    turn_D_constant = SPEED_TURN_D_CONSTANT;
    degrees = -90;
    _turn_init(done_callback);
}

// Controls the right turn. Uses the PID and the 
// encoder to ensure precision.
void speed_turn_right_diagonal(void(*done_callback)(void) = NULL){
    turn_P_constant = SPEED_TURN_P_CONSTANT;
    turn_D_constant = SPEED_TURN_D_CONSTANT;
    degrees = 45;
    _turn_init(done_callback);
}

// Controls the left turn. Uses the PID and the 
// encoder to ensure precision.
void speed_turn_left_diagonal(void(*done_callback)(void) = NULL){
    turn_P_constant = SPEED_TURN_P_CONSTANT;
    turn_D_constant = SPEED_TURN_D_CONSTANT;
    degrees = -45;
    _turn_init(done_callback);
} 


#endif