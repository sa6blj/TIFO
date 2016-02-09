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

static time_t lastUpdate;
static time_t lastPeriodTime;
static time_t timeSinceTurn;
static int dir;
static float speed;

void inputInterpeterInit() {
	lastUpdate = time(0);
	lastPeriodTime = time(0);
	timeSinceTurn = time(0);
	dir = 1;
	speed = 0;
}

void updatePosition() {
	float accel = getAccel();
	timeDiff = time(0) - lastUpdate;
	timeSinceTurn += timeDiff;
	speed += accel*timeDiff;
	if (dir && speed < 0) {
		dir = 0;
		timeSinceTurn = 0;
	} else if (!dir && speed > 0) {
		dir = 1;
		timeSinceTurn = 0;
	}
	float position = timeSinceTurn/lastPeriodTime;
	updateImage( dir ? position : (1-position) );
}

float getAccel() {
	//TODO Implement
}
