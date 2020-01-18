#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "nfc.h"
//ALIENTEK Mini STM32开发板范例代码
//
//科星NFC开发板读写卡实验
// 更多NFC资料 ：www.heybird.net
/********************************************************************
*  操作步骤：                                                       *
*  1、烧录好程序，使用战舰迷你板。MCU：stm32RCT6                    *
*  2、串口1连接科星NFC开发板                                        *
*  3、启动程序，当D0和D1，同时亮起，科星NFC开发板唤醒成功           *
*  4、按按键key0，此时 D0，反向，也就是 由亮变灭，此时放上标签2秒   *
*  5、按按键WK_UP,此时 D0，反向，也就是 由灭变亮，此时放上标签，D1灭*
*  6、按按键key1,此时 D0，反向，也就是 由亮变灭，此时放上标签2秒    *
*  7、按按键WK_UP,此时 D0，反向，也就是 由灭变亮，此时放上标签，D1亮*
*                                                                   *
*                                                                   *
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*  说明：                                                           *
*   1.每次D0灯状态的变化，说明按键按下成功                          *
*   2.key0表示写标签，写入0x55；                                    *
*   3.key1表示写标签，写入0xAA；                                    *
*   4.WK_UP表示读标签，读到0x55，使D1灭；读到0xAA，使D1亮           *
*********************************************************************/


extern u8 flag_nfc_status;


int main(void)
{
	u8 t = 0;
	delay_init();	    	 //延时函数初始化
	NVIC_Configuration();// 设置中断优先级分组
	TIM2_Configuration();
	uart_init(115200);	 //串口初始化为9600
	LED_Init();		  	 //初始化与LED连接的硬件接口
	KEY_Init();          	//初始化与按键连接的硬件接口

	nfc_WakeUp();//唤醒 PN532 芯片
	LED0 = 0;
	LED1 = 0;
	while (1)
	{
		t = KEY_Scan(0);		//得到键值
		switch (t)
		{
		case KEY0_PRES:
			LED0 = !LED0;
			flag_nfc_status = 1; //KEY0  ，nfc写0x55.灯灭
			break;
		case KEY1_PRES:
			LED0 = !LED0;
			flag_nfc_status = 2; //KEY1 ，nfc写0xAA。灯亮
			break;
		case WKUP_PRES:
			LED0 = !LED0;
			flag_nfc_status = 0; //WKUP_PRES ，nfc读状态
			break;
		default:
			delay_ms(10);
		}
		nfc_InListPassiveTarget(); //寻找 nfc标签（Mifare 卡），获取UID
		nfc_PsdVerifyKeyA();

	}
}


