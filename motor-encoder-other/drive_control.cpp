#include "drive_control.h"


//Internal storage of after run callback functions
void (*_turnLeft_callback)();
void (*_turnRight_callback)();
void (*_turnAround_callback)();
void (*_driveCell_callback)();
void (*_turnLeftSpeedRun_callback)();
void (*_turnRightSpeedRun_callback)();
void (*_driveCellSpeedRun_callback)();
void (*_driveCellDiagonalSpeedRun_callback)();


//Assuming negative is left for gyro and encoder

void turnLeft(void(*callback)(void)){
		_turnLeft_callback = callback;
		gyro.enable(_turnLeft);
}

//P control
void _turnLeft(){
		float degreesLeft = gyro + 90.0;
		if (degreesLeft > 0){
				turn(-degreesLeft / 90);
		}
		else {
			gyro.disable();
			if (_turnLeft_callback){
				_turnLeft_callback();
			}
		}
}

void turnRight(void(*callback)(void)){
		_turnRight_callback = callback;
		gyro.enable(_turnRight);
}

//P control
void _turnRight(){
		float degreesLeft = 90.0 - gyro;
		if (degreesLeft > 0){
				turn(degreesLeft / 90);
		}
		else {
			gyro.disable();
			if (_turnRight_callback){
				_turnRight_callback();
			}
		}
}

//Turns right 180.
void turnAround(void(*callback)(void)){
		_turnAround_callback = callback;
		gyro.enable(_turnAround);
}

//P control
void _turnAround(){
		float degreesLeft = 180.0 - gyro;
		
		if (degreesLeft > 90){
				turn(1);
		}
		else if (degreesLeft > 0){
				turn(degreesLeft / 90);
		}
		else {
				gyro.disable();
				if (_turnAround_callback){
					_turnAround_callback();
				}
		}
}

void driveOneCell(){
		
}

