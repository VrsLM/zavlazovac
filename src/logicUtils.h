/*
 * logicUtils.h
 *
 *  Created on: 15. 1. 2017
 *      Author: Lukas
 */

#ifndef LOGICUTILS_H_
#define LOGICUTILS_H_

#include "stm32l1xx.h"
#include <stdio.h>

void delay1000(int t);

void printTemperatur(float *celsiusF);
void printHumidity(uint16_t *ADCvalue_humidity);
void printLightingX(unsigned int I2C_data);

void calculateTemperatureToCelsius(uint16_t ADCvalue_term, float *celsiusF);

#endif /* LOGICUTILS_H_ */
