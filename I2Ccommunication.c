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



//initialize I2C module 1
//Slightly modified version of TI's example code
void InitI2C1(void)
{
    //enable I2C module 1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);

    //reset module
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C1);

    //enable GPIO peripheral that contains I2C 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Configure the pin muxing for I2C0 functions on port B2 and B3.
    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);

    // Select the I2C function for these pins.
    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);

    // Enable and initialize the I2C0 master module.  Use the system clock for
    // the I2C0 module.  The last parameter sets the I2C data transfer rate.
    // If false the data rate is set to 100kbps and if true the data rate will
    // be set to 400kbps.
    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);

    //clear I2C FIFOs
    HWREG(I2C1_BASE + I2C_O_FIFOCTL) = 80008000;
}

//sends an I2C command to the specified slave
void I2CSend(uint16_t device_address, uint16_t device_register, uint8_t device_data)
{
	int err = 0;
	//specify that we want to communicate to device address with an intended write to bus
	I2CMasterSlaveAddrSet(I2C1_BASE, device_address, false);

	err = I2CMasterErr(I2C1_BASE);
	if (err != 0) {
		while(1){}
	}

	//register to be read
	I2CMasterDataPut(I2C1_BASE, device_data);

	err = I2CMasterErr(I2C1_BASE);
	if (err != 0) {
		while(1){}
	}

	//send control byte and register address byte to slave device
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);

	err = I2CMasterErr(I2C1_BASE);
	if (err != 0) {
		while(1){}
	}

	//wait for MCU to finish transaction
	while(I2CMasterBusBusy(I2C1_BASE));

	err = I2CMasterErr(I2C1_BASE);
	if (err != 0) {
		while(1){}
	}
}

/*
//sends an I2C command to the specified slave
void I2CSend(uint8_t slave_addr, uint8_t num_of_args)
{
    // Tell the master module what address it will place on the bus when
    // communicating with the slave.
    I2CMasterSlaveAddrSet(I2C1_BASE, slave_addr, false);

    //stores list of variable number of arguments
    va_list vargs;

    //specifies the va_list to "open" and the last fixed argument
    //so vargs knows where to start looking
    va_start(vargs, num_of_args);

    //put data to be sent into FIFO
    I2CMasterDataPut(I2C1_BASE, va_arg(vargs, uint32_t));

    //if there is only one argument, we only need to use the
    //single send I2C function
    if(num_of_args == 1)
    {
        //Initiate send of data from the MCU
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);

        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C1_BASE));

        //"close" variable argument list
        va_end(vargs);
    }

    //otherwise, we start transmission of multiple bytes on the
    //I2C bus
    else
    {
        //Initiate send of data from the MCU
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);

        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C1_BASE));

        //send num_of_args-2 pieces of data, using the
        //BURST_SEND_CONT command of the I2C module
        uint8_t i = 1;
        for(i = 1; i < (num_of_args - 1); i++)
        {
            //put next piece of data into I2C FIFO
            I2CMasterDataPut(I2C1_BASE, va_arg(vargs, uint32_t));
            //send next data that was just placed into FIFO
            I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);

            // Wait until MCU is done transferring.
            while(I2CMasterBusy(I2C1_BASE));
        }

        //put last piece of data into I2C FIFO
        I2CMasterDataPut(I2C1_BASE, va_arg(vargs, uint32_t));
        //send next data that was just placed into FIFO
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C1_BASE));

        //"close" variable args list
        va_end(vargs);
    }
}
*/


//read specified register on slave device
uint32_t I2CReceive(uint32_t slave_addr, uint8_t reg)
{
	int err = 0;
    //specify that we are writing (a register address) to the
    //slave device
    I2CMasterSlaveAddrSet(I2C1_BASE, slave_addr, false);

    err = I2CMasterErr(I2C1_BASE);
	if (err != 0) {
		while(1){}
	}

    //specify register to be read
    I2CMasterDataPut(I2C1_BASE, reg);

    //send control byte and register address byte to slave device
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    err = I2CMasterErr(I2C1_BASE);
	if (err != 0) {
		while(1){}
	}

    //wait for MCU to finish transaction
    while(I2CMasterBusBusy(I2C1_BASE));

    //specify that we are going to read from slave device
    I2CMasterSlaveAddrSet(I2C1_BASE, slave_addr, true);

    //send control byte and read from the register we
    //specified
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

    err = I2CMasterErr(I2C1_BASE);
	if (err != 0) {
		while(1){}
	}

    //wait for MCU to finish transaction
    while(I2CMasterBusBusy(I2C1_BASE));

    //return data pulled from the specified register
    return I2CMasterDataGet(I2C1_BASE);
}
