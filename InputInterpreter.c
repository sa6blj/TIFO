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
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "I2Ccommunication.h"
#include "InputInterpreter.h"
#include "ImageHandler.h"

static int getXAccel();
static void initButton();
static void onButtonDown();
static void onButtonUp();

static time_t lastUpdate;
static time_t lastHalfPeriodTime;
static time_t timeSinceTurn;
static int dir;
static float speed;
static int running;

static time_t buttonLongPress;
static time_t lastTurnTime; //FIXME Temporary for setting a static period time
static time_t halfPeriodTime; //FIXME Temporary for setting a static period time

/*
 * Init function which starts timers and sets the start direction
 */
void inputInterpreterInit() {
	initImageHandler();
	InitI2C1();
	initButton();
	lastUpdate = time(0);
	lastHalfPeriodTime = time(0);
	timeSinceTurn = time(0);
	lastTurnTime = time(0); //FIXME Temporary for setting a static period time
	halfPeriodTime = 20; //FIXME Temporary for setting a static period time
	dir = 1;
	speed = 0;

	/*
    //Restarts the accelerometer and enables X-Axis.
    I2CSend(0x18, 0x20, 0x07);
    int read2 = I2CReceive(0x18, 0x20);
    I2CSend(0x18, 0x20, 0x21);
    int read1 = I2CReceive(0x18, 0x0F);
    read2 = I2CReceive(0x18, 0x20);
    */
	running = true;
}

/*
 * This function determines the current position of the stick and
 * sends this information to the updateImage function
 */

void updatePosition() {
	/*
	SysCtlDelay(2000);
	accelDrawer(getXAccel());
	return;
	*/
	updateFakePosition();	//FIXME Temporary for setting a static period time
	return;					//FIXME Temporary for setting a static period time
	// Fetch acceleration
	float accel = getXAccel();

	// Calculate time since updatePosition was last run
	time_t timeDiff = time(0) - lastUpdate;

	// Update how much time has passed since last turn
	timeSinceTurn += timeDiff;

	// Update current speed
	speed += accel*timeDiff;

	// If direction has changed, update direction and reset timeSinceTurn
	if (dir && speed < 0) {
		dir = 0;
		lastHalfPeriodTime = timeSinceTurn;
		timeSinceTurn = 0;
	} else if (!dir && speed > 0) {
		dir = 1;
		lastHalfPeriodTime = timeSinceTurn;
		timeSinceTurn = 0;
	}
	// Calculate what part of the image or message to send to the outputs
	float position = timeSinceTurn/lastHalfPeriodTime;
	// Send this position to updateImage function
	updateImage( dir ? position : (1-position) );
}

/*
 * This function reads a accelerometer value and returns it as a float value.
 */

int getXAccel() {
	/*int data = 0;
	int i=0;
	for (i=0; i<=0x7f; i++) {
		SysCtlDelay(200);
		data = I2CReceive(i, 0x0f);
		if (data != 0) {
			return data;
		}
	}*/
	return I2CReceive(0x18, 0x0f);
	uint32_t accelH, accelL;
	accelL = I2CReceive(0x18, 0x28);
	accelH = I2CReceive(0x18, 0x29);
	return (int)((accelH << 8) | accelL);
}

void initButton() {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	//Unlocks F0 (SW2 button) which is locked for specific NMI functionality
	while(!(SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)));
	HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_0;
	HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = 0;

	//Sets F0 as an input with a weak pullup resistor
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	//Interrupt setup
	GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_0);    //Disable interrupts
	GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);      //Clear interrupts
	GPIOIntRegister(GPIO_PORTF_BASE, onButtonDown); //Set handler
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0,	GPIO_FALLING_EDGE);//Send interrupt when pressed down
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0);     //Enable interrupts
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
		SysCtlDelay(200);
	}
}

void onButtonDown() {
    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_0) {
        // PF0 was interrupt cause
    	buttonLongPress = time(0);
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);  // Clear interrupt flag
        GPIOIntRegister(GPIO_PORTF_BASE, onButtonUp);   // Next interrupt will be button up
        GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_RISING_EDGE);
    }
}

void onButtonUp() {
    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_0) {
        // PF0 was interrupt cause
    	if (time(0) - buttonLongPress > 1) { // If held for longer than a second it turns of the LEDs
    		updateImage(0);
    		running = false;
    	} else if (!running) {
    		running = true;
    	} else {
    		nextImage();
    	}
		GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);  // Clear interrupt flag
		GPIOIntRegister(GPIO_PORTF_BASE, onButtonDown); // Next interrupt will be button down
		GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);
    }
}
