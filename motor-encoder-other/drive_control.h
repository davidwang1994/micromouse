#include "pin_assignments.cpp"





#define CELL_DISTANCE		16384 //Encoder units, TO BE TESTED


//Control code for exploration. Callbacks are run after each finishes if given
void turnLeft(void(*callback)(void) = NULL);
void turnRight(void(*callback)(void) = NULL);
void turnAround(void(*callback)(void) = NULL);
void driveCell(void(*callback)(void) = NULL);

//Control code for speed runs. Callbacks are run after each finishes if given
void turnLeftSpeedRun(void(*callback)(void) = NULL);
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
void _driveCellDiagonalSpeedRun();