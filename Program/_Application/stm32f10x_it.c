/**
  ******************************************************************************
  * @file    STM32 - Tech Days 2009 
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    06/19/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/******************************************************************************/
/******************************************************************************/

#include <stdio.h>
#include "gps.h"
#include "rfm12.h"
#include "rfm12_controller.h"
#include "./../../Libraries/FATFs/src/diskio.h"

void SysTick_Handler(void)
{
  /* Decrement the TimingDelay variable */
  TimingDelay_Decrement();
}

void USART2_IRQHandler(void)
{
#ifdef QUAD
	//obsluga przerwania
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		gps_pool();
	}
#endif
}

void EXTI0_IRQHandler(void)
{
	uint16_t status = 0;

	if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
			
		// wylaczenie flagi obslugi przerwania
		EXTI_ClearITPendingBit(EXTI_Line0);

		// pobranie statusu
		status = rf12_read_status();
		
		// interesuje nas najstarszy bit
		if (status & RFM12_STATUS_RGIT) {		

			// wyslanie, odebranie bajtu
			rf12_pool();
		}
	}
}

void TIM1_UP_IRQHandler(void) {
	
	// przepelnienie liznika
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
		// czyszczenie flagi przerwania
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);	

#ifdef QUAD	
		// send next value
		rf12_controller_send();

		// blink
		GPIO_WriteBit(LEDS_PORT, LED_BIT_4,
			(BitAction)(1 - GPIO_ReadOutputDataBit(LEDS_PORT, LED_BIT_4)));
#endif

#ifdef PILOT	
  	
		// karta SD
		disk_timerproc();
#endif

	}
}

/******************************************************************************/
/******************************************************************************/

#ifdef Lab3_Exti
/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles External lines 9 to 5 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{

    GPIO_ResetBits(GPIOE, GPIO_Pin_14 | GPIO_Pin_15);
    /* Clear the Key Button EXTI line pending bit */
    EXTI_ClearITPendingBit(EXTI_Line9);
}

/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
 if(EXTI_GetITStatus(EXTI_Line10) != RESET)
  {
    GPIO_SetBits(GPIOE, GPIO_Pin_14);
    GPIO_ResetBits(GPIOE, GPIO_Pin_15);
    /* Clear the EXTI line 10 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line10);
  }
 else if(EXTI_GetITStatus(EXTI_Line11) != RESET)
  {
    GPIO_SetBits(GPIOE, GPIO_Pin_15);
    GPIO_ResetBits(GPIOE, GPIO_Pin_14);
    /* Clear the EXTI line 11 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line11);
  }
 else if(EXTI_GetITStatus(EXTI_Line12) != RESET)
  {
    GPIO_SetBits(GPIOE, GPIO_Pin_14 | GPIO_Pin_15);
    /* Clear the EXTI line 12 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line12);
  }
}
#endif
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
