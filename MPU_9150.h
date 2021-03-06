/*
 * Accelerometer.h
 *
 *  Created on: 4 apr 2016
 *      Author: Patrik
 */

#ifndef MPU_9150_H_
#define MPU_9150_H_

#define FIFO_OVERFLOW_INT 0x10
#define I2C_MASTER_INT 0x08
#define DATA_READY_INT 0x01

void setSampleRateDivider(uint8_t data);
void setAccelConfig(uint8_t data);
void setGyroConfig(uint8_t data);
void setFifoOverflowHandler(void (*handler)(void));
void setI2CMasterIntHandler(void (*handler)(void));
void setDataReadyHandler(void (*handler)(void));
void setMPU_9150Interrupts(uint8_t data);
int getXAccel(void);
int getYAccel(void);
int getZAccel(void);
int getXGyro(void);
int getYGyro(void);
int getZGyro(void);
void initMPU_9150(void);

#endif /* MPU_9150_H_ */
