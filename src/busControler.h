/*
 * vrs_cv6_druha.h
 *
 *  Created on: Oct 27, 2016
 *      Author: Marek
 */

#ifndef VRS_CV5_H_
#define VRS_CV5_H_

#include "stm32l1xx.h"

void adc_init(void);
void gpio_init(void);
void usart_init(void);
void LED_init(void);

void ADC1_IRQHandler(void);
void USART1_IRQHandler(void);
int readADC1_temp(uint8_t channel);

#endif /* VRS_CV5_H_ */

