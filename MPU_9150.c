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

#define GYRO_CONFIG 0x1b
#define ACCEL_CONFIG 0x1c
#define PWR_MGMT_1 0x6b
#define PWR_MGMT_2 0x6c

void initAccelerometer() {
	I2CReceiveShort(BASE_ADDRESS, PWR_MGMT_1);
	I2CSend(BASE_ADDRESS, PWR_MGMT_1, 0x80);	//Reset the unit
	SysCtlDelay(200000);
	I2CSend(BASE_ADDRESS, PWR_MGMT_1, 0x09);	//Disable thermometer and gyro as clock source
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

/*
 * Sets selftest and/or sensitivity of the Accelerometer.
 *
 * ---------------------------------------------------------
 * | bit7 | bit6 | bit5 | bit4 | bit3 | bit2 | bit1 | bit0 |
 * |------|------|------|------|------|------|------|------|
 * | XA_ST| YA_ST| ZA_ST| AFS_SEL[1:0]|  --  |  --  |  --  |
 * ---------------------------------------------------------
 *
 * XA_ST, YA_ST, ZA_ST is selftest enabled for x-, y- and z-axis
 * respectively.
 *
 * AFS_SEL selects the full scale range of the accelerometer outputs
 * according to the following table.
 *
 * -----------------------------
 * | AFS_SEL | Full Scale Range |
 * |---------|------------------|
 * |    0    |      ± 2g        |
 * |---------|------------------|
 * |    1    |      ± 4g        |
 * |---------|------------------|
 * |    2    |      ± 8g        |
 * |---------|------------------|
 * |    3    |      ± 16g       |
 * ------------------------------
 */
void setAccelConfig(uint8_t data) {
	I2CSend(BASE_ADDRESS, ACCEL_CONFIG, data);
	SysCtlDelay(200000);
}

/*
 * Sets selftest and/or sensitivity of the Gyroscope.
 *
 * ---------------------------------------------------------
 * | bit7 | bit6 | bit5 | bit4 | bit3 | bit2 | bit1 | bit0 |
 * |------|------|------|------|------|------|------|------|
 * | XG_ST| YG_ST| ZG_ST| FS_SEL[1:0] |  --  |  --  |  --  |
 * ---------------------------------------------------------
 *
 * XG_ST, YG_ST, ZG_ST is selftest enabled for x-, y- and z-axis
 * respectively.
 *
 * FS_SEL selects the full scale range of the gyroscope outputs
 * according to the following table.
 *
 * -----------------------------
 * | FS_SEL | Full Scale Range |
 * |--------|------------------|
 * |    0   |     ± 250°/s     |
 * |--------|------------------|
 * |    1   |     ± 500°/s     |
 * |--------|------------------|
 * |    2   |     ± 1000°/s    |
 * |--------|------------------|
 * |    3   |     ± 2000°/s    |
 * -----------------------------
 */
void setGyroConfig(uint8_t data) {
	I2CSend(BASE_ADDRESS, GYRO_CONFIG, data);
	SysCtlDelay(200000);
}
