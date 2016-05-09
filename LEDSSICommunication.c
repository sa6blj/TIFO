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
#define BLANK GPIO_PIN_4
#define HIGH 0xff
#define LOW 0

void InitSSI() {
	// Enable SSI module 1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);

	// Enable GPIO peripheral that contains SSI 1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	//HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
	//HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_0;

	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,  GPIO_PIN_3 | GPIO_PIN_4); // MODE, XLAT
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4); // BLANK
	//GPIOPinConfigure(GPIO_PF0_SSI1RX);
	GPIOPinConfigure(GPIO_PF1_SSI1TX);
	GPIOPinConfigure(GPIO_PF2_SSI1CLK);

	GPIOPinTypeSSI(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);


	//SSIClockSourceSet(SYSCTL_PERIPH_SSI1, SSI_CLOCK_PIOSC); //TODO Might need this if the default is not good enough.

	//30 MHz max for LED drivers.
	SSIConfigSetExpClk(SSI1_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000000, 16);

	SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_WRITE);

	SSIEnable(SSI1_BASE);

	GPIOPinWrite(GPIO_PORTF_BASE, (MODE | XLAT), LOW); // Set MODE and XLAT to LOW
	GPIOPinWrite(GPIO_PORTB_BASE, BLANK, LOW); // Set BLANK to LOW
}

/*
 * Each bit turns on or off a LED, 1 = on, 0 = off.
 */
void onOffUpdate(uint32_t data) {
	GPIOPinWrite(GPIO_PORTF_BASE, MODE, LOW); // Set MODE to LOW

	SSIDataPut(SSI1_BASE, data >> 16);
	SSIDataPut(SSI1_BASE, data);

	while(SSIBusy(SSI1_BASE));	// Wait until transfer is done before sending the "done" pulse
	GPIOPinWrite(GPIO_PORTF_BASE, XLAT, HIGH); // Give a XLAT pulse of atleast 10ns
	GPIOPinWrite(GPIO_PORTF_BASE, XLAT, LOW);
}

/*
 * Dimms all LEDs.
 * Accepted values are 0 to 127 with 127 being the brightest.
 */
void dimAllLEDs(uint8_t data) {
	GPIOPinWrite(GPIO_PORTF_BASE, MODE, HIGH);	// Set MODE to HIGH

	data &= 0x7f; 								// Discard highest bit.
	int out = (data << 14) | (data << 7) | data;
	int conversionNumber = 26;					// (26 mod 7 = 5), (24 mod 7 = 3)... conversionArray = {5, 3, 1, 6, 4, 2, 0,(repeat) 5, 3, 1, 6, 4, 2, 0}
	int i;
	for (i = 0; i < 14; ++i) {					// 14x16 = 224 = 7x32
		SSIDataPut(SSI1_BASE, (out >> (conversionNumber%7)));
		conversionNumber -= 2;
	}

	while(SSIBusy(SSI1_BASE));	// Wait until transfer is done before sending the "done" pulse
	GPIOPinWrite(GPIO_PORTF_BASE, XLAT, HIGH); // Give a XLAT pulse of atleast 10ns
	GPIOPinWrite(GPIO_PORTF_BASE, XLAT, LOW);
}

void turnLEDsON() {
	GPIOPinWrite(GPIO_PORTB_BASE, BLANK, LOW); // Set BLANK to LOW
}

void turnLEDsOFF() {
	GPIOPinWrite(GPIO_PORTB_BASE, BLANK, HIGH); // Set BLANK to HIGH
}
