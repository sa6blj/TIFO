/*
 * Accelerometer.h
 *
 *  Created on: 4 apr 2016
 *      Author: Patrik
 */

#ifndef MPU_9150_H_
#define MPU_9150_H_

void initAccelerometer(void);
int getXAccel(void);
int getYAccel(void);
int getZAccel(void);
int getXGyro(void);
int getYGyro(void);
int getZGyro(void);
void setAccelConfig(uint8_t data);

#endif /* MPU_9150_H_ */
