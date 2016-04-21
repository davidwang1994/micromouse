/*
Notes: 
- About IR unit conversion: 
	 hasWallXDistanceAway can be determined by values within given range (cutoffs precomputed)
	 PID does not require extremely accurate approximations, as long as the relative values for left/right are same. Raw data would work, as will inverse graph with cutoff
	 PID against forwards wall should be fine with an invers graph with cutoff 
	


*/
#define CELL_DISTANCE		16384 //Encoder units, TO BE TESTED
#include "mbed.h"
#include "IO.h"
#include "motor.h"
#include "encoder.h"
#include "ir.h"


//Encoder and motor channels are mirrowed for left/right to ensure positive is forwards. 
//If in the end it is all negative, 
Motor leftMotor(D9, D10, D11); 
Motor rightMotor(D12, D13, D14); 
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
void drive_cell();
void _drive_cell();


//Assuming following things present elsewhere
bool hasRightWall() { return 0; };
bool hasLeftWall() { return 0; };
void update_maze() {}; //Runs complete maze algorithm
void run_next_move() {}; //Reads results from update_maze and runs turn or drive straight
int current_position[] = {0, 0}; //Set by update_maze and updated by drive_cell for update_maze to read
int next_position[] = {1, 0}; //Set by update_maze


/***** Main control "thread" *****/
//States on main control 
volatile bool moving = false; //Indicates that drive or turn functions are running 
volatile bool in_next_cell = false;
int main(){
		
		while(1){
				if (!moving){
						moving = true;
						run_next_move();
				}
				
				if (in_next_cell){
						in_next_cell = false;
						current_position[0] = next_position[0];
						current_position[1] = next_position[1];
						update_maze();
				}
			
				wait(0.0005); //Not actually necessary since everything else is interrupt driven
		}
}



//Since all methods and variables are in one place, don't need make functions take generic callbacks or variables passed in since otherwise it would need a
//class and an object to store callbacks and state variables, as well as prevent concurrent modification
//#define CELL_DISTANCE		22566 // = 1024 * (40/8) * (18 / (3.1415*1.3))   encoder_units_per_revolution * gear_ratio * (cell_size / wheel_circumference)
#define MOUSE_LENGTH		8000 	// encoder units, guess for now
#define DISTANCE_TO_NEXT_CELL		CELL_DISTANCE/2 + MOUSE_LENGTH/2  //Distance to drive to ensure mouse is fully in next cell 
#define DRIVE_CELL_DISTANCE_P		0
#define DRIVE_CELL_DISTANCE_D		0
#define DRIVE_CELL_POSITION_P		0
#define DRIVE_CELL_POSITION_D		0

Ticker motor_ticker;

const float leftIRMiddleValue = 0;
const float rightIRMiddleValue = 0;
#define ENCODER_TO_IR_CONVERSION_FACTOR		0

volatile int last_distance;
volatile float last_position_error;
volatile bool drive_cell_not_yet_next_cell;
volatile bool drive_cell_full_cell_passed;

//Drives one cell, updating current_position and calling update_maze when reached next cell, and calling get_next_move when drove full cell distance
void drive_cell(){
		resetEncoders();
		last_distance = 0;
		last_position_error = 0;
		drive_cell_not_yet_next_cell = true;
		motor_ticker.attach(&_drive_cell, 0.01); //10ms period, should be more accurate than necessary
}
//
void _drive_cell(){
		//If in next cell, update position and start running maze algorithm
		if (drive_cell_not_yet_next_cell && leftEncoder > DISTANCE_TO_NEXT_CELL && rightEncoder > DISTANCE_TO_NEXT_CELL){
				drive_cell_not_yet_next_cell = false;
				in_next_cell = true; //Signal to main
		}
		
		int distance = CELL_DISTANCE - getEncoderDistance();

		
		//If passed distance, then done (no oversteer control)
		if (distance <= 0){
				moving = false; //Signal to main
				motor_ticker.detach();
		}
		
		
		
		
		
		
		//PID for driving correct distance, including braking but not including oversteer control
		float motorSpeed = distance * DRIVE_CELL_DISTANCE_P + (distance - last_distance) * DRIVE_CELL_DISTANCE_D;
		if (motorSpeed > 1.0f){
				motorSpeed = 1.0f;
		}
		
		
		
		
		
		//PID for staying in middle
		float errorP, errorD;
		//Seeing both left and right wall
		if(hasLeftWall() && hasRightWall()){ 
				errorP = rightIR1 - leftIR1;
		}   	 
		//only sees left wall
		else if(hasLeftWall()){
				errorP = 2 * (leftIRMiddleValue - leftIR1);
		}
		//only sees right wall
		else if((hasRightWall())){
				errorP = 2 * (rightIRMiddleValue - rightIR1);
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
Timer timeWithinCutoff = 0; 	//Done turning if within angleCutoff for longer than timeCutoff
// Constants to define to calc. Will be refactored.
//const float countsPerRotation = 1400;	// To be verified
//const float gearRatio = 40.0/8.0;		// 40:8 gear ratio	
//const float wheelSize = 24.0;			// wheel diameter = 24mm
//const float pi = 3.141593;
//const float inchesPerRotation = pi * wheelSize * gearRatio;
//const float countsPerInch = countsPerRotation / inchesPerRotation;
const float countsPerDegree = 25;		// Will be refactored during testing. Assume 25 degrees.

// * Find the angle precision using encoders
// Degrees -> positive
// Power -> controls direction
void turn(float degrees, int power){
	int count;						
	count = (degrees * countsPerDegree + 1/2);	// Adding offset for testing purposes.s
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
