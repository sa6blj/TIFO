/*
 * Accelerometer.c
 *
 *  Created on: 4 apr 2016
 *      Author: Patrik
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
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
#define INT_PIN_CFG 0x37
#define INT_ENABLE 0x38
#define INT_STATUS 0x3a
#define PWR_MGMT_1 0x6b
#define PWR_MGMT_2 0x6c

static void MPU_9150InterruptHandler();

void (*fifo_overflow_int_handler)(void) = ((void*)0);
void (*I2C_master_int_handler)(void) = ((void*)0);
void (*data_ready_int_handler)(void) = ((void*)0);

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
 * |    0    |      � 2g        |
 * |---------|------------------|
 * |    1    |      � 4g        |
 * |---------|------------------|
 * |    2    |      � 8g        |
 * |---------|------------------|
 * |    3    |      � 16g       |
 * ------------------------------
 */
void setAccelConfig(uint8_t data) {
	I2CSend(BASE_ADDRESS, ACCEL_CONFIG, (0xf8 & data));
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
 * |    0   |     � 250�/s     |
 * |--------|------------------|
 * |    1   |     � 500�/s     |
 * |--------|------------------|
 * |    2   |     � 1000�/s    |
 * |--------|------------------|
 * |    3   |     � 2000�/s    |
 * -----------------------------
 */
void setGyroConfig(uint8_t data) {
	I2CSend(BASE_ADDRESS, GYRO_CONFIG, (0xf8 & data));
	SysCtlDelay(200000);
}

/*
 * The handler given will be called when the fifo buffer overflows.
 */
void setFifoOverflowHandler(void (*handler)(void)) {
	fifo_overflow_int_handler = handler;
}

/*
 * The handler given will be called when a slave to the MPU_9150
 * generates an interrupt.
 */
void setI2CMasterIntHandler(void (*handler)(void)) {
	I2C_master_int_handler = handler;
}

/*
 * The handler given will be called when new data is ready to be read.
 */
void setDataReadyHandler(void (*handler)(void)) {
	data_ready_int_handler = handler;
}

void MPU_9150InterruptHandler() {
	uint8_t int_status = I2CReceiveByte(BASE_ADDRESS, INT_STATUS);
	GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_2);		//Clear interrupt
	if ((fifo_overflow_int_handler!=0) && (int_status & 0x10)) {
		(*fifo_overflow_int_handler)();
	}
	if ((I2C_master_int_handler != 0) && (int_status & 0x08)) {
		(*I2C_master_int_handler)();
	}
	if ((data_ready_int_handler != 0) && (int_status & 0x01)) {
		(*data_ready_int_handler)();
	}
}

/*
 * Enables or disables interrupts for FIFO overflow,
 * I2C master or data ready.
 * Write 1/0 to the appropriate bit to turn on/off interrupts
 * of that kind.
 *
 * ---------------------------------------------------------
 * | bit7 | bit6 | bit5 | bit4 | bit3 | bit2 | bit1 | bit0 |
 * |------|------|------|------|------|------|------|------|
 * |  --  |  --  |  --  | FIFO |  I2C |  --  |  --  | DATA |
 * ---------------------------------------------------------
 *
 *	FIFO - When set to 1, this bit enables a FIFO buffer overflow
 *	to generate an interrupt.
 *
 *	I2C - When set to 1, this bit enables any of the I2C Master
 *	interrupt sources to generate an interrupt.
 *
 *	When set to 1, this bit enables the Data Ready interrupt, which
 *	occurs each time a write operation to all of the sensor registers
 *	has been completed.
 */
void setMPU_9150Interrupts(uint8_t data) {
	I2CSend(BASE_ADDRESS, INT_ENABLE, (0x19 & data));
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

void initMPU_9150() {
	I2CSend(BASE_ADDRESS, PWR_MGMT_1, 0x80);	//Reset the unit
	SysCtlDelay(200000);
	I2CSend(BASE_ADDRESS, PWR_MGMT_1, 0x09);	//Disable thermometer and use the gyro as clock source
	SysCtlDelay(200000);

	//Setup B2 for interrupts
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_2);
	GPIOIntDisable(GPIO_PORTB_BASE, GPIO_PIN_2);				//Disable interrupt
	GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_2);					//Clear interrupt
	GPIOIntRegister(GPIO_PORTB_BASE, MPU_9150InterruptHandler);	//Set interrupt handler
	GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_5,	GPIO_RISING_EDGE);//Set the interrupt to trigger on any input change
	GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_2);					//Enable interrupt
}
