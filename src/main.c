#include <busControler.h>
#include <logicUtils.h>
#include <stddef.h>
#include "stm32l1xx.h"
#include <stdio.h>

#include "i2c.h"
#include "GY_30.h"

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

	startSystem();

	while (1) {

//***********teplota*********************//
		ADCvalue_term = readADC1_temp(ADC_Channel_0);
		calculateTemperatureToCelsius(ADCvalue_term, &celsiusF);
		printTemperatur(&celsiusF);

//***********vlhkomer*********************//
		ADCvalue_humidity = readADC1_temp(ADC_Channel_1);
		printHumidity(&ADCvalue_humidity);

//*********** read value from light senzor and print *********************/
		readDataGY_30(&I2C_data);
		printLightingX(I2C_data);

		controlWatering();

	}
	return 0;
}

void startSystem() {
	blinking(50, 5);
	delay1000(1000 );
}

void blinking(int time, int count) {
	for (int j = 1; j <= count; j++){
		GPIO_ToggleBits(GPIOA, GPIO_Pin_8);
		delay1000(time);
	}
		GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);
	delay1000(1000);
}

void controlWatering() {

	if (isDrought() == 3) {
		// je moc mokro, nepolievaj nic
		GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);
		delay1000(100);
	} else if (isDrought() == 1 && (isWarm() == 1 || isShining() == 1)) {
		// sucho  & chladno | noc ->> polievaj naplno
		blinking(20, 40);
	} else if ((isDrought() == 1 || isDrought() == 2)
			&& (isWarm() == 2 || isShining() == 2)) {
		// sucho  & mierne teplo | den ->> polievaj miernejsie ( moze to byt rano alebo nejaky jarny/jesenny den)
		blinking(100, 10);
	} else if (isWarm() == 3 && isShining() == 3) {
		// velke teplo | slnecny den ->> NEPOLIEVAJ
		GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);
		delay1000(100);
	}
}

int isWarm() {
	if (celsiusF < 23) {
		return 1; // polievaj naplno - je chladno
	} else if (23 <= celsiusF && celsiusF < 30) {
		return 2; // polievaj naplno je teplo
	} else {
		return 3; // nepolievaj - je horuco (nad 30 stupnov)
	}
}

int isDrought() {
	if (ADCvalue_humidity > 2500) {
		return 1; // polievaj - sucho
	} else if (ADCvalue_humidity > 1500 && ADCvalue_humidity <= 2500) {
		return 2; // polievaj = mierne sucho
	} else {
		return 3; // nepolievaj - mokro jak slak
	}
}

int isShining() {
	if (I2C_data < 20) {
		return 1; // polievaj naplno - je noc (pritmie)
	} else if (20 <= I2C_data && I2C_data < 15000) {
		return 2; // polievaj uvazne - je den
	} else {
		return 3; // nepolievaj - je jasny slnecny den
	}
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
