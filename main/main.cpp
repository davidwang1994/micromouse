/*
Notes: 
- About IR unit conversion: 
	 hasWallXDistanceAway can be determined by values within given range (cutoffs precomputed)
	 PID does not require extremely accurate approximations, as long as the relative values for left/right are same. Raw data would work, as will inverse graph with cutoff
	 PID against forwards wall should be fine with an invers graph with cutoff 
	


*/

#include "mbed.h"
//#include "drive_control.h"
//#include "pin_assignments.cpp"


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
#define CELL_DISTANCE		22566 // = 1024 * (40/8) * (18 / (3.1415*1.3))   encoder_units_per_revolution * gear_ratio * (cell_size / wheel_circumference)
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
		if(hasLeftWall() && hasRightWall())
		{ 
				errorP = rightIR1 - leftIR1;
		}   	 
		//only sees left wall
		else if(hasLeftWall())
		{
				errorP = 2 * (leftIRMiddleValue - leftIR1);
		}
		//only sees right wall
		else if((hasRightWall()))
		{
				errorP = 2 * (rightIRMiddleValue - rightIR1);
		}
		//no walls, use encoder
		else
		{
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







//Everything is in the same folder because I had them in that way on mbed. 
//motor/encoder/pin_assignemnt and this file are the only ones fully tested

/*
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
        drive(speed);
    }
    else if (user_button == 1){
        pressed = false;
    }
  }
}*/
