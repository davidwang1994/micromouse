#ifndef IR_H
#define IR_H

#include "mbed.h"

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


class IRSensor {
	private:
    	float last_read[5];
    	
	public:
   	DigitalOut _enable;
    AnalogIn _input;
   
    volatile float raw_value; //Currently read value
    float _baseline; //Ambient value
    
    IRSensor::IRSensor(PinName enable, PinName input) : _enable(enable), _input(input){}
    
    //Get the value in encoder units, samples?... 
    float read(){
	    	
		float sum = 0;
	
		//Each duration takes 100us, 5 times = 0.5ms
		for (int i = 0; i < 5; i++)
		{
			//Turn on IR LED tx
			_enable = 1;
	
			//Wait for capacitor to fire, 10us
			wait_us(10);
			last_read[i] = _input;
			sum += _input;
	
			//Wait 5us for turning off IR LED tx
			wait_us(5);
			_enable = 0;
	
			//Wait 85us for turning on IR LED tx
			wait_us(85);
		}
			
		return ir_to_dist(sum/5.0f);
    }
    
    float[] get_last_readings(){
    	return last_read;
    }
    
    //Shorthand for read()
    operator float() {
        return read();
    }

};

extern IRSensor IRLED6_R;
extern IRSensor IRLED5_RD;
extern IRSensor IRLED4_RF;
extern IRSensor IRLED3_LF;
extern IRSensor IRLED2_LD;
extern IRSensor IRLED1_L;





#endif
