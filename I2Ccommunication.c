/*
 * I2Ccommunication.c
 * Functions for I2C communication with the accelerometer
 *  Created on: 17 feb 2016
 *
 */
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"



//initialize I2C module 3 D0&D1
void initI2C3(void)
{
    //enable GPIO peripheral that contains I2C 3
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    // Configure the pins
    GPIOPinConfigure(GPIO_PD0_I2C3SCL);
    GPIOPinConfigure(GPIO_PD1_I2C3SDA);

    // Select the I2C function for these pins.
    GPIOPinTypeI2CSCL(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinTypeI2C(GPIO_PORTD_BASE, GPIO_PIN_1);

    //enable I2C module 3
    SysCtlPeripheralDisable(SYSCTL_PERIPH_I2C3);
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C3);

    // Enable and initialize the I2C3 master module.  Use the system clock for
    // the I2C3 module.  The last parameter sets the I2C data transfer rate.
    // If false the data rate is set to 100kbps and if true the data rate will
    // be set to 400kbps.
    I2CMasterInitExpClk(I2C3_BASE, SysCtlClockGet(), true);

    //clear I2C FIFOs
    HWREG(I2C3_BASE + I2C_O_FIFOCTL) = 80008000;
}

//sends an I2C command to the specified slave
void I2CSend(uint16_t device_address, uint16_t device_register, uint8_t device_data)
{
	//specify that we want to communicate to device address with an intended write to bus
	I2CMasterSlaveAddrSet(I2C3_BASE, device_address, false);

	//register to be read
	I2CMasterDataPut(I2C3_BASE, device_register);

	//send control byte and register address byte to slave device
	I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_BURST_SEND_START);

	//wait for MCU to finish transaction
	while(I2CMasterBusy(I2C3_BASE));

	// Data to be sent
	I2CMasterDataPut(I2C3_BASE, device_data);

	I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);

	//wait for MCU to finish transaction
	while(I2CMasterBusy(I2C3_BASE));

	I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);

	//wait for MCU to finish transaction
	while(I2CMasterBusBusy(I2C3_BASE));
}

/*
//sends an I2C command to the specified slave
void I2CSend(uint8_t slave_addr, uint8_t num_of_args)
{
    // Tell the master module what address it will place on the bus when
    // communicating with the slave.
    I2CMasterSlaveAddrSet(I2C3_BASE, slave_addr, false);

    //stores list of variable number of arguments
    va_list vargs;

    //specifies the va_list to "open" and the last fixed argument
    //so vargs knows where to start looking
    va_start(vargs, num_of_args);

    //put data to be sent into FIFO
    I2CMasterDataPut(I2C3_BASE, va_arg(vargs, uint32_t));

    //if there is only one argument, we only need to use the
    //single send I2C function
    if(num_of_args == 1)
    {
        //Initiate send of data from the MCU
        I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_SINGLE_SEND);

        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C3_BASE));

        //"close" variable argument list
        va_end(vargs);
    }

    //otherwise, we start transmission of multiple bytes on the
    //I2C bus
    else
    {
        //Initiate send of data from the MCU
        I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_BURST_SEND_START);

        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C3_BASE));

        //send num_of_args-2 pieces of data, using the
        //BURST_SEND_CONT command of the I2C module
        uint8_t i = 1;
        for(i = 1; i < (num_of_args - 1); i++)
        {
            //put next piece of data into I2C FIFO
            I2CMasterDataPut(I2C3_BASE, va_arg(vargs, uint32_t));
            //send next data that was just placed into FIFO
            I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);

            // Wait until MCU is done transferring.
            while(I2CMasterBusy(I2C3_BASE));
        }

        //put last piece of data into I2C FIFO
        I2CMasterDataPut(I2C3_BASE, va_arg(vargs, uint32_t));
        //send next data that was just placed into FIFO
        I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C3_BASE));

        //"close" variable args list
        va_end(vargs);
    }
}
*/

int I2CReceive(uint32_t slave_addr, uint8_t start_reg, uint32_t nbrBytes, uint8_t* resultArray) {
	if (nbrBytes < 1) {
		return -1;
	}

	//specify that we are writing (a register address) to the
	//slave device
	I2CMasterSlaveAddrSet(I2C3_BASE, slave_addr, false);

	//specify register to be read
	I2CMasterDataPut(I2C3_BASE, start_reg);

	//send control byte and register address byte to slave device
	I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_BURST_SEND_START);

	//wait for MCU to finish transaction
	while(I2CMasterBusy(I2C3_BASE));

	//specify that we are going to read from slave device
	I2CMasterSlaveAddrSet(I2C3_BASE, slave_addr, true);

	if (nbrBytes == 1) {
		I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

		//wait for MCU to finish transaction
		while(I2CMasterBusy(I2C3_BASE));

		resultArray[0] = I2CMasterDataGet(I2C3_BASE);
	} else {
		I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
		while(I2CMasterBusy(I2C3_BASE));
		resultArray[0] = I2CMasterDataGet(I2C3_BASE);

		int i = 1;
		for (; i < (nbrBytes-1); i++) {
			I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);

			while(I2CMasterBusy(I2C3_BASE));
			resultArray[i] = I2CMasterDataGet(I2C3_BASE);
		}
		I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

		while(I2CMasterBusy(I2C3_BASE));
		resultArray[i] = I2CMasterDataGet(I2C3_BASE);
	}

	//wait for MCU to finish transaction
	while(I2CMasterBusBusy(I2C3_BASE));

	return 1;
}

//read specified register on slave device
uint8_t I2CReceiveByte(uint32_t slave_addr, uint8_t reg)
{
	uint8_t data[0];
	I2CReceive(slave_addr, reg, 1, data);
	return data[0];
}

uint16_t I2CReceiveShort(uint32_t slave_addr, uint8_t start_reg) {
	uint8_t data[2];
	I2CReceive(slave_addr, start_reg, 2, data);
	return (uint16_t)((data[0] << 8) | data[1]);
}
