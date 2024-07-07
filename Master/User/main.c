
/*****************************************************		
 *  						   	
 *������������������������޹�˾   	
 *						   			
 ****************************************************/
#include "stm32f10x.h"
#include "Uart.h"

#define STATUS_OK			0x00
#define STATUS_ERR    0x01

#define RX_MODE  0
#define TX_MODE  1

#define RS485_Mode(mode)				((mode) == 1)?GPIO_SetBits(GPIOA, GPIO_Pin_0):GPIO_ResetBits(GPIOA, GPIO_Pin_0);

unsigned char Uart1RxBuf[UART1_RX_BUF_LEN];
unsigned char Uart1RxDataConut = 0;
unsigned char Rx1Flag = 0;

unsigned char Uart2RxBuf[UART2_RX_BUF_LEN];
unsigned char Uart2RxDataConut = 0;
unsigned char Rx2Flag = 0;

//void GpioInit(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
//	
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//	
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	 
//	
//	RS485_Mode(RX_MODE);
//}
	 
//����ʸ���жϣ�ʸ������˼������˳�����Ⱥ����˼��
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure1;	//�������ݽṹ��
  NVIC_InitTypeDef NVIC_InitStructure2;	//�������ݽṹ��
  
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);//���ж�ʸ���ŵ�Flash��0��ַ

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�������ȼ����õ�ģʽ���������Ķ�ԭ�����е�����

  //ʹ�ܴ���1�жϣ����������ȼ�
  NVIC_InitStructure1.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure1);	//���ṹ�嶪�����ú�������д�뵽��Ӧ�Ĵ�����

    //ʹ�ܴ���2�жϣ����������ȼ�
  NVIC_InitStructure2.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure2);	//���ṹ�嶪�����ú�������д�뵽��Ӧ�Ĵ�����
}

//��ʱ��10000000��ԼΪ1S
void Delay(__IO unsigned int nCount)
{
  for (; nCount != 0; nCount--);
}

//�����·�װ�İ�
void Uart1_Send_Data(unsigned char *buf,unsigned char num)
{
	unsigned char i;
	for(i=0;i<num;i++)
	{ 
	 	USART_SendData(USART1, buf[i]);
	 	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}	
}

void Uart2_Send_Data(unsigned char *buf,unsigned char num)
{
	unsigned char i;
	for(i=0;i<num;i++)
	{ 
	 	USART_SendData(USART2, buf[i]);
	 	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}	
}

unsigned char RxCheckSum(unsigned char *ptr,unsigned char len)
{
	unsigned char i;
	unsigned char checksum;
	checksum = 0;
	for(i=0;i<(len-1);i++)
	{
		   checksum ^= ptr[i];
	}
	checksum = ~checksum;
	if(ptr[len-1] == checksum)
		return 	STATUS_OK;
	else 
		return 	STATUS_ERR;
}

//void TxCheckSum(unsigned char *ptr,unsigned char len)
//{
//	unsigned char i;
//	unsigned char checksum;
//	checksum = 0;
//	for(i=0;i<(len-1);i++)
//	{
//		   checksum ^= ptr[i];
//	}
//	checksum = ~checksum;
//	ptr[len-1] = checksum;
//}
//ReadId����:��IC��ID�ţ����ţ�,��������Ҫ����Ϊ�Զ������ţ��Զ��ϴ���ʽ
//������*idout���濨��
//����ֵ��0���ɹ���ȡ���ţ�1��û�ж���������ûˢ��
unsigned char ReadTagId(unsigned char *idout)
{
	unsigned char status;
	unsigned char i;
	unsigned char pkt[12];
	status = STATUS_ERR;
	if(Uart2RxDataConut > 0) //�жϴ����Ƿ���յ�����
	{
		Delay(150000);  //�ȴ����ڽ�����ɣ����������������ʱ
		if(Rx2Flag == 1)//�жϴ����Ƿ���յ�һ֡��������
		{	
			Rx2Flag = 0;
			for(i=0;i<12;i++) //�Զ����������ݰ��̶�Ϊ12�ֽ�
			{
				pkt[i] = Uart2RxBuf[i]; //�����ڽ��������ڵ����ݸ��Ƶ�pkt������
			}
			if(RxCheckSum(pkt,12) == STATUS_OK)  //�ж�У����Ƿ���ȷ
			{
				if(pkt[4] == STATUS_OK)	//�ж��Ƿ���ȷ�Ķ�����
				{
					//04 0C 02 20 00 04 00 45 96 B7 8A 3F
					if((pkt[0] == 0x04)&&(pkt[1] == 0x0C)&&(pkt[2] == 0x02)&&(pkt[3] == 0x20))//�����ݰ������ж�
					{
						for(i=0;i<4;i++)//��ȡ4�ֽڿ���
						{
							idout[i] = pkt[i+7];//������ĵ�7���ֽڿ�ʼΪ���ţ���4�ֽ�
						}
						status = STATUS_OK;		 //�ɹ�����0
					}
				}
			}
		}
		for(i=0;i<Uart2RxDataConut;i++)//��մ��ڽ�������
		{
			Uart2RxBuf[i] = 0x00; 
		}
		Uart2RxDataConut = 0x00;
 	} 
	return status;			//ʧ�ܷ���1
}
unsigned char kahao[100][4];
int chazhao(unsigned char id[4],unsigned char kahao[][4])
{
	int i;//,j,t;
	for(i=0;i<100;i++)
	{
		if(id[0]==kahao[i][0]&&id[1]==kahao[i][1]&&id[2]==kahao[i][2]&&id[3]==kahao[i][3])
			return 0;
	}
	return 1;
}

int i,k,j,n;
uint16_t Instruction=0;
int main(void)
{	
	//����2���Ӷ�����
	//����1������λ��
	//�������Ӷ����������ն������Զ��������ݣ�Ȼ�󽫶����Ŀ���ͨ������1���ͳ�ȥ������λ����
	unsigned char status;
	unsigned char id[4];
	int num=0;
	SystemInit();					//��ʼ��ϵͳ
	NVIC_Configuration(); //��ʼ������ж�
	COM1_2_Init();  //��ʼ��UART1,UART2
//	GpioInit();
printf("����Ҫ¼�뼸�ſ�������������С��2");
	printf("\n");
	Rx1Flag = 0;
	Rx2Flag = 0;
	Uart1RxDataConut = 0;
	Uart2RxDataConut = 0;
while(1)
{
	while(Instruction)
	{
/*************���ö����ź���ReadTagId����������ID���뵽ָ���Ļ�����id��************************************/	
		//printf("%d",num);
		status = ReadTagId(id);  //��ȡ�Զ������ŵ�����
		/*if(chazhao(*id,kahao))
		{
			kahao[flag]=status;
		  flag++;
		}*/
		if(chazhao(id,kahao))
		{
			if(num<Instruction-1)//�洢Instruction�ſ���
			{
				for(i=0;i<4;i++)
		    {
					if(k==1)
			    {
				    num++;
				    k=0;
				    n++;
			    }
			  kahao[num][i]=id[i];
		    }
		 }
	 }
		if(status == STATUS_OK)	//�ж϶������Ƿ�ɹ�������0�ɹ�
		{
			//if(num<=2)
			//{
				k=1;
			//}
			/*Uart1_Send_Data(id,4);	//�Ӵ���1�Ѷ�ȡ���Ŀ��ŷ��ͳ��
			printf("%d",num);
			
			for(j=0;j<num+1;j++)
			{
				for(i=0;i<4;i++)
				{
					printf("%c",kahao[j][i]);
				}
			}*/
			//printf("/n");
			if(!chazhao(id,kahao))
			{
				//printf("find");
				Uart1_Send_Data(id,4);
			}
		}
	}
}	
printf("%d",Instruction)	;
}
#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (1);
}
#endif

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

