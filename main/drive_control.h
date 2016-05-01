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
int print_count = 0;
float log_lir[200];
float log_rir[200];
float log_ep[200];
float log_sa[200];
float log_lm[200];
float log_rm[200];


//TODO!!!!: how is maze alg going to drive back to beginning?


//test fix, or try other fix (0.001f)TODO: its not going fast enough at end of drive if more than like 2 cells 

//TODO: make sure center only has 1 entrance. 
//TODO: verify_at_center()

//TODO: add lost checking while turning.


//Done. have maze implement it. TODO: avoid double reading ir - changed, now need maze to read given values
//done (most things tested). TODO: check and fix #defines.
//Done. Now make sure other code is changedTODO: maze cant use has_wall(), instead received passed in values
//Done.  Now there will only be one stop per turn (unless maze is slow), and no longer need to be super accurate! //TODO: if keep driving straight, as soon as it is determined, add the distance to the total distance so it doesnt stop. //TODO: carry over distance driven too far/too little. allows for much lower accuracy
//done. TODO: drive straignt time out


#define PID_SAMPLE_PERIOD               0.01//0.01 //every 5 ms

#define CELL_DISTANCE                   34500//11500  //22566 // = 1024 * (40/8) * (18 / (3.1415*1.3))   encoder_units_per_revolution * gear_ratio * (cell_size / wheel_circumference)
#define CELL_DISTANCE_DIAGONAL          CELL_DISTANCE * 1.414214
#define MOUSE_LENGTH                    6300 
#define DISTANCE_TO_NEXT_CELL           8900//CELL_DISTANCE/2 + MOUSE_LENGTH/2 //Distance to drive to ensure mouse is fully in next cell 
#define DISTANCE_TO_NEXT_CELL_DIAGONAL  19413//CELL_DISTANCE_DIAGONAL/2 + MOUSE_LENGTH/2 //Distance to drive to ensure mouse is fully in next cell 

#define DRIVE_CELL_DISTANCE_P           22//8.66 //divided by 10e6
#define DRIVE_CELL_DISTANCE_D           43.5 //divided by 10e6
#define SPEED_DRIVE_CELL_DISTANCE_P     0 //divided by 10e6
#define SPEED_DRIVE_CELL_DISTANCE_D     0 //divided by 10e6
#define SPEED_DRIVE_CELL_DISTANCE_DIAGONAL_P 0 //divided by 10e6
#define SPEED_DRIVE_CELL_DISTANCE_DIAGONAL_D 0 //divided by 10e6
#define DRIVE_CELL_POSITION_P           0.1f //0.05 to 0.1 
#define DRIVE_CELL_POSITION_D           0 //0 seems good
#define DRIVE_CELL_POSITION_DIAGONAL_P  0
#define DRIVE_CELL_POSITION_DIAGONAL_D  0
#define ENCODER_TO_IR_CONVERSION        0

#define LEFT_IR_MIDDLE_VALUE            4.387f
#define RIGHT_IR_MIDDLE_VALUE           4.634f
#define IR_OFFSET                       -0.147f

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
int same_count; //If last error == this error for this many cycles of pid, then is stuck right before distance so manually stop it


void _drive_cell(){
    
    if (print_count >= 200){
        print_count = 199;
    }
    
    
    
    
    //If update has finished before reaching stopping and the next action is to keep driving, just add more distance and keep driving
    if (UPDATE_FINISHED) {
        UPDATE_FINISHED = false;
        if (mouse_action == DRIVE){
            total_distance += CELL_DISTANCE;
            last_distance_left += CELL_DISTANCE;
            pc.printf("keep going\r\n");
        }
    }
    
    
    int distance_left = total_distance - getEncoderDistance();
    
    //If passed distance or encoder remained still for 50ms (too slow to move), then done (no oversteer control)
    if (distance_left <= 50 || same_count >= 5){
        
        drive_ticker.detach();
        
        stop();
        
        DONE_MOVING = true; //Signal to main
        //pc.printf("left_encoder_actual: %d\r\n", leftEncoder);
//        pc.printf("right_encoder_actual: %d\r\n", rightEncoder);
//        for (int i = 0; i < 199; i++){
//            //pc.printf("left motor: %f\r\n", log_lm[i]);
//            //pc.printf("right motor: %f\r\n", log_rm[i]);
//            //pc.printf("errorP: %f\r\n", log_ep[i]);
//            //pc.printf("speed adjust: %f\r\n", log_sa[i]);
//            pc.printf("left ir: %f\r\n", log_lir[i]);
//            pc.printf("right ir: %f\r\n", log_rir[i]);
//        }
        return;
    }
    
    
    
    
    //P is for distance, D is for velocity. Less distance, lower the speed. Higher the velocity, lower the speed (actually acceleration) 
    float motorSpeed = ((distance_left * distance_P) + ((distance_left - last_distance_left) * distance_D)) / 1000000;
    
    if (last_distance_left - distance_left){
        same_count++;
    }
    else {
        same_count = 0;
    }
    
    last_distance_left = distance_left;
    
    if (motorSpeed > drive_top_speed){
        motorSpeed = drive_top_speed;
    }
    
    //PID for staying in middle
    float errorP = 0.0f;
    
    float irl = log_lir[print_count] = leftIR.readIR();
    float irr = log_rir[print_count] = rightIR.readIR();
    has_left_wall = irl < 12;
    has_right_wall = irr < 12;
    
    //Seeing both left and right wall
    if(has_left_wall && has_right_wall){ 
        log_ep[print_count] = errorP = irl - irr - IR_OFFSET;
    }      
    //only sees left wall and not diagonal
    else if(has_left_wall){
        errorP = 2 * (irl - LEFT_IR_MIDDLE_VALUE);
    }
    //only sees right wall and not diagonal
    else if(has_right_wall){
        errorP = 2 * (RIGHT_IR_MIDDLE_VALUE - irr);
    }
    //no walls, use encoder
    else {
        errorP = (rightEncoder - leftEncoder) * ENCODER_TO_IR_CONVERSION; 
    }
    
    float motorSpeedAdjust = position_P * errorP - position_D * (errorP - last_position_error);
    log_sa[print_count] = motorSpeedAdjust;
    if (motorSpeedAdjust < -drive_top_speed / 2){
        motorSpeedAdjust = -drive_top_speed / 2;
    }
    else if (motorSpeedAdjust > drive_top_speed / 2 ){
        motorSpeedAdjust = drive_top_speed / 2;
    }
    //Set speeds
    leftMotor = log_lm[print_count] =  motorSpeed - motorSpeedAdjust;
    rightMotor = log_rm[print_count] = motorSpeed + motorSpeedAdjust;
    
    last_position_error = errorP;   
    
    print_count++;
    
    
    
    //If in next cell, update position and start running maze algorithm
    if (leftEncoder > next_update_distance && rightEncoder > next_update_distance){
        next_update_distance += cell_distance;
        UPDATE_POSITION = true; //Signal to main that it is in next cell and update current position 
        
        
        
    }
}

//Initializes driving using preset constraints
void _drive_init(){
    same_count = 0;
    
    //Keeps using last drive encoder over/under values if drive again
    if (!LAST_ACTION_WAS_DRIVE){
        resetEncoders();
        LAST_ACTION_WAS_DRIVE = true;    
    }
    
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
    _drive_callback = done_callback;
    
    _drive_init();
}
*/




#define DEGREES_PER_COUNT       0.01875 //Need test
#define TURN_P_CONSTANT          0.05
#define TURN_D_CONSTANT          0//0.001
#define SPEED_TURN_P_CONSTANT          0
#define SPEED_TURN_D_CONSTANT          0
volatile float lastDiffP;           
float degrees;
float turn_P_constant;
float turn_D_constant;
void(*_turn_callback)(void) = NULL;

//Internal turn function, called by turn_right/left/around
void _turn(){
    // PID control code
    float diffP = degrees - 0.01875f  * leftEncoder;  //How many more degrees
    
    //If reached there within 5 degrees, stop
    if ((degrees > 0 && diffP < 1) || (degrees < 0 && diffP > -1) || same_count >= 5){
        drive_ticker.detach();
        
        
        //Updates direction mouse is facing
        current_direction = next_direction;
        
        //Callback (to drive) if not null
        if (_turn_callback){
            _turn_callback();
        }
        //Otherwise it can stop moving
        else {
            DONE_MOVING = true;
            stop();
        }
        
        pc.printf("left_encoder_actual: %d\r\n", leftEncoder);
        pc.printf("right_encoder_actual: %d\r\n", rightEncoder);
        
        return;
    }
    
    float motorSpeed = diffP * turn_P_constant - (lastDiffP - diffP) * turn_D_constant;
    if (motorSpeed > turn_top_speed){
        motorSpeed = turn_top_speed;
    }
    else if (motorSpeed < -turn_top_speed){
        motorSpeed = -turn_top_speed;
    }
    turn(motorSpeed); 
    
    if (diffP == lastDiffP){
        same_count++;
    }
    lastDiffP = diffP;
}

//Initializes the turn using preset constraints
void _turn_init(void(*done_callback)(void)){
    same_count = 0;
    resetEncoders();
    LAST_ACTION_WAS_DRIVE = false;
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