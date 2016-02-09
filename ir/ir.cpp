#include "ir.h"

//Shared 3 digital out for pairwise enable, 2 analog ins for pairwise read
IRSensor leftIR1(D15, A1);
IRSensor leftIR2(D14, A1);
IRSensor leftIR3(D13, A1);
IRSensor rightIR1(D15, A2);
IRSensor rightIR2(D14, A2);
IRSensor rightIR3(D13, A2);


IRSensor::IRSensor(PinName enable, PinName input) : _enable(enable), _input(input){

    _baseline = 0.0f;
    _value = 0.0f;
}


IRSensor::read(){
    
}