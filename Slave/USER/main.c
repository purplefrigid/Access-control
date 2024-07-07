#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
 

//**** 声明 ********************************************************************
/*******************************************************************************
 * 下面来自互联开源程序，由广州汇承信息科技有限公司收集
 * 方便用户参考学习，本公司不对单片机做编程指导，请用户自行研究
 * 程序仅供测试参考，不能应用在实际工程中，不一定能通过编译
 * 公司网站  http://www.hc01.com/
 * 淘宝网址  http://hc-com.taobao.com/
*******************************************************************************/
//int k;
int times;
 int main(void)
 {	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600
 	LED_Init();			     //LED端口初始化
	printf("111111");
	TIM3_PWM_Init(199,7199);
 	while(1)
	{
		//k=0;
		
	}
	
 }

