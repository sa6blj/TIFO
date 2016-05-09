/*
 * A class for handling a virtual column of LEDs
 * to turn on or off the actual physical outputs
 * to represent this column.
 */

#include <stdint.h>
#include "LEDSSICommunication.h"

void initOutputs(void){
	InitSSI();
}

void updateOutputs(unsigned int col) {
	onOffUpdate(col);
}
