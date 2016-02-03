#ifndef encoder_H
#define encoder_H


/*
A utility class for encoder management.

Encoder themselves support reset(), getPulses(), getRevolutions() themselves. 512 pulses per revolution

This class provides methods to manage both encoders at once

*/

#include "encoderdriver.h"

Encoder leftEncoder(D7, D8, NC, 512); //+- sign direction not yet known.
Encoder rightEncoder(D9, D10, NC, 512); //+- sign direction not yet known.

/*
Reset both encoders
*/
void resetEncoders(void);

/*
Returns the average number of pulses across both encoders since last reset. (a convenient distance unit)
*/
int getEncoderDistance(void);
