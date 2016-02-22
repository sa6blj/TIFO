/*
 * A class for handling a virtual column of LEDs
 * to turn on or off the actual physical outputs
 * to represent this column.
 */
 
#include "OutputInterpreter.h"

#define LEDS_OUT <insert memory adress here>

// Init port for read and write
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

// Init Port A and B as outputs
GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1 | 
									   GPIO_PIN_2 | GPIO_PIN_3 | 
									   GPIO_PIN_4 | GPIO_PIN_5 | 
									   GPIO_PIN_6 | GPIO_PIN_7);

GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | 
									   GPIO_PIN_2 | GPIO_PIN_3 | 
									   GPIO_PIN_4 | GPIO_PIN_5 | 
									   GPIO_PIN_6 | GPIO_PIN_7);

void updateOutputs(int col) {
	/*
	 * void GPIOPinWrite (uint32_t ui32Port, uint)
	 *
	 *
	 */
GPIOPinWrite(GPIO_PORTA_BASE,
(GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3 |
GPIO_PIN_4 | GPIO_PIN_5),
(GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 |
GPIO_PIN_7));


}
