#include "pin_assignments.h"


//Methods for drive control
void drive_cell();
void speed_drive_cell(int cells);
void speed_drive_cell_diagonal(int cells);


//Methods for turn control
void turn_left();
void turn_right();
void turn_around();
void speed_turn_left();
void speed_turn_right();
void speed_turn_left_diagonal();
void speed_turn_right_diagonal();



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


#define CELL_DISTANCE                   22566 // = 1024 * (40/8) * (18 / (3.1415*1.3))   encoder_units_per_revolution * gear_ratio * (cell_size / wheel_circumference)
#define CELL_DISTANCE_DIAGONAL          CELL_DISTANCE * 1.414214
#define MOUSE_LENGTH                    8000 // encoder units, guess for now
#define DISTANCE_TO_NEXT_CELL           CELL_DISTANCE/2 + MOUSE_LENGTH/2 //Distance to drive to ensure mouse is fully in next cell 
#define DISTANCE_TO_NEXT_CELL_DIAGONAL  CELL_DISTANCE_DIAGONAL/2 + MOUSE_LENGTH/2 //Distance to drive to ensure mouse is fully in next cell 
#define DRIVE_CELL_DISTANCE_P           0 //Slows to near stop after
#define DRIVE_CELL_DISTANCE_D           0
#define SPEED_DRIVE_CELL_DISTANCE_P     0 //Still decently fast after
#define SPEED_DRIVE_CELL_DISTANCE_D     0
#define SPEED_DRIVE_CELL_DISTANCE_DIAGONAL_P 0
#define SPEED_DRIVE_CELL_DISTANCE_DIAGONAL_D 0
#define DRIVE_CELL_POSITION_P           0
#define DRIVE_CELL_POSITION_D           0
#define DRIVE_CELL_POSITION_DIAGONAL_P  0
#define DRIVE_CELL_POSITION_DIAGONAL_D  0
#define ENCODER_TO_IR_CONVERSION        0

Ticker drive_ticker;

const float leftIRMiddleValue = 0;
const float rightIRMiddleValue = 0;

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
        //in_next_cell = true; //Signal to main that it is in next cell and update current position 
    }
    
    int distance_left = total_distance - getEncoderDistance();
    
    //If passed distance, then done (no oversteer control)
    if (distance_left <= 0){
        //moving = false; //Signal to main
        drive_ticker.detach();
        return;
    }
    
    //PID for driving correct distance, including braking but not including oversteer control
    float motorSpeed = distance_left * distance_P + (distance_left - last_distance_left) * distance_D;
    if (motorSpeed > 1.0f){
        motorSpeed = 1.0f;
    }
    last_distance_left = distance_left;
    
    //PID for staying in middle
    float errorP;
    bool has_left = has_left_wall();
    bool has_right = has_right_wall();
    
    //Seeing both left and right wall
    if(has_left && has_right){ 
        errorP = rightIR - leftIR;
    }        
    //only sees left wall and not diagonal
    else if(has_left && not_diagonal){
        errorP = 2 * (leftIRMiddleValue - leftIR);
    }
    //only sees right wall and not diagonal
    else if(has_right && not_diagonal){
        errorP = 2 * (rightIRMiddleValue - rightIR);
    }
    //no walls, use encoder
    else {
        errorP = (rightEncoder - leftEncoder) * ENCODER_TO_IR_CONVERSION; 
    }
    
    float motorSpeedAdjust = DRIVE_CELL_POSITION_P * errorP + DRIVE_CELL_POSITION_D * (errorP - last_position_error);
    
    //Set speeds
    leftMotor = motorSpeed + motorSpeedAdjust;
    rightMotor = motorSpeed - motorSpeedAdjust;
    
    last_position_error = errorP;   
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
    resetEncoders();
    last_distance_left = 0;
    last_position_error = 0;
    drive_ticker.attach(&_drive_cell, 0.01); 
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
    resetEncoders();
    last_distance_left = 0;
    last_position_error = 0;
    drive_ticker.attach(&_drive_cell, 0.01);
}

//Speed run function to drive straight for given number of cells. Updates maze in every cell as usual
void speed_drive_cell_diagonal(int cells){
    cell_distance = CELL_DISTANCE_DIAGONAL;
    distance_P = SPEED_DRIVE_CELL_DISTANCE_DIAGONAL_P;
    distance_D = SPEED_DRIVE_CELL_DISTANCE_DIAGONAL_D;
    position_P = DRIVE_CELL_POSITION_DIAGONAL_P;
    position_D = DRIVE_CELL_POSITION_DIAGONAL_D;
    total_distance = CELL_DISTANCE_DIAGONAL * cells;
    next_update_distance = DISTANCE_TO_NEXT_CELL_DIAGONAL;
    resetEncoders();
    last_distance_left = 0;
    last_position_error = 0;
    drive_ticker.attach(&_drive_cell, 0.01);
}





#define DEGREES_PER_COUNT       0.01 
#define TURN_P_CONSTANT          1
#define TURN_D_CONSTANT          1
#define SPEED_TURN_P_CONSTANT          1
#define SPEED_TURN_D_CONSTANT          1
volatile float lastDiffP;           
float degrees;
float turn_P_constant;
float turn_D_constant;

//Internal turn function, called by turn_right/left/around
void _turn(){
    // PID control code
    float diffP = degrees - DEGREES_PER_COUNT * getEncoderDistance();  //How many more degrees
    
    //If reached there within 5 degrees, stop
    if ((degrees > 0 && diffP < 5) || (degrees < 0 && diffP > -5)){
        drive_ticker.detach();
        stop();
        return;
    }
    
    turn(diffP * turn_P_constant + (lastDiffP - diffP) * turn_D_constant); 
    lastDiffP = diffP;
}

// Controls the right turn. Uses the PID and the 
// encoder to ensure precision.
void turn_right(){
    turn_P_constant = TURN_P_CONSTANT;
    turn_D_constant = TURN_D_CONSTANT;
    degrees = 90;
    resetEncoders();
    drive_ticker.attach(&_turn, 0.01);
}

// Controls the left turn. Uses the PID and the 
// encoder to ensure precision.
void turn_left(){
    turn_P_constant = TURN_P_CONSTANT;
    turn_D_constant = TURN_D_CONSTANT;
    degrees = -90;
    resetEncoders();
    drive_ticker.attach(&_turn, 0.01);
} 

// Controls the 180 turn. Uses the PID and the 
// encoder to ensure precision.
void turn_around(){
    turn_P_constant = TURN_P_CONSTANT;
    turn_D_constant = TURN_D_CONSTANT;
    degrees = 180;
    resetEncoders();
    drive_ticker.attach(&_turn, 0.01);
}

// Controls the right turn. Uses the PID and the 
// encoder to ensure precision.
void speed_turn_right(){
    turn_P_constant = SPEED_TURN_P_CONSTANT;
    turn_D_constant = SPEED_TURN_D_CONSTANT;
    degrees = 90;
    resetEncoders();
    drive_ticker.attach(&_turn, 0.01);
}

// Controls the left turn. Uses the PID and the 
// encoder to ensure precision.
void speed_turn_left(){
    turn_P_constant = SPEED_TURN_P_CONSTANT;
    turn_D_constant = SPEED_TURN_D_CONSTANT;
    degrees = -90;
    resetEncoders();
    drive_ticker.attach(&_turn, 0.01);
}

// Controls the right turn. Uses the PID and the 
// encoder to ensure precision.
void speed_turn_right_diagonal(){
    turn_P_constant = SPEED_TURN_P_CONSTANT;
    turn_D_constant = SPEED_TURN_D_CONSTANT;
    degrees = 45;
    resetEncoders();
    drive_ticker.attach(&_turn, 0.01);
}

// Controls the left turn. Uses the PID and the 
// encoder to ensure precision.
void speed_turn_left_diagonal(){
    turn_P_constant = SPEED_TURN_P_CONSTANT;
    turn_D_constant = SPEED_TURN_D_CONSTANT;
    degrees = -45;
    resetEncoders();
    drive_ticker.attach(&_turn, 0.01);
} 