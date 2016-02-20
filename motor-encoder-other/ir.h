/*#ifndef IR_H
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




#endif*/