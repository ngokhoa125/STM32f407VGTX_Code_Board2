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

#ifndef __BLDC
#define __BLDC
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
 
/******************************************************************************
 *                                                                            *
 *  												FUNCTION	                                        *
 *  														                                              *
 ******************************************************************************/
void CANbus_Transmit(uint32_t StdID, uint8_t can_DLC,uint8_t* can_Data);
uint8_t CAN_Checksum(uint8_t* _data, int _start, int _end);
void BLDC_PDO_Enable(void);
void BLDC_Mode_Velocity(void);
void BLDC_Begin(void);
void BLDC_Motor_On(void);
void BLDC_Motor_Off(void);
void BLDC_Velocity_Zero(void);
void DEC2HEX_converter(int16_t numberDEC,uint8_t* HEXvalue1,uint8_t* HEXvalue2);
void BLDC_Set_Velocity(int16_t velocity);
uint16_t Encoder_data(uint8_t q,uint8_t x,uint8_t y);
int16_t BLDC_ReadVelocity(uint8_t* can_data);
void BLDC_Config(void);
void VX (void);
#endif 
