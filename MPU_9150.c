/*
 * Accelerometer.c
 *
 *  Created on: 4 apr 2016
 *      Author: Patrik
 */

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "I2Ccommunication.h"

#define BASE_ADDRESS 0x68

#define ACCEL_XOUT_H 0x3b
#define ACCEL_XOUT_L 0x3c
#define ACCEL_YOUT_H 0x3d
#define ACCEL_YOUT_L 0x3e
#define ACCEL_ZOUT_H 0x3f
#define ACCEL_ZOUT_L 0x40
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

#define ACCEL_CONFIG 0x1c
#define PWR_MGMT_1 0x6b
#define PWR_MGMT_2 0x6c

void initAccelerometer() {
	I2CSend(BASE_ADDRESS, PWR_MGMT_1, 0x80);	//Reset the unit
	SysCtlDelay(200000);
	I2CSend(BASE_ADDRESS, PWR_MGMT_1, 0x09);	//Disable thermometer and gyro as clock source
	SysCtlDelay(200000);
	I2CSend(BASE_ADDRESS, PWR_MGMT_2, 0xc0);	//Highest sampling rate
	SysCtlDelay(200000);
	I2CSend(BASE_ADDRESS, ACCEL_CONFIG, 0x00);	//Set sensitivity to +-2g
	SysCtlDelay(200000);
}

/*
 * This function reads the current value of the accelerometers X-axis.
 */
int getXAccel() {
	return ~I2CReceiveShort(BASE_ADDRESS, ACCEL_XOUT_H);
}

/*
 * This function reads the current value of the accelerometers Y-axis.
 */
int getYAccel() {
	return ~I2CReceiveShort(BASE_ADDRESS, ACCEL_YOUT_H);
}

/*
 * This function reads the current value of the accelerometers Z-axis.
 */
int getZAccel() {
	return ~I2CReceiveShort(BASE_ADDRESS, ACCEL_ZOUT_H);
}

/*
 * This function reads the current value of the gyroscopes X-axis.
 */
int getXGyro() {
	return ~I2CReceiveShort(BASE_ADDRESS, GYRO_XOUT_H);
}

/*
 * This function reads the current value of the gyroscopes X-axis.
 */
int getYGyro() {
	return ~I2CReceiveShort(BASE_ADDRESS, GYRO_YOUT_H);
}

/*
 * This function reads the current value of the gyroscopes X-axis.
 */
int getZGyro() {
	return ~I2CReceiveShort(BASE_ADDRESS, GYRO_ZOUT_H);
}
