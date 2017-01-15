/*

 * vrs_cv6_druha.c
 *
 *  Created on: Oct 27, 2016
 *      Author: Marek
 */

#include <busControler.h>

int ADCvalue_term;
int ADCvalue_humidity;
int I2C_data;
float celsiusF;

char poleChar[40];
uint16_t ADCvalue = 0;
uint8_t pom = 0;
uint8_t index = 0;


void adc_init(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	/* Enable the HSI oscillator */
	RCC_HSICmd(ENABLE);

	/* Check that HSI oscillator is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	/* Enable ADC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);

	/* ADC1 configuration */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADCx regular channel8 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_384Cycles);
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn; //zoznam prerušení nájdete v
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the ADC */
	ADC_Cmd(ADC1, ENABLE);

	/* Wait until the ADC1 is ready */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET);

	/* Start conversion */
	ADC_SoftwareStartConv(ADC1);
}

void gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // USART

	/* Configure ADCx Channel 0 as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
}

void usart_init()
{
	/* USART setup */
	USART_InitTypeDef USART_InitStructure;

//	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = 9800;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	/* Enable USART interrupt */
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);


	/* NVIC for USART2 */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(USART2,ENABLE);

}

/////////**************init led***************////
void LED_init (void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

}


void ADC1_IRQHandler(void)
{
	if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC))
	{
		ADCvalue = ADC_GetConversionValue(ADC1);
	}
}



void USART2_IRQHandler()
{
	if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE))
	{

		if (USART_ReceiveData(USART2) == 'm')
		{
			if (!pom) pom = 1;
			else pom = 0;
		}

	}

	if(USART_GetFlagStatus(USART2,USART_FLAG_TXE))
	{
		if(poleChar[index]!=0)
		{
			USART_SendData(USART2,poleChar[index++]);
		}
		else
		{
			index=0;
			USART_ITConfig(USART2,USART_IT_TXE,DISABLE);
		}
	}
}

int readADC1_temp(uint8_t channel) {
	ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_384Cycles);
	// Start the conversion
	ADC_SoftwareStartConv(ADC1);
	// Wait until conversion completion
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
		;
	// Get the conversion value
	return ADC_GetConversionValue(ADC1);
}



