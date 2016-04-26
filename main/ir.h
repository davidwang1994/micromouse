#ifndef IR_H
#define IR_H

#include "mbed.h"





/*
 * has_front_wall
 * Parameters: none
 * Returns: boolean, if there is a front wall detected or not, for maze 
 */
bool has_front_wall();

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
    float COEFF_1;
    float COEFF_2;
    float COEFF_3;
    float COEFF_4;
    float COEFF_5;
    float COEFF_6;
    float COEFF_7;

    float last_read[5];
    
    IRSensor(PinName enable, PinName input, float c1 = 0.217722944294793f, float c2 = -5.50965509541424f, 
                        float c3 = 55.0111470003071f, float c4 = -229.934241342364f, float c5 = 506.986371752309f, 
                        float c6 = -502.862134567129f, float c7 = 177.712915019135f) : _enable(enable), _input(input), 
                        COEFF_1(c1), COEFF_2(c2), COEFF_3(c3), COEFF_4(c4), COEFF_5(c5), COEFF_6(c6), COEFF_7(c7) {
        timer.start();
    }
    
    //Get the value in encoder units, samples?... 
    float read(){
        
        //If not 2 ms passed, use last value
        if (timer.read_us() <= 2000){
            return value;
        }
        //Otherwise reset the timer(it keeps counting) and get new reading
        timer.reset();
        
        float sum = 0;
        //_enable = 0;
        //_input.read();
        //wait_us(10);
        
        //Each duration takes 100us, 5 times = 0.5ms
        for (int i = 0; i < 5; i++)
        {
            //Turn on IR LED tx
            _enable = 1;
    
            //Wait for capacitor to fire, 10us
            wait_us(6);
            
            last_read[i] = _input.read();
            if (i){
                sum += last_read[i];
            }
            //Wait 5us for turning off IR LED tx
            _enable = 0;
    
            //Wait 85us for turning on IR LED tx
            wait_us(75);
        }
        
        
        sum /= 4;
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