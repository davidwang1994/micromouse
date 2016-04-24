#ifndef IR_H
#define IR_H

#include "mbed.h"

#define COEFF_1 0
#define COEFF_2 0
#define COEFF_3 0
#define COEFF_4 0
#define COEFF_5 0
#define COEFF_6 0
#define COEFF_7 0

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

void read_coeff_values();

class IRSensor {
private:
    DigitalOut _enable;
    AnalogIn _input;
    volatile float value; 
    
public:
    float last_read[5];
    
    IRSensor(PinName enable, PinName input) : _enable(enable), _input(input){}
    
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
        
        sum /= 5.0f;
        float square = sum * sum;
        value = 0;
        value += COEFF_1 * pow(sum, -3);
        value += COEFF_2 * pow(sum, -2);
        value += COEFF_3 / sum;
        value += COEFF_4;
        value += COEFF_5 * sum;
        value += COEFF_6 * square;
        value += COEFF_7 * square * sum;
        return value;
    }

    //Get the number of cells away wall is
    float cell_dist(){
        return read() / 18;
    }

    //Shorthand for read()
    operator float() {
        return read();
    }

};

extern IRSensor rightIR;
extern IRSensor rightDiagonalIR;
extern IRSensor rightFrontIR;
extern IRSensor leftFrontIR;
extern IRSensor leftDiagonalIR;
extern IRSensor leftIR;


#endif