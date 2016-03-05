
#include "mbed.h"


//Analog gyro class written from scratch. Since temperature is likely stable, it is not compensated for. 
//Intended for relative turning only - not filtered with accelerometer to use as IMU.


//Converting the sensitivity value into degrees per measured unit based on the sample rate here. 
#define RAW_SENSITIVITY                 6  //mV/degree/sec, as per data sheet. Pending exact test for our chip if possible, though likely not necessary
#define SENSITIVITY                     RAW_SENSITIVITY / 3.3 //(AnalogIn unit*1000)/degree/sec, converted range from 0-3.3v to 0-1. Pending test if possible
#define GYRO_SAMPLE_RATE                200  //Hz
#define DEGREES_PER_ANALOG_IN_UNIT      1000 / SENSITIVITY * GYRO_SAMPLE_RATE //Degrees per analog unit to directly multiply measurement with

const timestamp_t GYRO_SAMPLE_PERIOD = 1.0 / GYRO_SAMPLE_RATE;  


class Gyro {
public:
    float degrees; //degrees turned since last reset
    float nullVoltage; //Baseline voltage to subtract for each measurement0-1.0, as calibrated.
    
    Gyro(PinName input);
    
    //Calibrates the gyro, setting the nullVoltage.
    void calibrate(void);
    
    //Resets the current degrees value
    void reset(void);
    
    //Enables gyro sampling, reseting degrees to 0
    void enable(void);

		//Enables the gyro and sets the given function (for PID) to run on each sample fo the gyro
		void enable(void(*per_sample_callback)(void));

    //Disables gyro sampling. 
    void disable(void);

    //Gets degree measurement by directly querying the gyro (eg. float turnAngle = gyro;)
    operator float(){
        return degrees;
    }
    
private:
    Ticker _gyroTicker; //To handle sampling
    
    AnalogIn _input; 
    bool _isOn; //Enabled or disabled
    int _calibrateCount; //Number of samples already performed by calibration    
    
		void (*_per_sample_callback)(void); //Callback function passed into enable(callback)

    //Samples the gyro
    void sample(void);

		//Samples, calling _per_sample_callback everytime
		void sampleWithCallback(void);
    
    //Performs calibration, called by ticker.
    void _calibrate();
};

extern Gyro gyro; //The gyro sensor

