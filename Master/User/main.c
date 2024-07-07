
/*****************************************************		
 *  						   	
 *ÉîÛÚÊĞî£³ÛÊÀ¼ÍÎïÁªÍø¼¼ÊõÓĞÏŞ¹«Ë¾   	
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
	 
//ÅäÖÃÊ¸Á¿ÖĞ¶Ï£¬Ê¸Á¿µÄÒâË¼¾ÍÊÇÓĞË³Ğò£¬ÓĞÏÈºóµÄÒâË¼¡£
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure1;	//¶¨ÒåÊı¾İ½á¹¹Ìå
  NVIC_InitTypeDef NVIC_InitStructure2;	//¶¨ÒåÊı¾İ½á¹¹Ìå
  
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);//½«ÖĞ¶ÏÊ¸Á¿·Åµ½FlashµÄ0µØÖ·

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//ÉèÖÃÓÅÏÈ¼¶ÅäÖÃµÄÄ£Ê½£¬ÏêÇéÇëÔÄ¶ÁÔ­²ÄÁÏÖĞµÄÎÄÕÂ

  //Ê¹ÄÜ´®¿Ú1ÖĞ¶Ï£¬²¢ÉèÖÃÓÅÏÈ¼¶
  NVIC_InitStructure1.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure1);	//½«½á¹¹Ìå¶ªµ½ÅäÖÃº¯Êı£¬¼´Ğ´Èëµ½¶ÔÓ¦¼Ä´æÆ÷ÖĞ

    //Ê¹ÄÜ´®¿Ú2ÖĞ¶Ï£¬²¢ÉèÖÃÓÅÏÈ¼¶
  NVIC_InitStructure2.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure2);	//½«½á¹¹Ìå¶ªµ½ÅäÖÃº¯Êı£¬¼´Ğ´Èëµ½¶ÔÓ¦¼Ä´æÆ÷ÖĞ
}

//ÑÓÊ±£¬10000000´óÔ¼Îª1S
void Delay(__IO unsigned int nCount)
{
  for (; nCount != 0; nCount--);
}

//·¢ËÍĞÂ·â×°µÄ°ü
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
//ReadId£¨£©:¶ÁIC¿¨IDºÅ£¨¿¨ºÅ£©,¶Á¿¨Æ÷ĞèÒªÉèÖÃÎª×Ô¶¯¶Á¿¨ºÅ£¬×Ô¶¯ÉÏ´«·½Ê½
//²ÎÊı£º*idout±£´æ¿¨ºÅ
//·µ»ØÖµ£º0£º³É¹¦¶ÁÈ¡¿¨ºÅ£¬1£ºÃ»ÓĞ¶Áµ½¿¨»òÕßÃ»Ë¢¿¨
unsigned char ReadTagId(unsigned char *idout)
{
	unsigned char status;
	unsigned char i;
	unsigned char pkt[12];
	status = STATUS_ERR;
	if(Uart2RxDataConut > 0) //ÅĞ¶Ï´®¿ÚÊÇ·ñ½ÓÊÕµ½Êı¾İ
	{
		Delay(150000);  //µÈ´ı´®¿Ú½ÓÊÕÍê³É£¬¸ù¾İÊÓÇé¿öµ÷ÕûÑÓÊ±
		if(Rx2Flag == 1)//ÅĞ¶Ï´®¿ÚÊÇ·ñ½ÓÊÕµ½Ò»Ö¡ÍêÕûÊı¾İ
		{	
			Rx2Flag = 0;
			for(i=0;i<12;i++) //×Ô¶¯¶Á¿¨ºÅÊı¾İ°ü¹Ì¶¨Îª12×Ö½Ú
			{
				pkt[i] = Uart2RxBuf[i]; //½«´®¿Ú½ÓÊÕÊı×éÄÚµÄÊı¾İ¸´ÖÆµ½pktÊı×éÖĞ
			}
			if(RxCheckSum(pkt,12) == STATUS_OK)  //ÅĞ¶ÏĞ£ÑéºÍÊÇ·ñÕıÈ·
			{
				if(pkt[4] == STATUS_OK)	//ÅĞ¶ÏÊÇ·ñÕıÈ·µÄ¶Áµ½¿¨
				{
					//04 0C 02 20 00 04 00 45 96 B7 8A 3F
					if((pkt[0] == 0x04)&&(pkt[1] == 0x0C)&&(pkt[2] == 0x02)&&(pkt[3] == 0x20))//¶ÔÊı¾İ°ü½øĞĞÅĞ¶Ï
					{
						for(i=0;i<4;i++)//»ñÈ¡4×Ö½Ú¿¨ºÅ
						{
							idout[i] = pkt[i+7];//´ÓÊı×éµÄµÚ7¸ö×Ö½Ú¿ªÊ¼Îª¿¨ºÅ£¬¹²4×Ö½Ú
						}
						status = STATUS_OK;		 //³É¹¦·µ»Ø0
					}
				}
			}
		}
		for(i=0;i<Uart2RxDataConut;i++)//Çå¿Õ´®¿Ú½ÓÊÕÊı×é
		{
			Uart2RxBuf[i] = 0x00; 
		}
		Uart2RxDataConut = 0x00;
 	} 
	return status;			//Ê§°Ü·µ»Ø1
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
	//´®¿Ú2Á¬½Ó¶Á¿¨Æ÷
	//´®¿Ú1Á¬½ÓÉÏÎ»»ú
	//´®¿ÚÁ¬½Ó¶Á¿¨Æ÷£¬½ÓÊÕ¶Á¿¨Æ÷×Ô¶¯¶Á¿¨Êı¾İ£¬È»ºó½«¶Áµ½µÄ¿¨ºÅÍ¨¹ı´®¿Ú1·¢ËÍ³öÈ¥£¨¸øÉÏÎ»»ú£©
	unsigned char status;
	unsigned char id[4];
	int num=0;
	SystemInit();					//³õÊ¼»¯ÏµÍ³
	NVIC_Configuration(); //³õÊ¼»¯Ïà¹ØÖĞ¶Ï
	COM1_2_Init();  //³õÊ¼»¯UART1,UART2
//	GpioInit();
printf("ÇëÎÊÒªÂ¼Èë¼¸ÕÅ¿¨£¿¿¨µÄÊıÁ¿²»Ğ¡ÓÚ2");
	printf("\n");
	Rx1Flag = 0;
	Rx2Flag = 0;
	Uart1RxDataConut = 0;
	Uart2RxDataConut = 0;
while(1)
{
	while(Instruction)
	{
/*************µ÷ÓÃ¶Á¿¨ºÅº¯ÊıReadTagId£¨£©½«¿¨ºÅID¶ÁÈëµ½Ö¸¶¨µÄ»º³åÇøidÖĞ************************************/	
		//printf("%d",num);
		status = ReadTagId(id);  //»ñÈ¡×Ô¶¯¶Á¿¨ºÅµÄÊı¾İ
		/*if(chazhao(*id,kahao))
		{
			kahao[flag]=status;
		  flag++;
		}*/
		if(chazhao(id,kahao))
		{
			if(num<Instruction-1)//´æ´¢InstructionÕÅ¿¨ºÅ
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
		if(status == STATUS_OK)	//ÅĞ¶Ï¶Á¿¨ºÅÊÇ·ñ³É¹¦£¬µÈÓÚ0³É¹¦
		{
			//if(num<=2)
			//{
				k=1;
			//}
			/*Uart1_Send_Data(id,4);	//´Ó´®¿Ú1°Ñ¶ÁÈ¡µ½µÄ¿¨ºÅ·¢ËÍ³öÈ
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

