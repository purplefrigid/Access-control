/**
  ******************************************************************************
  * @file    Project/Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    04/06/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "Uart.h"

extern unsigned char Uart1RxBuf[20];
extern unsigned char Uart1RxDataConut;
extern unsigned char Rx1Flag;

extern unsigned char Uart2RxBuf[UART2_RX_BUF_LEN];
extern unsigned char Uart2RxDataConut ;
extern unsigned char Rx2Flag;

extern void Uart1_Send_Data(unsigned char *buf,unsigned char num);

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
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
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
  * @retval : None
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
  * @retval : None
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
  * @retval : None
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
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
void SysTick_Handler(void)
{
}
/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void TIM2_IRQHandler(void)
{

}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval : None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
//在此处编写串口中断接收的处理函数，我们只是简单的将输入的字符立刻输出。
//读者可以考虑一下，中断函数名是哪里决定的，可以改吗？
/*void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
		//USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);	//关中断
		Uart1RxBuf[Uart1RxDataConut]=USART_ReceiveData(USART1);
		Uart1RxDataConut++;
		if(Uart1RxDataConut >= UART1_RX_BUF_LEN)
		{
			Uart1RxDataConut = 0;
		}
		if((Uart1RxBuf[1]== Uart1RxDataConut)&&(Uart1RxBuf[1] == 12)) //判读是否接收到12字节数据
		{
			Uart1RxDataConut = 0;
			Rx1Flag = 1;
		}
		//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//开中断
	}
}*/

void USART2_IRQHandler(void)
{	
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  { 
//		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  	{ 
			Uart2RxBuf[Uart2RxDataConut]=USART_ReceiveData(USART2);
			Uart2RxDataConut++;
			if(Uart2RxDataConut >= UART2_RX_BUF_LEN)
			{
				Uart2RxDataConut = 0;
			}
			if((Uart2RxBuf[1]== Uart2RxDataConut)&&(Uart2RxBuf[1] == 12))
			{
//				Uart2RxDataConut = 0;
				Rx2Flag = 1;
			}
  	}
//		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  }
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
