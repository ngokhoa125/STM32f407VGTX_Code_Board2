#include "stm32f4xx.h"
#include "system_timetick.h"
#include "CAN.h"
#include "Delay.h"
#include "BLDC.h"
#include "KB_GPIO.h"

#define K_BUFFER_LENGTH		33
#define K_TRANSBUFFER_LENGTH		17
uint8_t txbuff[K_TRANSBUFFER_LENGTH];
uint8_t 	rxbuff[K_BUFFER_LENGTH];

int main(void)
{
	SystemClock_Config();
	CAN_init();
	DELAY_Configure();
	BLDC_Config();
  while (1)
  {
		BLDC_Set_Velocity(100);
		LED_Ping();
		delay_ms(1000);
		BLDC_Set_Velocity(200);
		LED_Ping();
		delay_ms(1000);
		BLDC_Set_Velocity(300);
		LED_Ping();
		delay_ms(1000);
		BLDC_Set_Velocity(200);
		LED_Ping();
		delay_ms(1000);
  }
}


