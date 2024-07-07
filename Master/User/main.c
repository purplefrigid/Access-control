
/*****************************************************		
 *  						   	
 *深圳市睿驰世纪物联网技术有限公司   	
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
	 
//配置矢量中断，矢量的意思就是有顺序，有先后的意思。
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure1;	//定义数据结构体
  NVIC_InitTypeDef NVIC_InitStructure2;	//定义数据结构体
  
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);//将中断矢量放到Flash的0地址

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//设置优先级配置的模式，详情请阅读原材料中的文章

  //使能串口1中断，并设置优先级
  NVIC_InitStructure1.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure1);	//将结构体丢到配置函数，即写入到对应寄存器中

    //使能串口2中断，并设置优先级
  NVIC_InitStructure2.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure2);	//将结构体丢到配置函数，即写入到对应寄存器中
}

//延时，10000000大约为1S
void Delay(__IO unsigned int nCount)
{
  for (; nCount != 0; nCount--);
}

//发送新封装的包
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
//ReadId（）:读IC卡ID号（卡号）,读卡器需要设置为自动读卡号，自动上传方式
//参数：*idout保存卡号
//返回值：0：成功读取卡号，1：没有读到卡或者没刷卡
unsigned char ReadTagId(unsigned char *idout)
{
	unsigned char status;
	unsigned char i;
	unsigned char pkt[12];
	status = STATUS_ERR;
	if(Uart2RxDataConut > 0) //判断串口是否接收到数据
	{
		Delay(150000);  //等待串口接收完成，根据视情况调整延时
		if(Rx2Flag == 1)//判断串口是否接收到一帧完整数据
		{	
			Rx2Flag = 0;
			for(i=0;i<12;i++) //自动读卡号数据包固定为12字节
			{
				pkt[i] = Uart2RxBuf[i]; //将串口接收数组内的数据复制到pkt数组中
			}
			if(RxCheckSum(pkt,12) == STATUS_OK)  //判断校验和是否正确
			{
				if(pkt[4] == STATUS_OK)	//判断是否正确的读到卡
				{
					//04 0C 02 20 00 04 00 45 96 B7 8A 3F
					if((pkt[0] == 0x04)&&(pkt[1] == 0x0C)&&(pkt[2] == 0x02)&&(pkt[3] == 0x20))//对数据包进行判断
					{
						for(i=0;i<4;i++)//获取4字节卡号
						{
							idout[i] = pkt[i+7];//从数组的第7个字节开始为卡号，共4字节
						}
						status = STATUS_OK;		 //成功返回0
					}
				}
			}
		}
		for(i=0;i<Uart2RxDataConut;i++)//清空串口接收数组
		{
			Uart2RxBuf[i] = 0x00; 
		}
		Uart2RxDataConut = 0x00;
 	} 
	return status;			//失败返回1
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
	//串口2连接读卡器
	//串口1连接上位机
	//串口连接读卡器，接收读卡器自动读卡数据，然后将读到的卡号通过串口1发送出去（给上位机）
	unsigned char status;
	unsigned char id[4];
	int num=0;
	SystemInit();					//初始化系统
	NVIC_Configuration(); //初始化相关中断
	COM1_2_Init();  //初始化UART1,UART2
//	GpioInit();
printf("请问要录入几张卡？卡的数量不小于2");
	printf("\n");
	Rx1Flag = 0;
	Rx2Flag = 0;
	Uart1RxDataConut = 0;
	Uart2RxDataConut = 0;
while(1)
{
	while(Instruction)
	{
/*************调用读卡号函数ReadTagId（）将卡号ID读入到指定的缓冲区id中************************************/	
		//printf("%d",num);
		status = ReadTagId(id);  //获取自动读卡号的数据
		/*if(chazhao(*id,kahao))
		{
			kahao[flag]=status;
		  flag++;
		}*/
		if(chazhao(id,kahao))
		{
			if(num<Instruction-1)//存储Instruction张卡号
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
		if(status == STATUS_OK)	//判断读卡号是否成功，等于0成功
		{
			//if(num<=2)
			//{
				k=1;
			//}
			/*Uart1_Send_Data(id,4);	//从串口1把读取到的卡号发送出�
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

