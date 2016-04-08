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

static void initButton();
static void bitSwitchUpdate();
static void onButtonDown();
static void onButtonUp();
static unsigned int readBitSwitch();

static clock_t lastUpdate;
static clock_t currTime;
//static clock_t lastHalfPeriodTime;
//static clock_t timeSinceTurn;
static int dir;
static int16_t speed;
static float lastSpeed;
static float accel;
static float position;
static volatile int running;
static float imageWidth;
static int test;

static clock_t buttonLongPress;
static clock_t lastTurnTime; //FIXME Temporary for setting a static period time
static clock_t halfPeriodTime; //FIXME Temporary for setting a static period time

/*
 * Init function which starts timers and sets the start direction
 */
void inputInterpreterInit() {
	initImageHandler();
	initI2C3();
	initMPU_9150();
	initButton();
	lastUpdate = clock();
	lastSpeed = 0;
	//lastHalfPeriodTime = clock();
	//timeSinceTurn = clock();

	lastTurnTime = clock(); //FIXME Temporary for setting a static period time
	halfPeriodTime = 20; //FIXME Temporary for setting a static period time
	dir = 1;
	speed = 0;
	imageWidth = 5000000;
	test = getXAccel();
	while (abs(getXAccel()) + abs(getZAccel()) > 800){

		SysCtlDelay(2000);
		accelDrawer(getXAccel()+getZAccel());
	}
	position = imageWidth/2;

	setDataReadyHandler(updatePosition);
	setMPU_9150Interrupts(DATA_READY_INT);

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


	//SysCtlDelay(2000);
	//accelDrawer(getZGyro());
	//return;


	//updateFakePosition();	//FIXME Temporary for setting a static period time
	//return;					//FIXME Temporary for setting a static period time
	// Fetch acceleration
	//accel = getXAccel();

	if (running) {
		speed = getZGyro();
		if (abs(getXAccel()) < 400){
			position = imageWidth/2;
		} else {
			position = position + speed*0.4;
		}
		updateImage(position/imageWidth);
	}

	/*
	speed = getZGyro();
	// Update movement since last Update
	position = position + speed;
	// If direction has changed, update direction
	if ((speed > 0) && (dir == 0)){
		dir = 1;
		position = 0;
	} else if ((speed < 0) && (dir == 1)){
		dir = 0;
	}
	if (position > imageWidth){
		imageWidth = position*1.2;
	}
	// Send this position to updateImage function
	updateImage(position/imageWidth);
	*/
}

void initButton() {
	/*
	 * 1 - B5
	 * 2 - D7
	 * 3 - D1 - NEJ!
	 * 4 - D0 - NEJ!
	 * 5 - E4
	 *
	 * Booster pack
	 * S1 - D3
	 * S2 - E1
	 */

	//SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	//while(!(SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)));
	//HWREG(GPIO_PORTD_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
	//HWREG(GPIO_PORTD_BASE+GPIO_O_CR) |= GPIO_PIN_7;
	//HWREG(GPIO_PORTD_BASE+GPIO_O_LOCK) = 0;

	//Set all pins as inputs with a weak pullup resistors
	//GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_5);
	//GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_3);
	//GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1);
	//GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //Disable interrupts
	//GPIOIntDisable(GPIO_PORTB_BASE, GPIO_PIN_5);
	GPIOIntDisable(GPIO_PORTD_BASE, GPIO_PIN_3);
	GPIOIntDisable(GPIO_PORTE_BASE, GPIO_PIN_1);

	//Clear interrupts
	//GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_5);
	GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_3);
	GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_1);

	//Set interrupt handler
	//GPIOIntRegister(GPIO_PORTB_BASE, bitSwitchUpdate);
	GPIOIntRegister(GPIO_PORTD_BASE, onButtonDown);
	GPIOIntRegister(GPIO_PORTE_BASE, onButtonDown);

	//Set the interrupt to trigger on any input change
	//GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_5,	GPIO_BOTH_EDGES);
	GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_3,	GPIO_BOTH_EDGES);
	GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_1,	GPIO_BOTH_EDGES);

	//Enable interrupts
	//GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_5);
	GPIOIntEnable(GPIO_PORTD_BASE, GPIO_PIN_3);
	GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_1);

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
        // PF0 was interrupt cause
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
        // PF0 was interrupt cause
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
    }
}
