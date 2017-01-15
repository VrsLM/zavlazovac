#include <busControler.h>
#include <logicUtils.h>
#include <stddef.h>
#include "stm32l1xx.h"
#include <stdio.h>

#include "i2c.h"
#include "GY_30.h"

extern ADCvalue_term;
extern ADCvalue_humidity;
extern I2C_data;
extern celsiusF;

int main(void) {
	gpio_init();
	adc_init();
	usart_init();
	initI2C1();
	LED_init();
	Status stat = initGY_30();

	startSystem();

	while (1) {

//*********** teplota *********************//
		ADCvalue_term = readADC1_temp(ADC_Channel_0);
		calculateTemperatureToCelsius(ADCvalue_term, &celsiusF);
		printTemperatur(&celsiusF);

//*********** vlhkomer *********************//
		ADCvalue_humidity = readADC1_temp(ADC_Channel_1);
		printHumidity(&ADCvalue_humidity);

//*********** svetelny senzor *********************/
		readDataGY_30(&I2C_data);
		printLightingX(I2C_data);

		controlWatering();

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
