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

#include "websocket.h"

int main()
{     
	int connected = 0;
	while(1)
	{
		memset(g_ws_read_buf, 0, WS_MIN_LEN_READ);  
		//n = read(connfd, buf, REQUEST_LEN_MAX);   
		printf("---------------------\n");  


		if(0 == connected)  
		{  
			compute_accept_key();
			shake_hand();  
			connected=1;  
			continue;  
		}
		response("12345678abcdefj");
	}
}

