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

//1 us
float firingTime = 0.000001;

/*
 * has_wall
 * Parameters: float, distance in cm
 * 	       wall_direction, char: 'l', 'r', 'f'
 * Returns: 0 if no wall is detected in that area
 * 	    1 if wall is within 1 cell distance
 *  	    2 if wall is within 2 cell distances
 * 	    3 if wall is within 3 cell distances
 */
int ir::has_wall(float dist, char wall_direction)
{
	int walls;
	if (wall_direction = 'l')
	{
		if (dist < 12)
			return wall = 1;
		else if (dist < 29)
			return wall = 2;
		else if (dist < 46)
			return wall = 3;
		else
			return wall = 0;

	}
	else if (wall_direction = 'r')
	{
		if (dist < 12)
			return wall = 1;
		else if (dist < 29)
			return wall = 2;
		else if (dist < 46)
			return wall = 3;
		else
			return wall = 0;

	}
	else if (wall_direction = 'f')
	{
		if (dist < 12)
			return wall = 1;
		else if (dist < 29)
			return wall = 2;
		else if (dist < 46)
			return wall = 3;
		else
			return wall = 0;
	}

}

/*
 * ir_to_dist
 * Internal method, called by front_wall_dist, left_wall_dist, and right_wall_dist
 * Parameters: float, irVal from front_wall_dist
 * Returns: float, distance in cm from wall
 */
float ir::ir_to_dist(float irVal)
{
	//Call David's regression algorithm
	return newDist;
}

/*
 * front_wall_dist
 * Parameters: None
 * Returns: float, distance in cm from FRONT WALL
 */
float ir::front_wall_dist()
{
	float arr[5];
	float sumLeft = 0;
	float sumRight = 0;

	//Each duration takes 100us, 5 times = 0.5ms
	for (int i = 0; i < 5; i++)
	{
		//Turn on IR LED tx
		IRLEDTX4_RF = 1;

		//Wait for capacitor to fire, 10us
		delay(10*firingTime);
		arr[i] = irLedRX4_RF.read();
		sumLeft += arr[i];

		//Wait 5us for turning off IR LED tx
		delay(5*firingTime);
		IRLEDTX4_RF = 0;

		//Wait 85us for turning on IR LED tx
		delay(85*firingTime);
	}

	//Each duration takes 100us, 5 times = 0.5ms
	for (int i = 0; i < 5; i++)
	{
		//Turn on IR LED tx
		IRLEDTX3_LF = 1;

		//Wait for capacitor to fire, 10us
		delay(10*firingTime);
		arr[i] = irLedRX3_LF.read();
		sumRight += arr[i];

		//Wait 5us for turning off IR LED tx
		delay(5*firingTime);
		IRLEDTX3_LF = 0;

		//Wait 85us for turning on IR LED tx
		delay(85*firingTime);
	}

	float leftAvg = sumLeft/5.0;
	float rightAvg = sumRight/5.0;

	if (leftAvg - rightAvg > 5)
	{
		pc.baud(9600);
		pc.printf("IR Front Sensors don't match");
	}
	return leftAvg;
}

/*
 * left_wall_dist
 * Parameters: None
 * Returns: float, distance in cm from LEFT WALL
 */
float ir::left_wall_dist()
{
	float arr[5];
	float sum = 0;

	//Each duration takes 100us, 5 times = 0.5ms
	for (int i = 0; i < 5; i++)
	{
		//Turn on IR LED tx
		IRLEDTX1_L = 1;

		//Wait for capacitor to fire, 10us
		delay(10*firingTime);
		arr[i] = irLedRX1_L.read();
		sum += arr[i];

		//Wait 5us for turning off IR LED tx
		delay(5*firingTime);
		IRLEDTX1_L = 0;

		//Wait 85us for turning on IR LED tx
		delay(85*firingTime);
	}

	float avg = sum/5.0;

	return avg;
}

/*
 * right_wall_dist
 * Parameters: None
 * Returns: float, distance in cm from RIGHT WALL
 */
float ir::right_wall_dist()
{
	float arr[5];
	float sum = 0;

	//Each duration takes 100us, 5 times = 0.5ms
	for (int i = 0; i < 5; i++)
	{
		//Turn on IR LED tx
		IRLEDTX6_R = 1;

		//Wait for capacitor to fire, 10us
		delay(10*firingTime);
		arr[i] = irLedRX6_R.read();
		sum += arr[i];

		//Wait 5us for turning off IR LED tx
		delay(5*firingTime);
		IRLEDTX6_R = 0;

		//Wait 85us for turning on IR LED tx
		delay(85*firingTime);
	}

	float avg = sum/5.0;

	float cmDist = ir::ir_to_dist(avg);
	return cmDist;

}

//has_left_wall, for PID control
bool ir::has_left_wall()
{
	int distance = ir::left_wall_dist();
	if (distance < 12)
		return true;
	else
		return false;
}

//has_right_wall, for PID control
bool ir::has_right_wall()
{
	int distance = ir::right_wall_dist();
	if (distance < 12)
		return true;
	else
		return false;
}
