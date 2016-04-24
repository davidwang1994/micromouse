#include "encoder.h"



/*
Reset both encoders
*/
void resetEncoders(){
    leftEncoder.reset();
    rightEncoder.reset();
}

/*
Returns the average number of pulses across both encoders since last reset. Unit is encoder pulses; intended for straight driving only.
*/
int getEncoderDistance(){
    return (leftEncoder + rightEncoder) / 2;
}



/*
 * Represents a quadrature motor encoder. Modified from mbed QEI 
 */
Encoder::Encoder(PinName channelA, PinName channelB) : 
    channelA_(channelA), 
    channelB_(channelB){
    pulses = 0;

    //Workout what the current state is.
    int chanA = channelA_.read();
    int chanB = channelB_.read();

    //2-bit state.
    currState_ = (chanA << 1) | (chanB);
    prevState_ = currState_;

    channelA_.rise(this, &Encoder::encode);
    channelA_.fall(this, &Encoder::encode);
}

//Internally updates the pulse count for the encoder.
void Encoder::encode(void) {
    int chanA  = channelA_.read();
    int chanB  = channelB_.read();

    //2-bit state.
    currState_ = (chanA << 1) | (chanB);

    //11->00->11->00 is counter clockwise rotation or "forward".
    if ((prevState_ == 0x3 && currState_ == 0x0) ||
            (prevState_ == 0x0 && currState_ == 0x3)) {
        pulses++;
    }
    //10->01->10->01 is clockwise rotation or "backward".
    else if ((prevState_ == 0x2 && currState_ == 0x1) ||
             (prevState_ == 0x1 && currState_ == 0x2)) {
        pulses--;
    }
    
    prevState_ = currState_;
}

//Resets the encoder
void Encoder::reset(void) {
    pulses = 0;
}
