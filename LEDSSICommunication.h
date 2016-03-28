/*
 * LEDSSICommunication.h
 *
 *  Created on: 27 mar 2016
 *      Author: Patrik
 */

#ifndef LEDSSICOMMUNICATION_H_
#define LEDSSICOMMUNICATION_H_

void InitSSI();
void onOffUpdate(uint32_t data);
void dimAllLEDs(uint8_t data);
void turnLEDsON();
void turnLEDsOFF();

#endif /* LEDSSICOMMUNICATION_H_ */
