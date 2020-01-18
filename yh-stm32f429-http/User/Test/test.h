#ifndef  __TEST_H
#define	 __TEST_H



#include "stm32f4xx.h"



/********************************** �û���Ҫ���õĲ���**********************************/
#define      macUser_ESP8266_ApSsid                       "RDCenter"                //Ҫ���ӵ��ȵ������
#define      macUser_ESP8266_ApPwd                        "dchip2019"           //Ҫ���ӵ��ȵ����Կ

#define      macUser_ESP8266_TcpServer_IP                 "139.199.7.215"      //Ҫ���ӵķ������� IP
#define      macUser_ESP8266_TcpServer_Port               "88"               //Ҫ���ӵķ������Ķ˿�

//#define      macUser_ESP8266_TcpServer_IP                 "192.168.1.156"      //Ҫ���ӵķ������� IP
//#define      macUser_ESP8266_TcpServer_Port               "8080"               //Ҫ���ӵķ������Ķ˿�



/********************************** �ⲿȫ�ֱ��� ***************************************/
extern volatile uint8_t ucTcpClosedFlag;



/********************************** ���Ժ������� ***************************************/
void                     ESP8266_StaTcpClient_UnvarnishTest  ( void );
void                     ESP8266_AP_TCP_Server               ( void );
void                     ESP8266_StaTcpClient_UnvarnishTest_HTTP ( void );
void                     ESP8266_WebFetch_Test               ( void );
void                     wifi_init                           ( void );


#endif

