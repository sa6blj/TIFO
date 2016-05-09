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

//CONTROLLER
int main(void) {
	inputInterpreterInit();
	while (1){
		//updatePosition(); //This is called by interrupts from the sensors when they have new data. Look at inputInterpreterInit() for more info.
	}
}



