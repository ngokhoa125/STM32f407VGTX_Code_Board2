/**********************************************************/
/*                                                        */ 
/* 	KKK   KKK		HHH     HHH		OOOOOOOOOOO		  AAAAAAAA    */
/*	KKK  KKK		HHH     HHH   OOOOOOOOOOO		AAAAAAAAAAAA  */
/*	KKK KKK     HHH	    HHH   OOO     OOO		AAA      AAA  */
/*	KKKKK				HHHHHHHHHHH   OOO     OOO		AAA      AAA  */
/*	KKKKK     	HHHHHHHHHHH   OOO     OOO		AAAAAAAAAAAA  */
/*	KKK KKK     HHH	    HHH	  OOO     OOO		AAA      AAA  */
/*	KKK  KKK    HHH	    HHH	  OOOOOOOOOOO		AAA      AAA  */
/*  KKK   KKK   HHH     HHH   OOOOOOOOOOO   AAA      AAA  */  
/*                                                        */ 
/**********************************************************/ 
#include "stm32f4xx.h"
#include "system_timetick.h"
#include "CAN.h"
#include "Delay.h"
#include "BLDC.h"
#include "main.h"
#include "KB_GPIO.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
/**
 * @defgroup Module Pin define
 * @{
 */
		/** 
		* @brief   CAN Pin define 
		*/
			#define 	KCAN_TX_PIN 				   	  GPIO_Pin_9			
			#define 	KCAN_TX_PORT 				    	GPIOB
			#define 	KCAN_TX_CLK							  RCC_AHB1Periph_GPIOB			
			#define 	KCAN_TX_SOURCE				  	GPIO_PinSource9				
			#define		KCAN_TX_CLK_Cmd		  	  	RCC_AHB1PeriphClockCmd			

			#define 	KCAN_RX_PIN  				    	GPIO_Pin_8
			#define 	KCAN_RX_PORT 				    	GPIOB
			#define 	KCAN_RX_CLK						  	RCC_AHB1Periph_GPIOB
			#define 	KCAN_RX_SOURCE				  	GPIO_PinSource8
			#define		KCAN_RX_CLK_Cmd		  	  	RCC_AHB1PeriphClockCmd		

			#define		KCAN_CAN							    CAN1
			#define 	KCAN_CAN_CLK					    RCC_APB1Periph_CAN1
			#define		KCAN_CAN_CLK_Cmd		     	RCC_APB1PeriphClockCmd
			#define		KCAN_CAN_AF							  GPIO_AF_CAN1
			#define 	KCAN_CAN_IRQn					    CAN1_RX0_IRQn
			
			#define		KCAN_CAN_IRQHandler		  	CAN1_RX0_IRQHandler
/**
 * @}
 */

//static int ID_Computer = 0x121;
//static int ID_Board_UP = 0x122;
static int ID_Elmo = 0x281;
static int ID_Board_DOWN = 0x122;


uint8_t CAN_RxMessage[8];
uint8_t CAN_TxMessage[8];
CanRxMsg RxMessage;//Gan CanRxMsg thanh RxMessage
int16_t BLDC_VELOCITY;
int ENC_VELOCITY;
uint8_t BLDC_STATUS=1;
uint8_t BLDC_DIRECTION=1;
uint8_t BLDC_Send_Data[8];
uint8_t Temp_BLDC_Send_Data[4];
uint8_t Connection;
void CAN_init(void)
{
//	Khai bao GPIO cho CAN1
	GPIO_InitTypeDef GPIO_InitStruct;
	CAN_InitTypeDef CAN_InitStruct;
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//	Khai bao chan RX cho CAN1
  KCAN_TX_CLK_Cmd(KCAN_TX_CLK,ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=KCAN_TX_PIN;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_High_Speed;
	GPIO_Init(KCAN_RX_PORT,&GPIO_InitStruct);
	//	Khai bao chan TX cho CAN1
	KCAN_RX_CLK_Cmd(KCAN_RX_CLK,ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=KCAN_RX_PIN;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_High_Speed;
	GPIO_Init(KCAN_RX_PORT,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(KCAN_TX_PORT,KCAN_TX_SOURCE,KCAN_CAN_AF);
	GPIO_PinAFConfig(KCAN_RX_PORT,KCAN_RX_SOURCE,KCAN_CAN_AF);
//Khai bao cho CAN1	
	KCAN_CAN_CLK_Cmd(KCAN_CAN_CLK,ENABLE);
	CAN_DeInit(KCAN_CAN);
		
	CAN_InitStruct.CAN_TTCM = DISABLE;
	CAN_InitStruct.CAN_ABOM = DISABLE;
	CAN_InitStruct.CAN_AWUM = DISABLE;
	CAN_InitStruct.CAN_NART = ENABLE;
	CAN_InitStruct.CAN_RFLM = DISABLE;
	CAN_InitStruct.CAN_TXFP = ENABLE;
	CAN_InitStruct.CAN_Mode=CAN_Mode_Normal;
	CAN_InitStruct.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStruct.CAN_BS1 = CAN_BS1_12tq;
	CAN_InitStruct.CAN_BS2 = CAN_BS2_8tq;
	CAN_InitStruct.CAN_Prescaler = 2;
	CAN_Init(KCAN_CAN, &CAN_InitStruct);
	/* CAN Baudrate = 1 MBps (CAN clocked at 42 MHz) */
	// Baudrate = 1/(tq + tBS1 + tBS2)
	//http://www.bittiming.can-wiki.info/
	
//	Khai bao cho CAN Filter
	CAN_FilterInitStruct.CAN_FilterNumber = 0;
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStruct.CAN_FilterIdHigh = ID_Elmo << 5;
	CAN_FilterInitStruct.CAN_FilterIdLow = ID_Board_DOWN << 5;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = ID_Board_DOWN << 5;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow = ID_Elmo << 5;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStruct);
	//	/* Enable FIFO 0 message pending Interrupt */
	CAN_ITConfig(KCAN_CAN, CAN_IT_FMP0, ENABLE);

	NVIC_InitStruct.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	
}
void KCAN_TransData(int _IDstd,int _length, uint8_t _data[])
{
	CanTxMsg _TxMessage;
  uint8_t mailbox;
	uint8_t status;
	_TxMessage.StdId = _IDstd;
	_TxMessage.ExtId = 0x00;
  _TxMessage.RTR = CAN_RTR_DATA;
  _TxMessage.IDE = CAN_ID_STD;
  _TxMessage.DLC = _length;
	
	for (int j=0;j<_length;j++)
	{
		_TxMessage.Data[j]=_data[j];
	}
	mailbox = CAN_Transmit(KCAN_CAN,&_TxMessage);
	//Cho cho den khi CAN truyen xong hoac qua thoi gian
	int32_t i = 0;
	while((status != CANTXOK) && (i != 0xFFFF))
  {
    status = CAN_TransmitStatus(KCAN_CAN,mailbox);
    i++;
  }
}

uint8_t KCAN_Checksum(uint8_t *_data)
{
	uint8_t value = 0;
	//Tinh toan gia tri CheckSum (Byte)
	for (int i = 0; i < 7; i++)
	{
		value += _data[i];
	}
	value = ~value;
	value++;
	return (uint8_t)value;
}
//Ham chuyen tu kieu Float sang kieu Byte
void Convert_Float2Byte(float _data_in,uint8_t* _data_out)
{
	union 
	{
		float _value;
		uint8_t _byte[4];
	}_number;
	_number._value=_data_in;
	
	_data_out[0]=_number._byte[0];
	_data_out[1]=_number._byte[1];
	_data_out[2]=_number._byte[2];
	_data_out[3]=_number._byte[3];
}
//Ham chuyen tu kieu Byte sang kieu Float
void Convert_Byte2Float(uint8_t* _data_in, float* _data_out)
{
	union
	{ 
		float _value;
		uint8_t _byte[4];
	}_number;
	 
	_number._byte[0]=_data_in[0];
	_number._byte[1]=_data_in[1];
	_number._byte[2]=_data_in[2];
	_number._byte[3]=_data_in[3];
	
	*_data_out=_number._value;
}

void Process_Thurster(void)
{
	Connection=Check_Connect();
	if (Connection)
	{
				if ((RxMessage.Data[1]==WRITE_DATA)&&(RxMessage.Data[2]==VELOCITY))
				{
					BLDC_VELOCITY=(int16_t)( RxMessage.Data[3]+(RxMessage.Data[4]<<8));
//					if (BLDC_DIRECTION==2) BLDC_VELOCITY=-BLDC_VELOCITY;
					BLDC_Set_Velocity(BLDC_VELOCITY);
				}
				else if ((RxMessage.Data[1]==WRITE_DATA)&&(RxMessage.Data[2]==START))
				{
						if (BLDC_STATUS==0) 
						{BLDC_Motor_On();
						BLDC_STATUS=1;}
				}
				else if ((RxMessage.Data[1]==WRITE_DATA)&&(RxMessage.Data[2]==STOP))
				{
						if (BLDC_STATUS==1) 
						{BLDC_Motor_Off();
						BLDC_STATUS=0;}
				}
//				else if ((RxMessage.Data[1]==WRITE_DATA)&&(RxMessage.Data[2]==DIRECTION))
//				{
//					LED_Ping();	
//					if (RxMessage.Data[3]==1)
//							BLDC_DIRECTION=1;
//					else if (RxMessage.Data[3]==2)
//							BLDC_DIRECTION=2;
//				}
	}
}
void Process_All_Data(void)
{
				int rand_velocity=0;
				rand_velocity=rand()%1000;
				VX();
				Connection=Check_Connect();
				if ((Connection==1)&&(RxMessage.Data[1]==READ_DATA)&&(RxMessage.Data[2]==0x00))
				{
					//SEND STATUS OF MOTOR//
				BLDC_Send_Data[0]=THURSTER;
				BLDC_Send_Data[1]=STATUS_DATA;
				BLDC_Send_Data[2]=BLDC_STATUS_ENUM;
				BLDC_Send_Data[3]=BLDC_STATUS;
				BLDC_Send_Data[4]=BLDC_STATUS;
				BLDC_Send_Data[5]=BLDC_STATUS;
				BLDC_Send_Data[6]=BLDC_STATUS;
				BLDC_Send_Data[7]=KCAN_Checksum(BLDC_Send_Data);
				KCAN_TransData(ID_Board_DOWN,8,BLDC_Send_Data);
					//SEND VOLOCITY//
					delay_ms(2);
				BLDC_Send_Data[0]=THURSTER;
				BLDC_Send_Data[1]=STATUS_DATA;
				BLDC_Send_Data[2]=REQ_VELOCITY;
				Convert_Float2Byte(rand_velocity,Temp_BLDC_Send_Data);
				BLDC_Send_Data[3]=Temp_BLDC_Send_Data[0];
				BLDC_Send_Data[4]=Temp_BLDC_Send_Data[1];
				BLDC_Send_Data[5]=Temp_BLDC_Send_Data[2];
				BLDC_Send_Data[6]=Temp_BLDC_Send_Data[3];
				BLDC_Send_Data[7]=KCAN_Checksum(BLDC_Send_Data);
				KCAN_TransData(ID_Board_DOWN,8,BLDC_Send_Data);
			}
}

void KCAN_CAN_IRQHandler(void)
{
	CAN_Receive(KCAN_CAN,CAN_FIFO0,&RxMessage);
	//--------Thruster--------//
	if((RxMessage.StdId == ID_Elmo)&&(RxMessage.Data[0]=='V')&&(RxMessage.Data[1]=='X'))
	{
	
		if ((RxMessage.Data[6]==0x00) && (RxMessage.Data[7]==0x00))
		{
		ENC_VELOCITY=Encoder_data(RxMessage.Data[5],3,2)+Encoder_data(RxMessage.Data[4],1,0);
		}
		else if ((RxMessage.Data[6]==0xFF) && (RxMessage.Data[7]==0xFF))
		{
	  ENC_VELOCITY=-Encoder_data(0xFF-RxMessage.Data[5],3,2)-Encoder_data(0x100-RxMessage.Data[4],1,0);
		}
	}
	if((RxMessage.StdId == ID_Board_DOWN)&&(RxMessage.IDE == CAN_ID_STD)&&(RxMessage.DLC == 8)&&(KCAN_Checksum(RxMessage.Data) == RxMessage.Data[7]))
	{
		if (RxMessage.Data[0]==THURSTER) Process_Thurster();
		else if (RxMessage.Data[0]==ALL_DATA) Process_All_Data();
	}
}
