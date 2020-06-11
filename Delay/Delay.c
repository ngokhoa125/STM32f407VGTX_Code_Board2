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

#include "Delay.h"


/**
 * @defgroup Timer define
 * @{
 */
			/** 
			* @brief   Timer delay define
			*/

			#define 	DELAY_TIM  				      		TIM6
			#define 	DELAY_TIM_CLK					 	 		RCC_APB1Periph_TIM6	
			#define 	DELAY_TIM_CLK_Cmd    				RCC_APB1PeriphClockCmd


/* Private variables ---------------------------------------------------------*/
static TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 
/* Exported function body ----------------------------------------------------*/
void SystemClock_Config(void)
{
  // Resets the clock configuration to the default reset state
    RCC_DeInit();

    // Enable external crystal (HSE)
    RCC_HSEConfig(RCC_HSE_ON);
    // Wait until HSE ready to use or not
    ErrorStatus errorStatus = RCC_WaitForHSEStartUp();

    if (errorStatus == SUCCESS)
    {
        // Configure the PLL for 168MHz SysClk and 48MHz for USB OTG, SDIO
        RCC_PLLConfig(RCC_PLLSource_HSE, 8, 336, 2, 7);
        // Enable PLL
        RCC_PLLCmd(ENABLE);
        // Wait until main PLL clock ready
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

        // Set flash latency
        FLASH_SetLatency(FLASH_Latency_5);

        // AHB 168MHz
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        // APB1 42MHz
        RCC_PCLK1Config(RCC_HCLK_Div4);
        // APB2 84 MHz
        RCC_PCLK2Config(RCC_HCLK_Div2);

        // Set SysClk using PLL
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    }
    else
    {
        // Do something to indicate that error clock configuration
        while (1);
    }

    SystemCoreClockUpdate();
}


void DELAY_Configure(void)
{

		DELAY_TIM_CLK_Cmd(DELAY_TIM_CLK, ENABLE);		

		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		
		TIM_TimeBaseInit(DELAY_TIM, &TIM_TimeBaseStructure);
}
 
void delay_ms(uint32_t ms)
{	
		int i;
	
		TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock/4)/1000000) * 2 - 1;     // frequency = 1000000 (Hz) = 1 us
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;

		TIM_TimeBaseInit(DELAY_TIM, &TIM_TimeBaseStructure);
	
		for (i = 0; i < ms; i++)
		{
			TIM_ClearFlag(DELAY_TIM, TIM_FLAG_Update);
			
			TIM_Cmd(DELAY_TIM, ENABLE);
					
			while (!TIM_GetFlagStatus(DELAY_TIM, TIM_IT_Update));
						
			TIM_Cmd(DELAY_TIM, DISABLE);
			TIM_ClearFlag(DELAY_TIM, TIM_FLAG_Update);
		}	
}


void delay_us(uint32_t us)
{	
		int i;
	
		TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock/4)/2000000) * 2 - 1;     // frequency = 2000000 (Hz) = 0.5 us
    TIM_TimeBaseStructure.TIM_Period = 4 - 1;

		TIM_TimeBaseInit(DELAY_TIM, &TIM_TimeBaseStructure);
	
		for (i = 0; i < us; i++)
		{
			TIM_ClearFlag(DELAY_TIM, TIM_FLAG_Update);
			
			TIM_Cmd(DELAY_TIM, ENABLE);
					
			while (!TIM_GetFlagStatus(DELAY_TIM, TIM_IT_Update));
						
			TIM_Cmd(DELAY_TIM, DISABLE);
		}	
}

