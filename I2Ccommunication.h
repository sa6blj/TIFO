#ifndef __I2CCOMMUNICATION_H__
#define __I2CCOMMUNICATION_H__

void InitI2C3(void);
void I2CSend(uint16_t device_address, uint16_t device_register, uint8_t device_data);
int I2CReceive(uint32_t slave_addr, uint8_t start_reg, uint32_t nbrBytes, uint8_t* result);
uint8_t I2CReceiveByte(uint32_t slave_addr, uint8_t reg);
uint16_t I2CReceiveShort(uint32_t slave_addr, uint8_t start_reg);

#endif
