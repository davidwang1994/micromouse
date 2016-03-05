#include "gyro.h"



Timeout gyroTimeOut; //Delay for calibration


//Creates the gyro using given input and calibrates asynchronously it over 2 seconds.
Gyro::Gyro(PinName input): _input(input){
    _isOn = false;
    degrees = 0;
    nullVoltage = 0.7576;
    
    gyroTimeOut.attach(this, &Gyro::calibrate, 1.0); //Delays 1 second before actually calibrating
}

//Resets the gyro measurement to 0 degrees
void Gyro::reset(){
    degrees = 0;
}

//Enables gyro sampling, resetting the measurement to 0
void Gyro::enable(){
    degrees = 0;
    if (!_isOn){
        _isOn = true;
        _gyroTicker.attach(this, &Gyro::sampleWithCallback, GYRO_SAMPLE_PERIOD);
    }
}

void Gyro::enable(void(*per_sample_callback)(void)){
    degrees = 0;
		_per_sample_callback = per_sample_callback;
    if (!_isOn){
        _isOn = true;
        _gyroTicker.attach(this, &Gyro::sampleWithCallback, GYRO_SAMPLE_PERIOD);
    }
}

//Disables gyro sampling
void Gyro::disable(){
    if (_isOn){
        _isOn = false;
        degrees = 0;
        _gyroTicker.detach();
    }
}

//Calibrates the gyro over 1 second, no delay, , setting the null voltage. Takes average of 20 samples.
void Gyro::calibrate(){
    _isOn = true;
    _calibrateCount = 0;
    _gyroTicker.attach(this, &Gyro::_calibrate, 0.05);
}
//Takes 1 sample during calibration, terminating calibration sequence after 20 runs
void Gyro::_calibrate(){
    if (_calibrateCount >= 20){
        _gyroTicker.detach();
        _isOn = false;
    }
    else {
        _calibrateCount++;
        if (_calibrateCount == 1){ //First time
            nullVoltage = _input;
        }
        else { //Otherwise computes the running average by averaging in the difference
            nullVoltage += (_input - nullVoltage) / _calibrateCount;
        }
    }
}

//Measures the current state and computes distance travelled based on past states
void Gyro::sample(){
    degrees += (_input - nullVoltage) * DEGREES_PER_ANALOG_IN_UNIT;
}

void Gyro::sampleWithCallback(){
	  degrees += (_input - nullVoltage) * DEGREES_PER_ANALOG_IN_UNIT;
		_per_sample_callback();
}



