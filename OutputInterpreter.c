/*
 * A class for handling a virtual column of LEDs
 * to turn on or off the actual physical outputs
 * to represent this column.
 */
 
#include "OutputInterpreter.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

/* These pins are part of Port A */
#define led10 GPIO_PIN_2
#define led11 GPIO_PIN_3
#define led12 GPIO_PIN_4
#define led15 GPIO_PIN_5

/* These pins are part of Port B */
#define led7  GPIO_PIN_3
#define led14 GPIO_PIN_2

/* These pins are part of Port C */
#define led3  GPIO_PIN_7
#define led4  GPIO_PIN_6
#define led5  GPIO_PIN_5
#define led6  GPIO_PIN_4

/* These pins are part of Port D */
#define led1  GPIO_PIN_7
#define led2  GPIO_PIN_6

/* These pins are part of Port E */
#define led13 GPIO_PIN_0

/* These pins are part of Port F */
#define led0  GPIO_PIN_4
#define led8  GPIO_PIN_3
#define led9 GPIO_PIN_2

// Init port for read and write
void initOutputs(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	// Init Port A and B as outputs
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);
}

void updateOutputs(int col) {
	/*
	 * Usage of GPIOPinWrite:
	 *
	 * GPIOPinWrite([Base address to chosen port], ([List of pins to write to]), [Value to print]);
	 */
	GPIOPinWrite(GPIO_PORTF_BASE,(led0), led0 & (col<<4));
	GPIOPinWrite(GPIO_PORTD_BASE,(led1), led1 & (col<<6));	//Paj
	GPIOPinWrite(GPIO_PORTD_BASE,(led2), led2 & (col<<4));
	GPIOPinWrite(GPIO_PORTC_BASE,(led3), led3 & (col<<4));
	GPIOPinWrite(GPIO_PORTC_BASE,(led4), led4 & (col<<2));
	GPIOPinWrite(GPIO_PORTC_BASE,(led5), led5 & col);
	GPIOPinWrite(GPIO_PORTC_BASE,(led6), led6 & (col>>2));
	GPIOPinWrite(GPIO_PORTB_BASE,(led7), led7 & (col>>4));
	GPIOPinWrite(GPIO_PORTF_BASE,(led8), led8 & (col>>5));
	GPIOPinWrite(GPIO_PORTF_BASE,(led9), led9 & (col>>7));
	GPIOPinWrite(GPIO_PORTA_BASE,(led10), led10 & (col>>8));
	GPIOPinWrite(GPIO_PORTA_BASE,(led11), led11 & (col>>8));
	GPIOPinWrite(GPIO_PORTA_BASE,(led12), led12 & (col>>8));
	GPIOPinWrite(GPIO_PORTE_BASE,(led13), led13 & (col>>13));
	GPIOPinWrite(GPIO_PORTB_BASE,(led14), led14 & (col>>12));
	GPIOPinWrite(GPIO_PORTA_BASE,(led15), led15 & (col>>10));
/*
	GPIOPinWrite(GPIO_PORTD_BASE,(led1), 0);	//Paj
	GPIOPinWrite(GPIO_PORTD_BASE,(led1), led1);	//Paj

	GPIOPinWrite(GPIO_PORTC_BASE,(led6), 0);	//Paj
	GPIOPinWrite(GPIO_PORTC_BASE,(led6), led6);	//Paj

	GPIOPinWrite(GPIO_PORTF_BASE,(led9), 0);	//Paj
	GPIOPinWrite(GPIO_PORTF_BASE,(led9), led9);	//Paj

	GPIOPinWrite(GPIO_PORTA_BASE,(led10), 0);	//Paj
	GPIOPinWrite(GPIO_PORTA_BASE,(led10), led10);	//Paj

	GPIOPinWrite(GPIO_PORTA_BASE,(led12), 0);	//Paj
	GPIOPinWrite(GPIO_PORTA_BASE,(led12), led12);	//Paj

	GPIOPinWrite(GPIO_PORTB_BASE,(led14), 0);	//Paj
	GPIOPinWrite(GPIO_PORTB_BASE,(led14), led14);	//Paj

	GPIOPinWrite(GPIO_PORTA_BASE,(led15), 0);	//Paj
	GPIOPinWrite(GPIO_PORTA_BASE,(led15), led15);	//Paj
*/
}
