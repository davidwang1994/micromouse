#include "buzzer.h"

//Play the given tone for given duration (in seconds). Blocking.
void Buzzer::play(float tone, float duration){
    if (playing){
        ticker.detach();
        playing = false;
    }
    pin.period(tone);
    pin.write(volume); // 50% duty cycle
    wait(duration); 
    pin.period(0); // Sound off
}

//Play the given tone for the given duration. Nonblocking. Playing new tone will immediately overwrite
void Buzzer::play_async(float tone, float duration){
    if (playing){
        ticker.detach();
    }
    playing = true;
    pin.period(tone);
    pin.write(volume); // 50% duty cycle
    ticker.attach(this, &Buzzer::stop_playing, duration);
}

//Plays the given tone for 0.3 seconds when assigned directly. Nonblocking. Playing new tone within duration will immedaitely overwrite
void Buzzer::operator=(float tone){
    play_async(tone, 0.3);
}

//Sets the volume (duty cycle) of the buzzer. Default 0.005, higher is louder
void Buzzer::set_volume(float val){
    volume = val;
}