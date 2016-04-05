#ifndef IR_H
#define IR_H

#include "mbed.h"

#define IR_SAMPLE_PERIOD 0.001 //once per ms when enabled 
#define CALIBRATE_SAMPLE_COUNT 20 //Average of 20 

//Has wall directly forwards/left/right of mouse (wall is less than 1 cell away). Does not require to be at center of cell, just has to be within it and not too close to walls.
bool has_wall();
bool has_left_wall(); 
bool has_right_wall();

//Has a wall this many cells away (0 for immediately in this cell)
int wall_distance();
int left_wall_distance();
int right_wall_distance();

//Forward wall is this many encoder units away (averages the 2 sensors). Possibly used for controlling drive.
int forwards_distance();

//Enables sampling at given period for front and side IRSensors
void enableIR();

//Units of IRSensor read is linearized distance away in encoder units
class IRSensor {
	public:
   	DigitalOut _enable;
    AnalogIn _input;
   
    volatile float raw_value; //Currently read value
    float _baseline; //Ambient value
    
    IRSensor(PinName enable, PinName input);
    
    //Enables sampling for this IRSensor. Give it 
    void enable();
    
    //Disables sampling for this IRSensor
    void disable();
    
    //Calibrate it (set baseline). Give it ~25 ms to run before reading. Currently called on init
    void calibrate();
    
    //Get the value in encoder units, samples?... 
    float read();
    
    //Shorthand for read()
    operator float() {
        return read();
    }
private:
		bool _isOn;
		Ticker _ir_ticker;
        
        //Samples and stores the raw value once
		void _sample();
        
        //Calibration internal content
        int _calibrate_count;
        float _calibrate_samples[CALIBRATE_SAMPLE_COUNT];
        void _calibrate();
};

extern IRSensor leftIR1;
extern IRSensor leftIR2;
extern IRSensor leftIR3;
extern IRSensor rightIR1;
extern IRSensor rightIR2;
extern IRSensor rightIR3;


#endif

