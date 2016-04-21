#ifndef IR_H
#define IR_H

#include "mbed.h"


DigitalOut IRLEDTX6_R (PH_0);
DigitalOut IRLEDTX5_RD (PH_1);
DigitalOut IRLEDTX4_RF (PC_0);
DigitalOut IRLEDTX3_LF (PC_1);
DigitalOut IRLEDTX2_LD (PC_2);
DigitalOut IRLEDTX1_L (PC_3);

AnalogIn irLedRX6_R(PA_4);
AnalogIn irLedRX5_RD(PA_5);
AnalogIn irLedRX4_RF(PA_6);
AnalogIn irLedRX3_LF(PA_7);
AnalogIn irLedRX2_LD(PC_4);
AnalogIn irLedRX1_L(PC_5);

Serial pc(PA_2, PA_3);

public:

/*
 * has_wall
 * Parameters: float, distance in cm
 * 	       wall_direction, char: 'l', 'r', 'f'
 * Returns: 0 if no wall is detected in that area
 * 	    1 if wall is within 1 cell distance
 *  	    2 if wall is within 2 cell distances
 * 	    3 if wall is within 3 cell distances
 */
int has_wall(float dist, char wall_direction);

/*
 * has_left_wall
 * Parameters: none
 * Returns: boolean, if there is a left wall detected or not, for PID control
 */
bool has_left_wall();

/*
 * has_right_wall
 * Parameters: none
 * Returns: boolean, if there is a right wall detected or not, for PID control
 */
bool has_right_wall();

//bool has_front_wall();

/*
 * ir_to_dist
 * Internal method, called by front_wall_dist, left_wall_dist, and right_wall_dist
 * Parameters: float, irVal from front_wall_dist
 * Returns: float, distance in cm from wall
 */
float ir_to_dist(float irVal);

/*
 * front_wall_dist
 * Parameters: None
 * Returns: float, distance in cm from FRONT WALL
 */
float front_wall_dist();

/*
 * left_wall_dist
 * Parameters: None
 * Returns: float, distance in cm from LEFT WALL
 */
float left_wall_dist();

/*
 * right_wall_dist
 * Parameters: None
 * Returns: float, distance in cm from RIGHT WALL
 */
float right_wall_dist();

#endif
