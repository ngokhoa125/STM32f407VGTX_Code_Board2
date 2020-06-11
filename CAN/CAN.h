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
	MASS_SHIFTER = 0x00,
	PISTOL = 0x01,
	LEAK_SENSOR = 0x02,
	STRAIN_GAUGE = 0x03,
	BMS = 0x04,
	BMS24V40AH = 0x05,
	ALTIMETER = 0x06,
	POWER_INT = 0x07,
	PRESSURE=0x08,
	ALL_DATA=0xFF,
}AUV_ID;

typedef enum
{
	COM_MASSSHIFTER = 0x00,
	OPENLOOP_DUTY_MASSSHIFTER = 0x01,
	PID_DESIREDSPEED_MASSSHIFTER = 0x02,
	M_KP_MASSSHIFTER = 0x03,
	M_KI_MASSSHIFTER = 0x04,
	M_KD_MASSSHIFTER = 0x05,
	FUZZY_DESIREDSPEED_MASSSHIFTER = 0x06,
	M_GE_MASSSHIFTER = 0x07,
	M_GDE_MASSSHIFTER = 0x08,	
	M_GDU_MASSSHIFTER = 0x09,
	REQ_IMOTOR_MASSSHIFTER = 0x0A,
	REQ_ACTUALSPEED_MASSSHIFTER=0x0F,
	REQ_POSITION_MASSSHIFTER=0x11,
	WR_SPEED_MASSSHIFTER=0x12,
	WR_POSITION_MASSSHIFTER=0x13,
}AUV_REGISTER_MASSSHIFTER;

typedef enum
{
	DEPTH_FEET = 0x00,
	DEPTH_METRES = 0x01,
	DEPTH_FATHOMS = 0x02,
}AUV_REGISTER_ALTIMETER;
typedef enum
{
	LEAK_POSITION = 0x00,
}AUV_REGISTER_LEAK;

typedef enum
{
	STATUS_HULL0 = 0x00,
	STATUS_HULL1 = 0x01,
	STATUS_HULL2 = 0x02,
	STATUS_HULL3 = 0x03,
	STATUS_HULL4 = 0x04,
}AUV_REGISTER_STRAINGAUGE;

typedef enum
{
	REQ_PRESSURE = 0x00,
	REQ_TEMPERATURE = 0x01,
}AUV_REGISTER_PRESSURE;

typedef enum
{
	COM_THRUSTER = 0x00,
	OPENLOOP_DUTY_THRUSTER = 0x01,
	PID_DESIREDSPEED_THRUSTER = 0x02,
	M_KP_THRUSTER = 0x03,
	M_KI_THRUSTER = 0x04,
	M_KD_THRUSTER = 0x05,
	AFUZZY_DESIREDSPEED_THRUSTER = 0x06,
	M_GE_THRUSTER = 0x07,
	M_GDE_THRUSTER = 0x08,	
	M_GDU_THRUSTER = 0x09,
	REQ_IMOTOR_THRUSTER = 0x0A,
	REQ_TEMP_ON_CHIP_THRUSTER = 0x0B,
	REQ_TEMP_LM35_THRUSTER = 0x0C,
	REQ_DUTY_THRUSTER = 0x0D,	
	REQ_DESIREDSPEED_THRUSTER = 0x0E,
	REQ_AUTUALSPEED_THRUSTER = 0x0F,
	REQ_ENCODER_THRUSTER = 0x10,
}AUV_REGISTER_THRUSTER;

typedef enum
{
	GOAL_POSITION = 0x00,
	MOVING_SPEED = 0x01,
	MX28_KP = 0x02,
	MX28_KI = 0x03,
	MX28_KD = 0x04,
	BAUDRATE = 0x05,
	PRESENT_POSITION = 0x06,
	PRESENT_SPEED = 0x07,
	PRESENT_LOAD = 0x08,	
	PRESENT_VOL = 0x09,
	PRESENT_TEMP = 0x0A,
}AUV_REGISTER_RUDDER;

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


#endif 
