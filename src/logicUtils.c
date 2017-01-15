/*
 * logicUtils.c
 *
 *  Created on: 15. 1. 2017
 *      Author: Lukas
 */
#include "logicUtils.h"
#include "stm32l1xx.h"
#include <stdio.h>

char poleChar[10];

void delay1000(int t) {
	for (uint32_t i = 1; i < 1000 * t; i++)
		;
}

void printTemperatur(float *celsiusF) {
	//***********teplota*********************//
	int celeCislo, desatinneCislo;

	celeCislo = (int) *celsiusF;
	desatinneCislo = ((int) (*celsiusF * 100) % 100);
	sprintf(poleChar, "teplota: %d.%d [C]\r\n", celeCislo, desatinneCislo);
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	delay1000(100);
}

void printHumidity(uint16_t *ADCvalue_humidity) {
	//***********vlhkomer*********************//
	sprintf(poleChar, "vlhkomer: %d \r\n", *ADCvalue_humidity);
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	delay1000(100);
}

void printLightingX(unsigned int I2C_data) {
	//*********** light senzor*********************/
	sprintf(poleChar, "svetlo: %d [lux]\r\n\n", I2C_data);
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	delay1000(100);
}

void calculateTemperatureToCelsius(uint16_t ADCvalue_term, float *celsiusF) {
	float milivolts = (ADCvalue_term * 3.29 / 4095) * 100;
	*celsiusF = milivolts - 273;
}
