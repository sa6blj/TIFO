/*
 * A class for reading the current position
 * of the stick by interpreting the input
 * from the accellerometers and calculation
 * the speed and position.
 */

#include <time.h>
#include <stdint.h>
#include "I2Ccommunication.h"
#include "InputInterpreter.h"
#include "ImageHandler.h"

static int getAccel();

static time_t lastUpdate;
static time_t lastHalfPeriodTime;
static time_t timeSinceTurn;
static int dir;
static float speed;

static time_t lastTurnTime; //FIXME Temporary for setting a static period time
static time_t halfPeriodTime; //FIXME Temporary for setting a static period time

/*
 * Init function which starts timers and sets the start direction
 */
void inputInterpreterInit() {
	initImageHandler();
	InitI2C1();
	lastUpdate = time(0);
	lastHalfPeriodTime = time(0);
	timeSinceTurn = time(0);
	lastTurnTime = time(0); //FIXME Temporary for setting a static period time
	halfPeriodTime = 20; //FIXME Temporary for setting a static period time
	dir = 1;
	speed = 0;
}

/*
 * This function determines the current position of the stick and
 * sends this information to the updateImage function
 */

void updatePosition() {
	SysCtlDelay(200);
	accelDrawer(getAccel());
	return;

	updateFakePosition();	//FIXME Temporary for setting a static period time
	return;					//FIXME Temporary for setting a static period time
	// Fetch acceleration
	float accel = getAccel();

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

int getAccel() {
	return (int)I2CReceive(24, 0x28);
}

void updateFakePosition() {
	float pos = 0;
	while(1) {
		updateImage( dir ? pos : (1-pos) );
		pos += 0.001;
		if (pos > 1) {
			dir = 1-dir;
			pos = 0;
		}
		SysCtlDelay(200);
	}
}
