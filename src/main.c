#include <logic.h>
#include <stddef.h>
#include "stm32l1xx.h"
#include <stdio.h>

#include "i2c.h"
#include "GY_30.h"

char poleChar[10];
uint16_t ADCvalue_term;
uint16_t ADCvalue_humidity;
unsigned int I2C_data;
float celsiusF;


int main(void) {
	gpio_init();
	adc_init();
	usart_init();
	initI2C1();
	LED_init();
	Status stat = initGY_30();

	while (1) {
		GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET); // zasvietenie LED

//***********teplota*********************//
		ADCvalue_term = readADC1_temp(ADC_Channel_0);
		calculateTemperatureToCelsius(ADCvalue_term, &celsiusF);
		printTemperatur();

//***********vlhkomer*********************//
		ADCvalue_humidity = readADC1_temp(ADC_Channel_1);
		printHumidity();

//*********** read value from light senzo and print *********************/
		//printLighting();
		printLightingX(I2C_data, poleChar);

		GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET); // zasvietenie LED

/**********delay****************************///
		delay1000(1000);

	}
	return 0;
}

void delay1000(int t) {
	for (uint32_t i = 1; i < 1000 * t; i++)
		;
}

void printTemperatur() {
	int celeCislo, desatinneCislo;

	celeCislo = (int) celsiusF;
	desatinneCislo = ((int) (celsiusF * 100) % 100);
	sprintf(poleChar, "teplota: %d.%d [C]\r\n", celeCislo, desatinneCislo);
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	delay1000(100);
}

void printHumidity() {
	//***********vlhkomer*********************//
	sprintf(poleChar, "vlhkomer: %d \r\n", ADCvalue_humidity);
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	delay1000(100);
}

void printLightingX(unsigned int I2C_data, char poleChar[10]) {
	readDataGY_30(&I2C_data);

	//printLighting();
	sprintf(poleChar, "svetlo: %d [lux]\r\n\n", I2C_data);
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	delay1000(100);
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}
