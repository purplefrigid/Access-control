/*
 * FileName:       
 * Author:         YuanYin  Version: QXW-V1.x  Date: 2010-3-11
 * Description: ���ļ���ĺ���������1.Uart�����һ���ģ�����Ͳ����ظ�ע���ˡ�    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#include "stm32f10x.h"
#include "Uart.h"
extern uint16_t Instruction;
void COM1_2_Init( void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 9600;	//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;		//��żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//����������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ģʽ

	/* ����GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_Init(USART1, &USART_InitStructure);
	USART_Init(USART2, &USART_InitStructure);
	

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		 
	/* Enable USART */
	USART_Cmd(USART1, ENABLE);
	USART_Cmd(USART2, ENABLE);
	
}
void USART1_IRQHandler(void)                	//����1�жϷ������
	{
	//u8 Res;
		
			if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
			USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
			Instruction=USART_ReceiveData(USART1)-48;
	   printf("%d",Instruction);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
     } 
//USART_ClearITPendingBit(USART1,USART_IT_RXNE);
} 
//��ʹ�ð�����ģʽ
#if 1 //���û����Σ�����Ҫ��targetѡ����ѡ��ʹ��USE microLIB
#pragma import(__use_no_semihosting)
struct __FILE  
{  
	int handle;  
};  
FILE __stdout;  

_sys_exit(int x)  
{  
	x = x;  
}
#endif

int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (unsigned char) ch); 

	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) 
	{}

	return ch;
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

