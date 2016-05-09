/*
 * A class for reading the current position
 * of the stick by interpreting the input
 * from the accellerometers and calculation
 * the speed and position.
 */

#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/systick.h"
#include "I2Ccommunication.h"
#include "InputInterpreter.h"
#include "ImageHandler.h"
#include "MPU_9150.h"
#include "LEDSSICommunication.h"	//TODO I don't want this here.

static void initButton();
static void bitSwitchUpdate();
static void onButtonDown();
static void onButtonUp();
static unsigned int readBitSwitch();


static int dir;
static int16_t speed;
static float position;
static volatile int running;
static float imageWidth;

static clock_t buttonLongPress;
static int currentDim = 127;

/*
 * Init function which starts timers and sets the start direction
 */
void inputInterpreterInit() {
	initImageHandler();
	initI2C3();
	initMPU_9150();
	initButton();

	dir = 1;
	speed = 0;
	imageWidth = 2000000;


	setAccelConfig(0x10);
	setGyroConfig(0x10);
	setDataReadyHandler(updatePosition);
	setMPU_9150Interrupts(DATA_READY_INT);

	running = true;
}

/*
 * This function determines the current position of the stick and
 * sends this information to the updateImage function
 */

void updatePosition() {
	//resests in endpositions
	if (running) {
		speed = getZGyro(); //get angular velocity

		position = position - speed;//update position

		if ((speed < 0) && (dir == 0)){//changing direction if at enpositions
			dir = 1;
			position = -imageWidth*0.17647; // resets position
		} else if ((speed > 0) && (dir == 1)){
			dir = 0;
			imageWidth = (imageWidth + position*0.85)/2;//calculate new imagewidth based on position
			position = imageWidth*1.17647; //resets position

		}

		// Send this position to updateImage function
		updateImage(position/imageWidth);
	}


}

void initButton() {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	while(!(SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)));
	HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_0;
	HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = 0;

	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_3);
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //Disable interrupts
	GPIOIntDisable(GPIO_PORTD_BASE, GPIO_PIN_3);
	GPIOIntDisable(GPIO_PORTE_BASE, GPIO_PIN_1);
	GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_0);

	//Clear interrupts
	GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_3);
	GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_1);
	GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);

	//Set interrupt handler
	GPIOIntRegister(GPIO_PORTD_BASE, onButtonDown);
	GPIOIntRegister(GPIO_PORTE_BASE, onButtonDown);
	GPIOIntRegister(GPIO_PORTF_BASE, onButtonUp);

	//Set the interrupt to trigger on any input change
	GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_3,	GPIO_BOTH_EDGES);
	GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_1,	GPIO_BOTH_EDGES);
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0,	GPIO_RISING_EDGE);

	//Enable interrupts
	GPIOIntEnable(GPIO_PORTD_BASE, GPIO_PIN_3);
	GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_1);
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0);

}

void updateFakePosition() {
	float pos = 0;
	while(1) {
		while(!running);
		updateImage( dir ? pos : (1-pos) );
		pos += 0.001;
		if (pos > 1) {
			dir = 1-dir;
			pos = 0;
		}
		SysCtlDelay(400);
	}
}

void bitSwitchUpdate() {
	GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_5);
	GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_7);
	GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_4);
	unsigned int apa = readBitSwitch();
}

unsigned int readBitSwitch() {
	unsigned int value = 0;
	value |= GPIO_PORTB_DATA_R & (GPIO_PIN_5 >> 5);
	value |= GPIO_PORTD_DATA_R & (GPIO_PIN_7 >> 6);
	value |= GPIO_PORTD_DATA_R & (GPIO_PIN_1 << 1);
	value |= GPIO_PORTD_DATA_R & (GPIO_PIN_0 << 3);
	value |= GPIO_PORTE_DATA_R & GPIO_PIN_4;
	return value;
}

void onButtonDown() {
    if (GPIOIntStatus(GPIO_PORTD_BASE, false) & GPIO_PIN_3) {
        // PF0 was interrupt cause
    	buttonLongPress = time(0);
        GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_3);  // Clear interrupt flag
        GPIOIntRegister(GPIO_PORTD_BASE, onButtonUp);   // Next interrupt will be button up
        GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_RISING_EDGE);
    } else if (GPIOIntStatus(GPIO_PORTE_BASE, false) & GPIO_PIN_1) {
        // PF0 was interrupt cause
    	buttonLongPress = time(0);
        GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_1);  // Clear interrupt flag
        GPIOIntRegister(GPIO_PORTE_BASE, onButtonUp);   // Next interrupt will be button up
        GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_RISING_EDGE);
    }
}

void onButtonUp() {
    if (GPIOIntStatus(GPIO_PORTD_BASE, false) & GPIO_PIN_3) {
        // PD3 was interrupt cause
    	if (time(0) - buttonLongPress > 1) { // If held for longer than a second it turns of the LEDs
    		updateImage(0);
    		running = false;
    	} else if (!running) {
    		running = true;
    	} else {
    		nextImage();
    	}
		GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_3);  // Clear interrupt flag
		GPIOIntRegister(GPIO_PORTD_BASE, onButtonDown); // Next interrupt will be button down
		GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_FALLING_EDGE);
    } else if (GPIOIntStatus(GPIO_PORTE_BASE, false) & GPIO_PIN_1) {
        // PE1 was interrupt cause
    	if (time(0) - buttonLongPress > 1) { // If held for longer than a second it turns of the LEDs
    		updateImage(0);
    		running = false;
    	} else if (!running) {
    		running = true;
    	} else {
    		previousImage();
    	}
		GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_1);  // Clear interrupt flag
		GPIOIntRegister(GPIO_PORTE_BASE, onButtonDown); // Next interrupt will be button down
		GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_FALLING_EDGE);
    } else if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_0) {
        // PF0 was interrupt cause
    	if (currentDim == 127) {
    		currentDim = 95;
    	} else if (currentDim == 95) {
    		currentDim = 63;
    	} else if (currentDim == 63) {
    		currentDim = 31;
    	} else if (currentDim == 31) {
    		currentDim = 127;
    	}
   		dimAllLEDs(currentDim);

		GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);  // Clear interrupt flag
    }
}
