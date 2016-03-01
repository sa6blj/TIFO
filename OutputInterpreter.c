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
#define led9  GPIO_PIN_2

// Init port for read and write
void initOutputs(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	//Unlocks D7 which is locked for specific NMI functionality
	while(!(SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)));
	HWREG(GPIO_PORTD_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTD_BASE+GPIO_O_CR) |= GPIO_PIN_7;

	// Init Port A and B as outputs
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);
}

void updateOutputs(unsigned int col) {
	/*
	 * Usage of GPIOPinWrite:
	 *
	 * GPIOPinWrite([Base address to chosen port], ([List of pins to write to]), [Value to print]);
	 */
	/*
	GPIOPinWrite(GPIO_PORTF_BASE,(led0), led0 & (col<<4));							//F
	GPIOPinWrite(GPIO_PORTD_BASE,(led1), led1 & (col<<6));					//D
	GPIOPinWrite(GPIO_PORTD_BASE,(led2), led2 & (col<<4));					//D
	GPIOPinWrite(GPIO_PORTC_BASE,(led3), led3 & (col<<4));				//C
	GPIOPinWrite(GPIO_PORTC_BASE,(led4), led4 & (col<<2));				//C
	GPIOPinWrite(GPIO_PORTC_BASE,(led5), led5 & col);					//C
	GPIOPinWrite(GPIO_PORTC_BASE,(led6), led6 & (col>>2));				//C
	GPIOPinWrite(GPIO_PORTB_BASE,(led7), led7 & (col>>4));			//B
	GPIOPinWrite(GPIO_PORTF_BASE,(led8), led8 & (col>>5));							//F
	GPIOPinWrite(GPIO_PORTF_BASE,(led9), led9 & (col>>7));							//F
	GPIOPinWrite(GPIO_PORTA_BASE,(led10), led10 & (col>>8));	//A
	GPIOPinWrite(GPIO_PORTA_BASE,(led11), led11 & (col>>8));	//A
	GPIOPinWrite(GPIO_PORTA_BASE,(led12), led12 & (col>>8));	//A
	GPIOPinWrite(GPIO_PORTE_BASE,(led13), led13 & (col>>13));					//E
	GPIOPinWrite(GPIO_PORTB_BASE,(led14), led14 & (col>>12));		//B
	GPIOPinWrite(GPIO_PORTA_BASE,(led15), led15 & (col>>10));	//A
	*/

	int aInt = 0, bInt = 0, cInt = 0, dInt = 0, eInt = 0, fInt = 0;

	if(col & 0x1) fInt |= led0;
	if(col & 0x2) dInt |= led1;
	if(col & 0x4) dInt |= led2;
	if(col & 0x8) cInt |= led3;
	if(col & 0x10) cInt |= led4;
	if(col & 0x20) cInt |= led5;
	if(col & 0x40) cInt |= led6;
	if(col & 0x80) bInt |= led7;
	if(col & 0x100) fInt |= led8;
	if(col & 0x200) fInt |= led9;
	if(col & 0x400) aInt |= led10;
	if(col & 0x800) aInt |= led11;
	if(col & 0x1000) aInt |= led12;
	if(col & 0x2000) eInt |= led13;
	if(col & 0x4000) bInt |= led14;
	if(col & 0x8000) aInt |= led15;

	GPIOPinWrite(GPIO_PORTA_BASE,(led10 | led11 | led12 | led15), aInt);
	GPIOPinWrite(GPIO_PORTB_BASE,(led7 | led14), bInt);
	GPIOPinWrite(GPIO_PORTC_BASE,(led3 | led4 | led5 | led6), cInt);
	GPIOPinWrite(GPIO_PORTD_BASE,(led1 | led2), dInt);
	GPIOPinWrite(GPIO_PORTE_BASE,(led13), eInt);
	GPIOPinWrite(GPIO_PORTF_BASE,(led0 | led8 | led9), fInt);
}
