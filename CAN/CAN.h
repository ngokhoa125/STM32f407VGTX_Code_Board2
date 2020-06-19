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

#ifndef __CAN
#define __CAN
#include "stm32f4xx.h"
#include "system_timetick.h"
#include "misc.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include <stdbool.h>

/******************************************************************************
 *                                                                            *
 *  												DEFINE  	                                        *
 *  														                                              *
 ******************************************************************************/


/******************************************************************************
 *                                                                            *
 *  												TYPEDEFINE                                        *
 *  														                                              *
 ******************************************************************************/
 typedef enum
{
	WRITE_DATA = 0x00,
	READ_DATA = 0x01,
	STATUS_DATA = 0x02,
}AUV_FUNCTION;
typedef enum
{
	THURSTER = 0x00,
	ALL_DATA=0xFF,
}AUV_ID;


typedef enum
{
	COM = 0x00,
	VELOCITY = 0x01,
	START = 0x02,
	STOP = 0x03,
	REQ_VELOCITY = 0x04,
	BLDC_STATUS_ENUM = 0x05,
	DIRECTION = 0x06,
	
}AUV_REGISTER_THRUSTER;



/******************************************************************************
 *                                                                            *
 *  												FUNCTION	                                        *
 *  														                                              *
 ******************************************************************************/

void CAN_init(void);
void KCAN_TransData(int _IDstd,int _length, uint8_t _data[]);
void Convert_Float2Byte(float _data_in,uint8_t* _data_out);
void Convert_Byte2Float(uint8_t* _data_in, float* _data_out); 
void KCAN_CAN_IRQHandler(void);
void Process_Thurster(void);
void Process_All_Data(void);
#endif 
