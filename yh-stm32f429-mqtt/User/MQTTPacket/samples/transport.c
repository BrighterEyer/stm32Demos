/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Sergio R. Caprile - "commonalization" from prior samples and/or documentation extension
 *******************************************************************************/

#include "stm32f4xx.h"
#include <string.h>
#include "./usart/bsp_debug_usart.h"
#include "transport.h"
#include "systick/bsp_SysTick.h"
#include "./ESP8266/bsp_esp8266.h"


#if !defined(SOCKET_ERROR)
	#define SOCKET_ERROR -1
#endif

/*
#define FD_SETSIZE 1024
#define MAXHOSTNAMELEN 256
#define EAGAIN WSAEWOULDBLOCK
#define EINTR WSAEINTR
#define EINVAL WSAEINVAL
#define EINPROGRESS WSAEINPROGRESS
#define EWOULDBLOCK WSAEWOULDBLOCK
#define ENOTCONN WSAENOTCONN
#define ECONNRESET WSAECONNRESET
#define ioctl ioctlsocket
#define socklen_t int
*/

static int mysock = -1;
static int first_index_to_read = 0;

static int END_CHAR_ERROR = -1;
static int AT_CIPSEND_ERROR = -2;
static int AT_CGATT_ERROR = -3;
static int AT_CGATT1_ERROR = -4;
static int AT_CIPSTART_ERROR = -5;


int transport_sendPacketBuffer(const char* server_ip_and_port,unsigned char* buf, int buflen)
{
    u8 ret;
    char end_char[2];
    char connect_server_ip_port_cmd[56];


    first_index_to_read = 21;

    end_char[0] = 0x1A;//
    end_char[1] = '\0';
	

	  //wifi连接状态
    ret = ESP8266_Cmd("AT+CIPSTATUS","CONNECT OK","3",2500);//
	
	  printf("wifi连接状态 %d\r\n",ret);

	  //wifi已连接 ret == 1
    if(ret == 1)
    {
        ret = ESP8266_Cmd("AT+CIPSEND",">","3",2500);//

        if(ret == 0) {
            return AT_CIPSEND_ERROR;
        }

        Usart_SendByte((USART_TypeDef*)buf,buflen);
        Delay_ms(2000);
        ret = ESP8266_Cmd(end_char,"SEND OK","4",2500);

        if(ret == 0)
        {
            return END_CHAR_ERROR;
        }

        return 1;
    }

    ret = ESP8266_Cmd("AT+CGATT=1","OK","2",2500);
    if(ret == 0)
    {
        return AT_CGATT_ERROR;
    }

    ret = ESP8266_Cmd("AT+CGACT=1,1","OK","2",2500);
    if(ret == 0)
    {
        return AT_CGATT1_ERROR;
    }

    memset(connect_server_ip_port_cmd,'\0',56);
    strcpy(connect_server_ip_port_cmd,"AT+CIPSTART=\"TCP\",");
    strcat(connect_server_ip_port_cmd,server_ip_and_port);

    ret = ESP8266_Cmd(connect_server_ip_port_cmd,"CONNECT OK","4",2500);

    if(ret == 0)
    {
        return AT_CIPSTART_ERROR;
    }

    ret = ESP8266_Cmd("AT+CIPSEND",">","3",2500);

    if(ret == 0) {
        return AT_CIPSEND_ERROR;
    }

    Usart_SendByte((USART_TypeDef*)buf,buflen);
    Delay_ms(2000);
    ret = ESP8266_Cmd(end_char,"SEND OK","4",2500);
    if(ret == 0) {
         return END_CHAR_ERROR;
    }
    return 1;
}

int transport_getdata(unsigned char* buf, int count)
{
	  int len = usart1_getstr(buf,count);
    first_index_to_read += count;
    return count;
}

