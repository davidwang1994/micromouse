#include "ir.h"




int forwards_distance(){
		return (leftIR1 + rightIR1) / 2;
}

void enableIR(){
		leftIR1.enable();
		leftIR3.enable();
		rightIR1.enable();
		rightIR3.enable();
}

IRSensor::IRSensor(PinName enable, PinName input) : _enable(enable), _input(input){
    _baseline = 0.0f;
    raw_value = 0.0f;
		_isOn = false;
		
		calibrate();
	
}
void IRSensor::_sample(){
	  raw_value = _input;
}

void IRSensor::enable(){
		if (!_isOn){
				_enable = 1;
				_isOn = true;
				_ir_ticker.attach(this, &IRSensor::_sample, IR_SAMPLE_PERIOD);
		}
}

void IRSensor::disable(){
		if (_isOn){
				_enable = 0;
				_isOn = false;
				_ir_ticker.detach();
		}
}

void IRSensor::calibrate(){
		disable();	
		_enable = 1;
		_calibrate_count = -1;
		_ir_ticker.attach(this, &IRSensor::_calibrate, IR_SAMPLE_PERIOD);
}

void IRSensor::_calibrate(){
		if (_calibrate_count < 0){
				//Toss first sample
		}
		else if (_calibrate_count == CALIBRATE_SAMPLE_COUNT){
				disable();
				float sum = 0;
				for (int i = 0; i < CALIBRATE_SAMPLE_COUNT; i++){
						sum += _calibrate_samples[i];
				}
				_baseline = sum / CALIBRATE_SAMPLE_COUNT;
		}
		else {
				_calibrate_samples[_calibrate_count] = _input;
		}
		_calibrate_count++;
}

//TODO: convert to distance using constants to be tested
float IRSensor::read(){
		
	
		
		return raw_value;
}




