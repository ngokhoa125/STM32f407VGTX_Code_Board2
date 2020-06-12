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
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "BLDC.h"
#include "Delay.h"

/**
 * @defgroup Module Pin define
 * @{
 */
		/** 
		* @brief   IO Pin define 
		*/

#define PRIORITY_CAN     0
#define SUBPRIORITY_CAN  1

/*******************************************************/
CanRxMsg RxMessage1;
uint8_t CAN1_TxMessage[8];
uint8_t CAN1_RxMessage[8];

/*Define the message*/
	//WRITE
	uint8_t NMT_Message[2]={0x01,0x01};//Enable PDO 
	//----------UM---------------
  uint8_t UM2_Message[8]={0x55,0x4D,0x00,0x00,0x02,0x00,0x00,0x00};//UM=2 Mode velocity
  uint8_t UM5_Message[8]={0x55,0x4D,0x00,0x00,0x05,0x00,0x00,0x00};//UM=5	Mode postion
	//---------------------------
  uint8_t MO1_Message[8]={0x4D,0x4F,0x00,0x00,0x01,0x00,0x00,0x00};//MO=1 Enable motor
  uint8_t MO0_Message[8]={0x4D,0x4F,0x00,0x00,0x00,0x00,0x00,0x00};//MO=0	Disable motor
	//---------------------------
  uint8_t JV0_Message[8]={0x4A,0x56,0x00,0x00,0x00,0x00,0x00,0x00};//JV=0 
  uint8_t JV_Message[8]={0x4A,0x56,0x00,0x00,0x00,0x00,0x00,0x00}; // Set velocity
  uint8_t JVn_Message[8]={0x4A,0x56,0x00,0x00,0x00,0x00,0xFF,0xFF};//Send a negative value
  uint8_t JV4_Message[8]={0x4A,0x56,0x00,0x00,0x00,0x00,0x00,0x00};
	//---------------------------
  uint8_t BG_Message[8]={0x42,0x47,0x00,0x00,0x00,0x00,0x00,0x00};//BG 
	//-------------------------
  uint8_t VX_Message[8]={0x56,0x58,0x00,0x40,0x00,0x00,0x00,0x00};//Read Velocity count/sec

	//READ
 uint8_t UM_MessageR[8]={0x55,0x4D,0x00,0x40,0x00,0x00,0x00,0x00};//UM
 uint8_t MO_MessageR[8]={0x4D,0x4F,0x00,0x40,0x00,0x00,0x00,0x00};//MO
 uint8_t JV_MessageR[8]={0x4A,0x56,0x00,0x40,0x00,0x00,0x00,0x00};//JV
 uint8_t BG_MessageR[8]={0x42,0x47,0x00,0x40,0x00,0x00,0x00,0x00};//BG
/******************************************************************************************************/
/*                                                                                                    */ 
/* 	FFFFFFFFF	UUU      UUU  NNNN       NNN	CCCCCCCCCCC	TTTTTTTTTTT  III	OOOOOOOOOO	NNNN       NNN  */
/*	FFFFFFFFF	UUU      UUU  NNNNNN     NNN	CCCCCCCCCCC	TTTTTTTTTTT  III	OOOOOOOOOO	NNNNNN     NNN  */
/*	FFF      	UUU      UUU  NNN  NNN   NNN	CCC        			TTT      III	OO      OO	NNN  NNN   NNN  */
/*	FFFFFFFFF	UUU      UUU  NNN    NNN NNN	CCC             TTT      III	OO      OO	NNN    NNN NNN  */
/*	FFF      	UUU      UUU  NNN     NNNNNN	CCC           	TTT      III	OO      OO	NNN     NNNNNN  */
/*	FFF      	UUUUUUUUUUUU  NNN      NNNNN	CCCCCCCCCCC     TTT      III	OOOOOOOOOO	NNN      NNNNN  */
/*	FFF      	UUUUUUUUUUUU  NNN       NNNN	CCCCCCCCCCC     TTT      III	OOOOOOOOOO	NNN       NNNN  */
/*                                                                                                    */
/******************************************************************************************************/ 


/*******************************************************/
void CANbus_Transmit(uint32_t StdID, uint8_t can_DLC,uint8_t* can_Data)
{
		uint32_t TransmitMailbox=0;
	  uint32_t i;
		CanTxMsg TxMessage;
		TxMessage.StdId=StdID;
		TxMessage.ExtId=0;
		TxMessage.IDE = CAN_ID_STD;
		TxMessage.RTR=CAN_RTR_DATA;
		TxMessage.DLC=can_DLC;
		for(i = 0; i < can_DLC; i++)
		{
			TxMessage.Data[i] = can_Data[i];
		}
		TransmitMailbox=CAN_Transmit(CAN1, &TxMessage);
			uint32_t k=0;
		while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (k != 0xFFFF))
		{
			k++;
		}
}
/*******************************************************/
uint8_t CAN_Checksum(uint8_t* _data, int _start, int _end)
{
	int value = 0;
	
	for(int i = _start; i <= _end ; i++)
	{
		value += _data[i];
	}
	value = ~value;
	value++;
	return (uint8_t)value;
}
/*******************************************************/
void BLDC_PDO_Enable(void)
{
	CANbus_Transmit(0x00,2,NMT_Message);
}
void BLDC_Mode_Velocity(void)
{
	CANbus_Transmit(0x301,8,UM2_Message);
}
void BLDC_Begin(void)
{
	CANbus_Transmit(0x301,8,BG_Message);
}
void BLDC_Motor_On(void)
{
	CANbus_Transmit(0x301,8,MO1_Message);
}
void BLDC_Motor_Off(void)
{
	CANbus_Transmit(0x301,8,MO0_Message);
}
void BLDC_Velocity_Zero(void)
{
	CANbus_Transmit(0x301,8,JV0_Message);
}
void DEC2HEX_converter(int16_t numberDEC,uint8_t* HEXvalue1,uint8_t* HEXvalue2)
{
	
	uint8_t DECvalue1,DECvalue2;
	int16_t a[5],b[5];
	uint8_t i=0;
	uint8_t datahex[256]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
		                    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
		                    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
		                    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
		                    0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
		                    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
		                    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
		                    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
		                    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
		                    0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
		                    0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
		                    0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
		                    0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
		                    0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
		                    0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
		                    0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF,
	                      };


  a[i]=abs(numberDEC);
	b[i]=0;
	while(1)
	{  
		a[i+1]=a[i]/16;
		b[i+1]=a[i] % 16;
		if (a[i+1]==0)
		{
			break;
		}
		i++;	
	}
	DECvalue1=b[2]*pow(16,1)+b[1]*pow(16,0);
	DECvalue2=b[4]*pow(16,1)+b[3]*pow(16,0);
	if (numberDEC >0)
	{
	*HEXvalue1=datahex[DECvalue1];
	*HEXvalue2=datahex[DECvalue2];
	}
	else if (numberDEC<0)
	{
	*HEXvalue1=0x100-datahex[DECvalue1];
	*HEXvalue2=0xFF-datahex[DECvalue2];
	}
}

void BLDC_Set_Velocity(int16_t velocity)
{
	// *******Count/second*******
	DEC2HEX_converter(velocity,&JV_Message[4],&JV_Message[5]);
	CANbus_Transmit(0x301,8,JV_Message);
	delay_ms(200);
	BLDC_Begin();
}	
uint8_t a=195;
uint8_t b=0xC3;
/****************************************************************/
uint16_t Encoder_data(uint8_t q,uint8_t x,uint8_t y)
{
	uint8_t number=0;
	uint16_t Vel_Value;
	if (q==0xFF)
	{
		Vel_Value=15*pow(16,x)+15*pow(16,y);
	}
	else
		{
	for (uint8_t count=0x00;count <= q;count =count+0x01)
	{
		number++;
	}
	
	if (number<=16)
	{
		Vel_Value=0*pow(16,x)+(number-1)*pow(16,y);
	}
	else if ((number>16) && (number <=32))
	{
		Vel_Value=1*pow(16,x)+(number-17)*pow(16,y);
	}
	else if ((number>32) && (number <=48))
	{
		Vel_Value=2*pow(16,x)+(number-33)*pow(16,y);
	}
	else if ((number>48) && (number <=64))
	{
		Vel_Value=3*pow(16,x)+(number-49)*pow(16,y);
	}
	else if ((number>64) && (number <=80))
	{
		Vel_Value=4*pow(16,x)+(number-65)*pow(16,y);
	}
	else if ((number>80) && (number <=96))
	{
		Vel_Value=5*pow(16,x)+(number-81)*pow(16,y);
	}
		else if ((number>96) && (number <=112))
	{
		Vel_Value=6*pow(16,x)+(number-97)*pow(16,y);
	}
		else if ((number>112) && (number <=128))
	{
		Vel_Value=7*pow(16,x)+(number-113)*pow(16,y);
	}
		else if ((number>128) && (number <=144))
	{
		Vel_Value=8*pow(16,x)+(number-129)*pow(16,y);
	}
		else if ((number>144) && (number <=160))
	{
		Vel_Value=9*pow(16,x)+(number-145)*pow(16,y);
	}
		else if ((number>160) && (number <=176))
	{
		Vel_Value=10*pow(16,x)+(number-161)*pow(16,y);
	}
		else if ((number>176) && (number <=192))
	{
		Vel_Value=11*pow(16,x)+(number-177)*pow(16,y);
	}
		else if ((number>192) && (number <=208))
	{
		Vel_Value=12*pow(16,x)+(number-193)*pow(16,y);
	}
		else if ((number>208) && (number <=224))
	{
		Vel_Value=13*pow(16,x)+(number-209)*pow(16,y);
	}
		else if ((number>224) && (number <=240))
	{
		Vel_Value=14*pow(16,x)+(number-225)*pow(16,y);
	}
			else if (number>240)
	{
		Vel_Value=15*pow(16,x)+(number-241)*pow(16,y);
	}
	
}
return Vel_Value;
}
int16_t BLDC_ReadVelocity(uint8_t* can_data)
{
	int16_t VELOCITY;
		if ((can_data[6]==0x00) && (can_data[7]==0x00))
		{
		VELOCITY=Encoder_data(can_data[5],3,2)+Encoder_data(can_data[4],1,0);
		}
		else if ((can_data[6]==0xFF) && (can_data[7]==0xFF))
		{
	  VELOCITY=-Encoder_data(0xFF-can_data[5],3,2)-Encoder_data(0x100-can_data[4],1,0);
		}
	return VELOCITY;
}
/****************************************************************
	 CAN1open_transmit(0x00,2,NMT_Message);
	 Delay(1000);
	 CAN1open_transmit(0x301,8,UM2_Message);
	 Delay(200);
   CAN1open_transmit(0x301,8,MO1_Message);
	 Delay(200);
   CAN1open_transmit(0x301,8,JV4_Message);
	 Delay(200);
	 CAN1open_transmit(0x301,8,BG_Message);
***************************************************************/
void BLDC_Config(void)
{
	BLDC_PDO_Enable(); // 1
	delay_ms(200);
	BLDC_Mode_Velocity();// 2 
	delay_ms(200);
	BLDC_Motor_On(); // 3
	delay_ms(200);
}

