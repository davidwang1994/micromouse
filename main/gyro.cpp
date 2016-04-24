

#include "gyro.h"


//Creates the gyro using given input and calibrates asynchronously it over 2 seconds.
Gyro::Gyro(PinName input, PinName ground): _input(input), _ground(ground){
    _isOn = false;
    degrees = 0;
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
        _gyroTicker.attach(this, &Gyro::sample, GYRO_SAMPLE_PERIOD);
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

//Measures the current state and computes distance travelled based on past states
void Gyro::sample(){
    degrees += (_input - _ground) * DEGREES_PER_ANALOG_IN_UNIT;
}


