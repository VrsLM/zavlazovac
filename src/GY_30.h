/*
 * GY_30.h
 *
 *  Created on: Dec 21, 2016
 *      Author: Marek
 */

#ifndef GY_30_H_
#define GY_30_H_
#include "i2c.h"

#define GY_30_POWER_DOWN 0x00
#define GY_30_POWER_ON 0x01
#define GY_30_ADDR 0x23
#define GY_30_ONE_TIME_HIGH_RES_MODE 0x20

Status initGY_30(void);
Status readDataGY_30(unsigned int*data);

#endif /* GY_30_H_ */
