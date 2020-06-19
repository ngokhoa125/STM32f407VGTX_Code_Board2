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
uint8_t khoadata[8]={8};
uint8_t check=0,dec;
int main(void)
{
	SystemClock_Config();
	GPIO_init();
	CAN_init();
	DELAY_Configure();
	delay_ms(6000);
	BLDC_Config();
	check=1;
  while (1)
  {
		
			
  }
}

uint8_t Check_Connect(void)
{
	return check;
}
