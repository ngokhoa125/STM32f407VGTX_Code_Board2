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

#ifndef __KB_GPIO
#define __KB_GPIO
#include "stm32f4xx.h"
#include "system_timetick.h"

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
void GPIO_init(void);
void LED_Ping(void);
void TurnOffAUV(void);
#endif 
