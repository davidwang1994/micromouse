#ifndef __IR_H__
#define __IR_H__

#include "mbed.h"
#include "pin_assignments.h"

const float SENSOR_THRESHOLD = 0.5;

class IRSensor {
    DigitalOut _enable;
    AnalogIn _input;
    
    volatile float _value; //Currently read value
    float _baseline; //Ambient value
    
    IRSensor(PinName enable, PinName input);
    
    
    //Calibrate it (set baseline)
    void calibrate();
    
    
    //Get the value (value - baseline)
    float read();

/
    //Shorthand for read()
    operator float() {
        return read();
    }
};

/** set the walls of the current cell by using the IR sensors **/
    bool has_front_wall() {
        if (frontIR1 > SENSOR_THRESHOLD && frontIR2 > SENSOR_THRESHOLD) {
            return true;
        }
        return false;
    }

    bool has_left_wall() {
        if (leftIR > SENSOR_THRESHOLD) {
            return true;
        }
        return false;
    }

    bool has_right_wall() {
        if (rightIR > SENSOR_THRESHOLD) {
            return true;
        }
        return false;
    }

#endif