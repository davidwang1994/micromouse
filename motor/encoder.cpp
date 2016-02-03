#include "encoderdriver.h" //Encoder

/*
Utility class for interfacing with both encoders
*/



/*
Reset both encoders
*/
public void resetEncoders(){
    leftEncoder.reset();
    rightEncoder.reset();
}


/*
Returns the average number of pulses across both encoders since last reset. (a convenient distance unit)
*/
public int getEncoderDistance(){
    return (leftEncoder.getPulses() + rightEncoder.getPulses()) / 2;
}

