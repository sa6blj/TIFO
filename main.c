/*
 * main.c
 *
 * Comment: Moved example code to ./Examples/main.c because it felt stupid to keep it here /J
 *          Also commented on most stuff.
 */

#include <stdio.h>
#include "InputInterpreter.h"

// We will probably need to include some libraries for the microcontroller, right? (To access the API functions) /J



/*
 * Inits
 *
 * Initializations for the program are made here
 */

// This is the default message. Will probably only be used for debugging and testing. 
char msg[32] = {0x00, 0x80, 0x80, 0xff, 0x80, 0x80, 0x00,          //T
                 0x00, 0x81, 0xff, 0x81, 0x00,                      //I
                 0x00, 0xff, 0x88, 0x88, 0x88, 0x88, 0x80, 0x00,    //F
                 0x00, 0x7e, 0xc3, 0x81, 0x81, 0xc3, 0x7e, 0x00,    //O
                 0x00, 0xfb, 0xfb, 0x00} ;                          //!



// I just put these here. Maybe they will go into some cool init function later? /J

// These statements inits the GPIO ports on the dev board
// For more info, see the TI API document.
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

// These statements inits the specific IO-pins and enables them as outputs.
// For more info, see the TI API document.
GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                       GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                       GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);




/* 
 * This function runs the main program
 *
 */

int main(void) {

	// Initiates the accelerometer inputs
        inputInterpreterInit();

	// Main loop
        while (1){

		// Makes all the magic happen!
                updatePosition();

                //sleep(1/frequecy)
        }
}

