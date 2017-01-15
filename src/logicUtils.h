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
#include <busControler.h>

void delay1000(int t);

void printTemperatur(float *celsiusF);
void printHumidity(int *ADCvalue_humidity);
void printLightingX(int I2C_data);

void calculateTemperatureToCelsius(int ADCvalue_term, float *celsiusF);

void startSystem();
void blinking(int time, int count);

void setWarm(float celsius);
void setDrought(int ADCvalue_humidity);
void setShining(int I2C_data);

void controlWatering();

#endif /* LOGICUTILS_H_ */
