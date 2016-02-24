/*
 * A class for handling a virtual column of LEDs
 * to turn on or off the actual physical outputs
 * to represent this column.
 */
 
#include "OutputInterpreter.h"

/* I threw this part into main.c, revert if I made something stupid :) /J

// Init port for read and write
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

// Init Port A and B as outputs
GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1 | 
									   GPIO_PIN_2 | GPIO_PIN_3 | 
									   GPIO_PIN_4 | GPIO_PIN_5 | 
									   GPIO_PIN_6 | GPIO_PIN_7i);

GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | 
									   GPIO_PIN_2 | GPIO_PIN_3 | 
									   GPIO_PIN_4 | GPIO_PIN_5 | 
									   GPIO_PIN_6 | GPIO_PIN_7);
*/



// I think I got this figured out. Gonna fix the correct port chars and
// pin numbers after talking to V. She wasn't sure today. /J


void updateOutputs(int col) {
	/*
	 * Usage of GPIOPinWrite:
	 *
	 * GPIOPinWrite([Base address to chosen port], ([List of pins to write to]), [Value to print]);
	 *
	 *
	 */
	GPIOPinWrite(GPIO_PORTA_BASE,(GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5),
				(GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7));

}
