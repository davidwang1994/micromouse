#include "motordriver.h" //Motor



/*
Drive forward. Speed (actually duty cycle)  -1.0 to 1.0, negative for backwards. 
*/
void drive(float speed){
    if (locked) return 0;
    leftMotor.speed(speed);
    rightMotor.speed(speed);  
    return 1;
}

/*
In place turn with given speed setting. -1.0 to 1.0. Positive is RIGHT. Negative is LEFT.
*/
void turn(float speedAndDirection) {
    if (locked) return 0;
    leftMotor.speed(speed);
    rightMotor.speed(-speed);
    return 1;
}

/*
Turns off the motors to coast without braking. Might work as brake pending testing.
*/
void stop(){
    if (locked){
      locked = 0;
      motorInOperation.detach();
    }
    leftMotor.coast();
    righMotor.coast();
}

/*
Dynamically brakes the motors. Might cause large stall currents. Avoid using pending testing
*/
bool brake(){
    if (locked) return 0;
    leftMotor.stop(0.3);
    rightMotor.stop(0.3);
    return 1;
}

/*
Drives approximately the given distance (encoder pulses) at given speed.
Distance must be positive, speed negative or positive

Does not brake.
Currently only uses encoders to approximate, not PID controlled.

Hogs cpu.
*/
bool drive(int distance, float speed){
    if (locked) return 0;
    resetEncoders();
    drive(speed);
    if (speed > 0){  
        while (getEncoderDistance() < distance);
    }
    else {
        while (getEncoderDistance() > distance);
    }
    return 1;
}

/*
Drives approximately the given distance (encoder pulses) at given speed.
Distance must be positive, speed negative or positive

This version is async and performs an optional callback function on termination.
Only stop() can terminate this prematurely, in which case the callback function is not performed
All other methods to access motor will return false or not perform.

Does not brake.
Currently only uses encoders to approximate, not PID controlled.
*/
void drive(int distance, float speed, void(*callback)(void)){
    if (!locked){
        resetEncoders();
        DriveLock lock (distance, speed, callback);
        motorInOperation.attach_us(&_drive, DriveLock::drive SAMPLE_RATE);
    }
}








/*
#include <Arduino.h>
#include "motors.h"

// input desired force and current speed
static float idealMotorOutput(float force, float velocity) {
  float required_current, back_emf;
  required_current = force / FORCE_PER_AMP;
  back_emf = velocity / VELOCITY_PER_VBEMF;
  return ((required_current * RATED_INTERNAL_RESISTANCE + back_emf) / BATTERY_VOLTAGE);
}
Motor motor_l (MOTOR_A1_PIN, MOTOR_A2_PIN, MOTOR_AP_PIN);
Motor motor_r (MOTOR_B1_PIN, MOTOR_B2_PIN, MOTOR_BP_PIN);

Motor::Motor(int pin1, int pin2, int pwm_pin) {
  pin1_ = pin1;
  pin2_ = pin2;
  pin_pwm_ = pwm_pin;

  pinMode(pin1_, OUTPUT);
  pinMode(pin2_, OUTPUT);
  pinMode(pin_pwm_, OUTPUT);
}

void Motor::Set(float accel, float current_velocity) {
  float force;
  float speed;
  int pin1_state, pin2_state;
  int speed_raw;

  if (current_velocity > 0) {
    force = (ROBOT_MASS * accel + FRICTION_FORCE) / NUMBER_OF_MOTORS;
  } else {
    force = (ROBOT_MASS * accel - FRICTION_FORCE) / NUMBER_OF_MOTORS;
  }

  speed = idealMotorOutput(force, current_velocity);
  speed = constrain(speed, -1, 1);

  speed_raw = abs((int)(round(PWM_SPEED_STEPS * speed)));

  if (speed > 0.0) {
    pin1_state = HIGH;
    pin2_state = LOW;
  } else if (speed < 0.0) {
    pin1_state = LOW;
    pin2_state = HIGH;
  } else {
    pin1_state = LOW;
    pin2_state = LOW;
  }

  digitalWrite(pin1_, pin1_state);
  delay(1000);   // set a one second delay
  digitalWrite(pin2_, pin2_state);
  delay(1000);  // set a one second delay
  analogWrite(pin_pwm_, speed_raw);
}
*/