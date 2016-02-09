#ifndef IR_H
#define IR_H

#include "mbed.h"

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
    
    //Shorthand for read()
    operator float() {
        return read();
    }
}

extern IRSensor leftIR1;
extern IRSensor leftIR2;
extern IRSensor leftIR3;
extern IRSensor rightIR1;
extern IRSensor rightIR2;
extern IRSensor rightIR3;


#endif