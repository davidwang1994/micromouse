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



#define PREV_MASK 0x1 //Mask for the previous state in determining direction
//of rotation.
#define CURR_MASK 0x2 //Mask for the current state in determining direction
//of rotation.
#define INVALID   0x3 //XORing two states where both bits have changed.

class Encoder {
public:
    typedef enum Encoding {
        X2_ENCODING,
        X4_ENCODING
    } Encoding;

    /**
     * Reads the current values on channel A and channel B to determine the
     * initial state.
     *
     * Attaches the encode function to the rise/fall interrupt edges of
     * channels A and B to perform X4 encoding.
     *
     * @param channelA mbed pin for channel A input.
     * @param channelB mbed pin for channel B input.
     * @param pulsesPerRev Number of pulses in one revolution.
     * @param encoding The encoding to use. Uses X2 encoding by default. X2
     *                 encoding uses interrupts on the rising and falling edges
     *                 of only channel A where as X4 uses them on both
     *                 channels.
     */
    Encoder(PinName channelA, PinName channelB,  int pulsesPerRev, Encoding encoding = X2_ENCODING);

    /**
     * Reset the encoder.
     *
     * Sets the pulses and revolutions count to zero.
     */
    void reset(void);

    /**
     * Read the number of pulses recorded by the encoder.
     *
     * @return Number of pulses which have occured.
     */
    int getPulses(void);

private:

    /**
     * Update the pulse count.
     *
     * Called on every rising/falling edge of channels A/B.
     *
     * Reads the state of the channels and determines whether a pulse forward
     * or backward has occured, updating the count appropriately.
     */
    void encode(void);

    Encoding encoding_;

    InterruptIn channelA_;
    InterruptIn channelB_;

    int pulsesPerRev_;
    int prevState_;
    int currState_;

    volatile int pulses_;
};



extern Encoder leftEncoder;
extern Encoder rightEncoder;


#endif 