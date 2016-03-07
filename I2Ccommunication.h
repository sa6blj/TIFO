#ifndef __I2CCOMMUNICATION_H__
#define __I2CCOMMUNICATION_H__

void InitI2C1(void);
void I2CSend(uint16_t device_address, uint16_t device_register, uint8_t device_data);
uint32_t I2CReceive(uint32_t slave_addr, uint8_t reg);

#endif
