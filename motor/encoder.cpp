#include "encoder.h"


//Defines the 2 encoders in use
Encoder leftEncoder(D8, D7, 512);
Encoder rightEncoder(D10, D9, 512);

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
    return (leftEncoder.getPulses() + rightEncoder.getPulses()) / 2;
}



/*
 * Represents a quadrature motor encoder. Modified from mbed QEI 
 */
Encoder::Encoder(PinName channelA,
         PinName channelB,
         int pulsesPerRev,
         Encoding encoding) : channelA_(channelA), channelB_(channelB){

    pulses_       = 0;
    pulsesPerRev_ = pulsesPerRev;
    encoding_     = encoding;

    //Workout what the current state is.
    int chanA = channelA_.read();
    int chanB = channelB_.read();

    //2-bit state.
    currState_ = (chanA << 1) | (chanB);
    prevState_ = currState_;

    //X2 encoding uses interrupts on only channel A.
    //X4 encoding uses interrupts on      channel A,
    //and on channel B.
    channelA_.rise(this, &Encoder::encode);
    channelA_.fall(this, &Encoder::encode);

    //If we're using X4 encoding, then attach interrupts to channel B too.
    if (encoding == X4_ENCODING) {
        channelB_.rise(this, &Encoder::encode);
        channelB_.fall(this, &Encoder::encode);
    }
}

//Internally updates the pulse count for the encoder.
void Encoder::encode(void) {
    int change = 0;
    int chanA  = channelA_.read();
    int chanB  = channelB_.read();

    //2-bit state.
    currState_ = (chanA << 1) | (chanB);

    if (encoding_ == X2_ENCODING) {
        //11->00->11->00 is counter clockwise rotation or "forward".
        if ((prevState_ == 0x3 && currState_ == 0x0) ||
                (prevState_ == 0x0 && currState_ == 0x3)) {
            pulses_++;
        }
        //10->01->10->01 is clockwise rotation or "backward".
        else if ((prevState_ == 0x2 && currState_ == 0x1) ||
                 (prevState_ == 0x1 && currState_ == 0x2)) {
            pulses_--;
        }
    } else if (encoding_ == X4_ENCODING) {
        //Entered a new valid state.
        if (((currState_ ^ prevState_) != INVALID) && (currState_ != prevState_)) {
            //2 bit state. Right hand bit of prev XOR left hand bit of current
            //gives 0 if clockwise rotation and 1 if counter clockwise rotation.
            change = (prevState_ & PREV_MASK) ^ ((currState_ & CURR_MASK) >> 1);
            if (change == 0) {
                change = -1;
            }
            pulses_ -= change;
        }
    }
    prevState_ = currState_;
}

void Encoder::reset(void) {
    pulses_      = 0;
}

int Encoder::getPulses(void) {
    return pulses_;
}

