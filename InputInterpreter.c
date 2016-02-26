/*
 * A class for reading the current position
 * of the stick by interpreting the input
 * from the accellerometers and calculation
 * the speed and position.
 */

#include <time.h>

#include "InputInterpreter.h"
#include "ImageHandler.h"

static float getAccel();
static void updateFakePosition();

static time_t lastUpdate;
static time_t lastHalfPeriodTime;
static time_t timeSinceTurn;
static int dir;
static float speed;

static time_t lastTurnTime; //FIXME Temporary for setting a static period time
static time_t halfPeriodTime; //FIXME Temporary for setting a static period time

void inputInterpreterInit() {
	lastUpdate = time(0);
	lastHalfPeriodTime = time(0);
	timeSinceTurn = time(0);
	lastTurnTime = time(0); //FIXME Temporary for setting a static period time
	halfPeriodTime = 200; //FIXME Temporary for setting a static period time
	dir = 1;
	speed = 0;
}

void updatePosition() {
	updateFakePosition();	//FIXME Temporary for setting a static period time
	return;					//FIXME Temporary for setting a static period time
	float accel = getAccel();
	time_t timeDiff = time(0) - lastUpdate;
	timeSinceTurn += timeDiff;
	speed += accel*timeDiff;
	if (dir && speed < 0) {
		dir = 0;
		lastHalfPeriodTime = timeSinceTurn;
		timeSinceTurn = 0;
	} else if (!dir && speed > 0) {
		dir = 1;
		lastHalfPeriodTime = timeSinceTurn;
		timeSinceTurn = 0;
	}
	float position = timeSinceTurn/lastHalfPeriodTime;
	updateImage( dir ? position : (1-position) );
}

float getAccel() {
	//TODO Implement
	return 0;
}

void updateFakePosition() {
	timeSinceTurn = time(0) - lastTurnTime;
	if (timeSinceTurn >= halfPeriodTime) {
		dir = 1-dir;
		lastTurnTime = time(0);
	}
	float position = timeSinceTurn/halfPeriodTime;
	updateImage( dir ? position : (1-position) );
}
