/*
 * logicUtils.c
 *
 *  Created on: 15. 1. 2017
 *      Author: Lukas
 */
#include <logicUtils.h>
#include "stm32l1xx.h"
#include <stdio.h>
#include <busControler.h>

int warm = 0;
int drought = 0;
int shining = 0;
extern poleChar[40];

void delay1000(int t) {
	for (uint32_t i = 1; i < 1000 * t; i++)
		;
}

void printTemperatur(float *celsiusF) {
	setWarm(*celsiusF);
	//***********teplota*********************//
	int celeCislo, desatinneCislo;

	celeCislo = (int) *celsiusF;
	desatinneCislo = ((int) (*celsiusF * 100) % 100);
	sprintf(poleChar, "teplota: %d.%d [C]\r\n", celeCislo, desatinneCislo);
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	delay1000(100);
}

void printHumidity(int *ADCvalue_humidity) {
	setDrought( *ADCvalue_humidity);
	//***********vlhkomer*********************//
	sprintf(poleChar, "vlhkomer: %d \r\n", *ADCvalue_humidity);
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	delay1000(100);
}

void printLightingX(int I2C_data) {
	setShining(I2C_data);
	//*********** light senzor*********************/
	sprintf(poleChar, "svetlo: %d [lux]\r\n\n", I2C_data);
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	delay1000(100);
}

void calculateTemperatureToCelsius(int ADCvalue_term, float *celsiusF) {
	float milivolts = (ADCvalue_term * 3.29 / 4095) * 100;
	*celsiusF = milivolts - 273;
}

void startSystem() {
	blinking(50, 5);
	delay1000(1000);
}

void blinking(int time, int count) {
	for (int j = 1; j <= count; j++) {
		GPIO_ToggleBits(GPIOA, GPIO_Pin_8);
		delay1000(time);
	}
	GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);
	delay1000(1000);
}

void setWarm(float celsius) {
	if (celsius < 23) {
		warm = 1; // polievaj naplno - je chladno
	} else if (23 <= celsius && celsius < 30) {
		warm = 2; // polievaj naplno je teplo
	} else {
		warm = 3; // nepolievaj - je horuco (nad 30 stupnov)
	}
}

void setDrought(int ADCvalue_humidity) {
	if (ADCvalue_humidity > 2500) {
		drought = 1; // polievaj - sucho
	} else if (ADCvalue_humidity > 1500 && ADCvalue_humidity <= 2500) {
		drought = 2; // polievaj = mierne sucho
	} else {
		drought = 3; // nepolievaj - mokro jak slak
	}
}

void setShining(int I2C_data) {
	if (I2C_data < 20) {
		shining = 1; // polievaj naplno - je noc (pritmie)
	} else if (20 <= I2C_data && I2C_data < 15000) {
		shining = 2; // polievaj uvazne - je den
	} else {
		shining = 3; // nepolievaj - je jasny slnecny den
	}
}

void controlWatering() {

	if (drought == 3) {
		// je moc mokro, nepolievaj nic
		GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);
		delay1000(100);
	} else if (drought == 1 && (warm == 1 || shining == 1)) {
		// sucho  & chladno | noc ->> polievaj naplno
		blinking(20, 40);
	} else if ((drought == 1 || drought == 2) && (warm == 2 || shining == 2)) {
		// sucho | mierne sucho && mierne teplo | den
		// ->> polievaj miernejsie ( moze to byt rano alebo nejaky jarny/jesenny den)
		blinking(100, 10);
	} else if (warm == 3 && shining == 3) {
		// velke teplo | slnecny den ->> NEPOLIEVAJ
		GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);
		delay1000(1000);
	}
}

