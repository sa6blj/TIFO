/*
 * LEDSSICommunication.c
 *
 *  Created on: 26 mar 2016
 *      Author: Patrik
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_ssi.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "LEDSSICommunication.h"

#define MODE GPIO_PIN_3
#define XLAT GPIO_PIN_4
#define BLANK GPIO_PIN_3
#define HIGH 0xff
#define LOW 0

void InitSSI() {
	// Enable GPIO peripheral that contains SSI 2
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3 | GPIO_PIN_4); // MODE and XLAT
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3); // BLANK
	GPIOPinConfigure(GPIO_PB4_SSI2CLK);
	GPIOPinConfigure(GPIO_PB6_SSI2RX);
	GPIOPinConfigure(GPIO_PB7_SSI2TX);

	GPIOPinTypeSSI(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7);

	// Enable SSI module 2
	SysCtlPeripheralDisable(SYSCTL_PERIPH_SSI2);
	SysCtlPeripheralReset(SYSCTL_PERIPH_SSI2);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);

	//SSIClockSourceSet(SYSCTL_PERIPH_SSI2, SSI_CLOCK_PIOSC); //TODO Might need this if the default is not good enough.

	//30 MHz max for LED drivers.
	SSIConfigSetExpClk(SYSCTL_PERIPH_SSI2, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 30000000, 16); //FIXME MOTO_MODE might need experimentation with

	SSIAdvModeSet(SYSCTL_PERIPH_SSI2, SSI_ADV_MODE_WRITE); //FIXME Might not be supported.

	SSIEnable(SYSCTL_PERIPH_SSI2);

	GPIOPinWrite(GPIO_PORTB_BASE, (MODE | XLAT), LOW); // Set MODE and XLAT to LOW
	GPIOPinWrite(GPIO_PORTF_BASE, BLANK, LOW); // Set BLANK to LOW
}

/*
 * Each bit turns on or off a LED, 1 = on, 0 = off.
 */
void onOffUpdate(uint32_t data) {
	GPIOPinWrite(GPIO_PORTB_BASE, MODE, LOW); // Set MODE to LOW

	SSIDataPut(SYSCTL_PERIPH_SSI2, data >> 16); //FIXME Might need to flip these
	SSIDataPut(SYSCTL_PERIPH_SSI2, data);		//FIXME Might need to flip these

	GPIOPinWrite(GPIO_PORTB_BASE, XLAT, HIGH); // Give a XLAT pulse of atleast 10ns
	GPIOPinWrite(GPIO_PORTB_BASE, XLAT, LOW);
}

/*
 * Dimms all LEDs.
 * Accepted values are 0 to 127 with 127 being the brightest.
 */
void dimAllLEDs(uint8_t data) {
	SSIConfigSetExpClk(SYSCTL_PERIPH_SSI2, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 30000000, 7); //FIXME MOTO_MODE might need experimentation with

	GPIOPinWrite(GPIO_PORTB_BASE, MODE, HIGH); // Set MODE to HIGH

	int i=0;
	for (; i<32; i++) {
		SSIDataPut(SYSCTL_PERIPH_SSI2, data);
	}

	GPIOPinWrite(GPIO_PORTB_BASE, XLAT, HIGH); // Give a XLAT pulse of atleast 10ns
	GPIOPinWrite(GPIO_PORTB_BASE, XLAT, LOW);

	SSIConfigSetExpClk(SYSCTL_PERIPH_SSI2, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 30000000, 16); //FIXME MOTO_MODE might need experimentation with
}

void turnLEDsON() {
	GPIOPinWrite(GPIO_PORTF_BASE, BLANK, LOW); // Set BLANK to LOW
}

void turnLEDsOFF() {
	GPIOPinWrite(GPIO_PORTF_BASE, BLANK, HIGH); // Set BLANK to HIGH
}
