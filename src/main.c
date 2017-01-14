#include <logic.h>
#include <stddef.h>
#include "stm32l1xx.h"
#include <stdio.h>

#include "i2c.h"
#include "GY_30.h"

extern uint16_t ADCvalue;
extern uint8_t pom;
char poleChar[10];
uint16_t ADCvalue_term;
uint16_t ADCvalue_humidity;
unsigned int data;

int celeCislo, desatinneCislo;

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
		celeCislo = (int) ((100 * ADCvalue_term * 3.29 / 4095) - 273);
		desatinneCislo = (int) ((100 * ADCvalue * 329 / 4095) - 273) % 100;

		sprintf(poleChar, "teplota: %d.%d [C]\r\n", celeCislo, desatinneCislo);
		USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
		for (uint32_t i = 1; i < 100000; i++)
			;

//***********vlhkomer*********************//
		ADCvalue_humidity = readADC1_temp(ADC_Channel_1);
		sprintf(poleChar, "vlhkomer: %d \r\n", ADCvalue_humidity);
		USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
		for (uint32_t i = 1; i < 100000; i++)
			;

//*********** read value from light senzo *********************/

		stat = readDataGY_30(&data);
		sprintf(poleChar, "svetlo: %d [lux]\r\n\n", data);
		USART_ITConfig(USART2, USART_IT_TXE, ENABLE);

		GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET); // zasvietenie LED
				/**********delay****************************///
		for (uint32_t i = 1; i < 1000000; i++)
			;

	}
	return 0;
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
