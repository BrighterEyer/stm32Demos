/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   DHT11温湿度传感器读取。
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F429 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "systick/bsp_SysTick.h"
#include "DHT11/bsp_dht11.h"
#include "bsp_SysTick.h"
#include "./ESP8266/bsp_esp8266.h"
#include "./test/test.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MQTTPacket.h"
#include "transport.h"
#include <signal.h>


int toStop = 0;

void cfinish(int sig)
{
	signal(SIGINT, NULL);
	toStop = 1;
}

void stop_init(void)
{
	signal(SIGINT, cfinish);
	signal(SIGTERM, cfinish);
}

char* itoa(int value,char*string,int radix);

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  

   /*初始化USART，配置模式为 115200 8-N-1*/
   Debug_USART_Config();
 	 /* 系统定时器初始化 */
	 SysTick_Init();

   ESP8266_Init(); 
   LED_GPIO_Config();
	 Key_GPIO_Config();

	 /* DHT11初始化 */
	 DHT11_GPIO_Config();

	 printf("\r\n***野火F429挑战者APP控制开发板实验***\r\n");
   //ESP8266_StaTcpClient_UnvarnishTest();
	 ESP8266_MQTT_UnvarnishTest();

	return 0;

}


char* itoa(int num,char* str,int radix)
{
	char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//
	unsigned unum;//
	int i=0,j,k;//
 
	//
	if(radix==10&&num<0)//
	{
		unum=(unsigned)-num;//
		str[i++]='-';//
	}
	else unum=(unsigned)num;//
 
	do
	{
		str[i++]=index[unum%(unsigned)radix];//
		unum/=radix;//
 
	}while(unum);//
 
	str[i]='\0';//
 
	//
	if(str[0]=='-') k=1;//
	else k=0;//
 
	char temp;//
	for(j=k;j<=(i-1)/2;j++)//
	{
		temp=str[j];//
		str[j]=str[i-1+k-j];//
		str[i-1+k-j]=temp;
	}
 
	return str;
}

/*********************************************END OF FILE**********************/

