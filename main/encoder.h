#ifndef ENCODER_H
#define ENCODER_H

#include "mbed.h"


/*
Reset both encoders
*/
void resetEncoders();

/*
Returns the average number of pulses across both encoders since last reset. Unit is encoder pulses; intended for straight driving only.
*/
int getEncoderDistance();


class Encoder {
public:

    volatile int pulses;

    Encoder(PinName channelA, PinName channelB);

    //Reset the encoder
    void reset(void);

    //Gets pulses by directly querying the encoder.
    operator int() {
        return pulses;
    }
    
private:
    InterruptIn channelA_;
    InterruptIn channelB_;
    int prevState_;
    int currState_;
    
    void encode(void);
};


extern Encoder leftEncoder;
extern Encoder rightEncoder;


#endif 