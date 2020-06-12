#include "stm32f4xx.h"
#include "system_timetick.h"
#include "CAN.h"
#include "Delay.h"
#include "BLDC.h"
#include "KB_GPIO.h"

#define K_BUFFER_LENGTH		33
#define K_TRANSBUFFER_LENGTH		17

static int ID_Computer = 0x121;
static int ID_Board_UP = 0x122;

uint8_t txbuff[K_TRANSBUFFER_LENGTH];
uint8_t 	rxbuff[K_BUFFER_LENGTH];
uint8_t khoadata[8]={8};
uint8_t dec;
int main(void)
{
	SystemClock_Config();
	GPIO_init();
	CAN_init();
	DELAY_Configure();
	delay_ms(5000);
	BLDC_Config();
//		khoadata[0]=1;
//		khoadata[1]=2;
//		khoadata[2]=3;
//		khoadata[3]=4;
//		khoadata[4]=5;
//		khoadata[5]=6;
//		khoadata[6]=7;
//		khoadata[7]=8;
		
  while (1)
  {
		BLDC_Set_Velocity(100);
		LED_Ping();
		delay_ms(2000);
		BLDC_Set_Velocity(200);
		LED_Ping();
		delay_ms(2000);
		BLDC_Set_Velocity(300);
		LED_Ping();
		delay_ms(2000);
		BLDC_Set_Velocity(200);
		LED_Ping();
		delay_ms(2000);
//		dec=khoadata[0];
//		KCAN_TransData(ID_Computer,8,khoadata);
//		LED_Ping();
//		delay_ms(1000);
//		for (uint8_t i=0;i<7;i++)
//		khoadata[i]=khoadata[i+1];
//		khoadata[7]=dec;
  }
}


