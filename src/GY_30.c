/*
 * GY_30.c
 *
 *  Created on: Dec 21, 2016
 *      Author: Marek
 */

#include "GY_30.h"
#include "i2c.h"

Status initGY_30(void) {
	Status error = I2C_Master_BufferWrite(0, 0, 0b1011100 << 1, 0x10);
	return error;
}

Status readDataGY_30(unsigned int*data) {
	unsigned char buffer[2];

	Status error = I2C_Master_BufferReadWithoutRegisterAddress(buffer, 2,
			0b1011100 << 1);

	*data = (buffer[0] << 8) + buffer[1];
	*data = (*data) / 1.2;

	return error;
}
