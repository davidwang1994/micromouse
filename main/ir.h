#ifndef IR_H
#define IR_H

#include "mbed.h"



#define COEFF_1 0.0000111242605f
#define COEFF_2 -0.00240366523f
#define COEFF_3 .322318258f
#define COEFF_4 15.4429807f
#define COEFF_5 -46.3568072f
#define COEFF_6 82.2683546f
#define COEFF_7 -54.0198951f

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

class IRSensor {
public:
    DigitalOut _enable;
    AnalogIn _input;
    volatile float value; 
    Timer timer; //The timer to make read use last read value if less than 2 ms passed
    

    float last_read[5];
    
    IRSensor(PinName enable, PinName input) : _enable(enable), _input(input){
        //timer.start();
    }
    
    //Get the value in encoder units, samples?... 
    float read(){
        /*
        //If not 2 ms passed, use last value
        if (timer.read_us() <= 2000){
            return value;
        }
        //Otherwise reset the timer(it keeps counting) and get new reading
        timer.reset();
        */
        
        float sum = 0;
    
        //Each duration takes 100us, 5 times = 0.5ms
        for (int i = 0; i < 5; i++)
        {
            //Turn on IR LED tx
            _enable = 1;
    
            //Wait for capacitor to fire, 10us
            wait_us(10);
            //last_read[i] = _input;
            sum += _input.read();
    
            //Wait 5us for turning off IR LED tx
            wait_us(5);
            _enable = 0;
    
            //Wait 85us for turning on IR LED tx
            wait_us(85);
        }
        
        
        sum /= 5;
        value = sum;
        
        float square = sum * sum;
        float cube = square * sum;
        float _value = 0;
        _value += COEFF_1 / cube;
        _value += COEFF_2 / square;
        _value += COEFF_3 / sum;
        _value += COEFF_4;
        _value += COEFF_5 * sum;
        _value += COEFF_6 * square;
        _value += COEFF_7 * square * sum;
        return _value;
        
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