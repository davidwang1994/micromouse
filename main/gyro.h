#ifndef __GYRO_H__
#define __GYRO_H__

#include "mbed.h"


//Converting the sensitivity value into degrees per measured unit based on the sample rate here.
#define RAW_SENSITIVITY                 6  //mV/degree/sec, as per data sheet. Pending exact test for our chip if possible, though likely not necessary
#define SENSITIVITY                     RAW_SENSITIVITY / 3.3 //(AnalogIn unit*1000)/degree/sec, converted range from 0-3.3v to 0-1. Pending test if possible
#define GYRO_SAMPLE_RATE                200  //Hz
const float DEGREES_PER_ANALOG_IN_UNIT = 1000 / SENSITIVITY * GYRO_SAMPLE_RATE; //Degrees per analog unit to directly multiply measurement with
const timestamp_t GYRO_SAMPLE_PERIOD = 1.0 / GYRO_SAMPLE_RATE;  


class Gyro {
public:
    
    Gyro(PinName input, PinName ground);
    
    //Resets the current degrees value
    void reset(void);
    
    //Enables gyro sampling, reseting degrees to 0
    void enable(void);
    
    //Disables gyro sampling. 
    void disable(void);
    
    //Gets degree measurement by directly querying the gyro (eg. float turnAngle = gyro;)
    operator float(){
        return degrees;
    }
    
private:
    Ticker _gyroTicker; //To handle sampling
    volatile float degrees; //current angular position since last reset
    AnalogIn _input;
    AnalogIn _ground;  
    bool _isOn; //Enabled or disabled
    
    //Samples the gyro
    void sample(void);
};


extern Gyro gyro; //The gyro sensor

#endif
