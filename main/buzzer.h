#ifndef BUZZER_H
#define BUZZER_H

#include "mbed.h"


//Tone periods (1/frequency)
const float _OFF = 0;
const float _C0 = 1 / 16.35;
const float _Db0 = 1 / 17.32;
const float _D0 = 1 / 18.35;
const float _Eb0 = 1 / 19.45;
const float _E0 = 1 / 20.60;
const float _F0 = 1 / 21.83;
const float _Gb0 = 1 / 23.12;
const float _G0 = 1 / 24.50;
const float _Ab0 = 1 / 25.96;
const float _LA0 = 1 / 27.50;
const float _Bb0 = 1 / 29.14;
const float _B0 = 1 / 30.87;
const float _C1 = 1 / 32.70;
const float _Db1 = 1 / 34.65;
const float _D1 = 1 / 36.71;
const float _Eb1 = 1 / 38.89;
const float _E1 = 1 / 41.20;
const float _F1 = 1 / 43.65;
const float _Gb1 = 1 / 46.25;
const float _G1 = 1 / 49.00;
const float _Ab1 = 1 / 51.91;
const float _LA1 = 1 / 55.00;
const float _Bb1 = 1 / 58.27;
const float _B1 = 1 / 61.74;
const float _C2 = 1 / 65.41;
const float _Db2 = 1 / 69.30;
const float _D2 = 1 / 73.42;
const float _Eb2 = 1 / 77.78;
const float _E2 = 1 / 82.41;
const float _F2 = 1 / 87.31;
const float _Gb2 = 1 / 92.50;
const float _G2 = 1 / 98.00;
const float _Ab2 = 1 / 103.83;
const float _LA2 = 1 / 110.00;
const float _Bb2 = 1 / 116.54;
const float _B2 = 1 / 123.47;
const float _C3 = 1 / 130.81;
const float _Db3 = 1 / 138.59;
const float _D3 = 1 / 146.83;
const float _Eb3 = 1 / 155.56;
const float _E3 = 1 / 164.81;
const float _F3 = 1 / 174.61;
const float _Gb3 = 1 / 185.00;
const float _G3 = 1 / 196.00;
const float _Ab3 = 1 / 207.65;
const float _LA3 = 1 / 220.00;
const float _Bb3 = 1 / 233.08;
const float _B3 = 1 / 246.94;
const float _C4 = 1 / 261.63;
const float _Db4 = 1 / 277.18;
const float _D4 = 1 / 293.66;
const float _Eb4 = 1 / 311.13;
const float _E4 = 1 / 329.63;
const float _F4 = 1 / 349.23;
const float _Gb4 = 1 / 369.99;
const float _G4 = 1 / 392.00;
const float _Ab4 = 1 / 415.30;
const float _LA4 = 1 / 440.00;
const float _Bb4 = 1 / 466.16;
const float _B4 = 1 / 493.88;
const float _C5 = 1 / 523.25;
const float _Db5 = 1 / 554.37;
const float _D5 = 1 / 587.33;
const float _Eb5 = 1 / 622.25;
const float _E5 = 1 / 659.26;
const float _F5 = 1 / 698.46;
const float _Gb5 = 1 / 739.99;
const float _G5 = 1 / 783.99;
const float _Ab5 = 1 / 830.61;
const float _LA5 = 1 / 880.00;
const float _Bb5 = 1 / 932.33;
const float _B5 = 1 / 987.77;
const float _C6 = 1 / 1046.50;
const float _Db6 = 1 / 1108.73;
const float _D6 = 1 / 1174.66;
const float _Eb6 = 1 / 1244.51;
const float _E6 = 1 / 1318.51;
const float _F6 = 1 / 1396.91;
const float _Gb6 = 1 / 1479.98;
const float _G6 = 1 / 1567.98;
const float _Ab6 = 1 / 1661.22;
const float _LA6 = 1 / 1760.00;
const float _Bb6 = 1 / 1864.66;
const float _B6 = 1 / 1975.53;
const float _C7 = 1 / 2093.00;
const float _Db7 = 1 / 2217.46;
const float _D7 = 1 / 2349.32;
const float _Eb7 = 1 / 2489.02;
const float _E7 = 1 / 2637.02;
const float _F7 = 1 / 2793.83;
const float _Gb7 = 1 / 2959.96;
const float _G7 = 1 / 3135.96;
const float _Ab7 = 1 / 3322.44;
const float _LA7 = 1 / 3520.01;
const float _Bb7 = 1 / 3729.31;
const float _B7 = 1 / 3951.07;
const float _C8 = 1 / 4186.01;
const float _Db8 = 1 / 4434.92;
const float _D8 = 1 / 4698.64;
const float _Eb8 = 1 / 4978.03;


class Buzzer {

public:
    Buzzer(PinName pwm_pin): pin(pwm_pin) {
        playing = false;
        volume = 0.2f;
    }
    
    //Sets the volume (duty cycle) of the buzzer. Default 0.005, higher is louder 
    void set_volume(float val);

    //Play the given tone for given duration (in seconds). Blocking.
    void play(float tone, float duration);
    
    //Play the given tone for the given duration. Nonblocking. Playing new tone will immediately overwrite
    void play_async(float tone){
        play_async(tone, 0.4);
    }
    
    //Play the given tone for the given duration. Nonblocking. Playing new tone will immediately overwrite
    void play_async(float tone, float duration);
    
    //Plays the given tone for 0.3 seconds when assigned directly. Nonblocking. Playing new tone within duration will immedaitely overwrite
    void operator=(float tone);
    
private:
    PwmOut pin;    
    
    float volume; //The duty cycle, 0-1
    
    bool playing; //If playing async. False while running play (the blocking one)
    Ticker ticker; //Ticker for async playing
    
    //Stops the async playing
    void stop_playing(){
        playing = false;
        pin.period(0);
        ticker.detach();
    }
};

extern Buzzer buzzer;

#endif
