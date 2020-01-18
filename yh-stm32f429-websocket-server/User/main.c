/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   DHT11��ʪ�ȴ�������ȡ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F429 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
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

