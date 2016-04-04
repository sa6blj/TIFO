/*
 * main.c
 */


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include <stdio.h>

#include "InputInterpreter.h"

char text[32] = {0x00, 0x80, 0x80, 0xff, 0x80, 0x80, 0x00, 		   //T
				0x00, 0x81, 0xff, 0x81, 0x00, 					   //I
				0x00, 0xff, 0x88, 0x88, 0x88, 0x88, 0x80, 0x00,	   //F
				0x00, 0x7e, 0xc3, 0x81, 0x81, 0xc3, 0x7e, 0x00,    //O
				0x00, 0xfb, 0xfb, 0x00} ;						   //!

//CONTROLLER
//freuency
int main(void) {
	//SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //TODO This is probably very important
	inputInterpreterInit();
	while (1){
		updatePosition();
		//sleep(1/frequecy)
	}
}



