
#ifndef R2000_H

#define R2000_H

#define Rs485_Tx_Mode() GPIO_SetBits(GPIOB, GPIO_Pin_0) 
#define Rs485_Rx_Mode() GPIO_ResetBits(GPIOB, GPIO_Pin_0) 

//R2000 CMD TYPE1,R2000 CMD			 
#define CMD_TYPE_1  0x01  //√¸¡Ó¿‡–Õ
#define UHF100X_ID  0x01  //√¸¡Ó
#define UHF100X_MODE  0x02	  //√¸¡Ó
#define UHF100X_FIND_READER  0x03  //√¸¡Ó


//R2000 CMD TYPE2,R2000 CMD
#define CMD_TYPE_2  0x02   //√¸¡Ó¿‡–Õ
#define CMD_OPEN_UART 0x20	  //√¸¡Ó
#define CMD_INVENTORY 0x30	  //√¸¡Ó
#define CMD_ANTENNA_CYCLE 0x40	//√¸¡Ó


//R2000 CMD TYPE,R2000 CMD
#define CMD_TYPE_3  0x03

#define R2000_PKT_TYPE_0 0x0
#define R2000_PKT_TYPE_LED 16

#define R2000_PKT_TYPE_1 0x1
#define R2000_PKT_TYPE_2 0x2
#define R2000_PKT_TYPE_3 0x3
#define R2000_PKT_TYPE_4 0x4
#define R2000_PKT_TYPE_5 0x5

#define UART1_FLAG	0x01
#define UART2_FLAG	0x01

#define DEFAULT_DEV_ADDR 0x20


#define UART2_RX_BUF_FULL  0xf0
#define UART2_RX_BUF_EMPTY  0x0f

	 

#endif /*end of R2000.H*/

