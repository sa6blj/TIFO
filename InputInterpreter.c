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

/*
 * Init function which starts timers and sets the start direction
 */

void inputInterpreterInit() {
	lastUpdate = time(0);
	lastPeriodTime = time(0);
	timeSinceTurn = time(0);
	dir = 1;
	speed = 0;
}

/*
 * This function determines the current position of the stick and
 * sends this information to the updateImage function
 */

void updatePosition() {
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
		timeSinceTurn = 0;
	} else if (!dir && speed > 0) {
		dir = 1;
		timeSinceTurn = 0;
	}

	// Calculate what part of the image or message to send to the outputs
	float position = timeSinceTurn/lastPeriodTime;
	// Send this position to updateImage function
	updateImage( dir ? position : (1-position) );
}

/*
 * This function reads a accelerometer value and returns it as a float value.
 */

float getAccel() {
	//TODO Implement
	return 0;
}
