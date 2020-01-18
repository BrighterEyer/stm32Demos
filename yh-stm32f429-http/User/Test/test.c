#include "test.h"
#include "./ESP8266/bsp_esp8266.h"
#include "bsp_SysTick.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>  
#include <stdbool.h>
#include "./DHT11/bsp_dht11.h"
#include "http.h"



volatile uint8_t ucTcpClosedFlag = 0;

char cStr [ 1500 ] = { 0 };


void PC_Usart(char* cStr){
	ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 ); 
}

void wifi_init(void){
  printf ( "\r\n�������� ESP8266 ......\r\n" );
	macESP8266_CH_ENABLE();
	ESP8266_AT_Test ();
	ESP8266_Net_Mode_Choose ( STA );
  while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	
	ESP8266_Enable_MultipleId ( DISABLE );
	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
	while ( ! ESP8266_UnvarnishSend () );
	printf ( "\r\n���� ESP8266 ���\r\n" );
}

/**
  * @brief  ESP8266 ��Sta Tcp Client��͸��
  * @param  ��
  * @retval ��
  */
void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
	uint8_t ucStatus;
	char cStr [ 100 ] = { 0 };
  printf ( "\r\n�������� ESP8266 ......\r\n" );
	macESP8266_CH_ENABLE();
	ESP8266_AT_Test ();
	ESP8266_Net_Mode_Choose ( STA );
  while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	
	ESP8266_Enable_MultipleId ( DISABLE );
	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
	while ( ! ESP8266_UnvarnishSend () );
	printf ( "\r\n���� ESP8266 ���\r\n" );
	
	
	while ( 1 )
	{		
		sprintf ( cStr,
"ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n" );
		
		ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //��������
		
		Delay_ms ( 100 );
		
		if ( ucTcpClosedFlag )                                             //����Ƿ�ʧȥ����
		{
			ESP8266_ExitUnvarnishSend ();                                    //�˳�͸��ģʽ
			
			do ucStatus = ESP8266_Get_LinkStatus ();                         //��ȡ����״̬
			while ( ! ucStatus );
			
			if ( ucStatus == 4 )                                             //ȷ��ʧȥ���Ӻ�����
			{
				printf ( "\r\n���������ȵ�ͷ����� ......\r\n" );
				
				while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );
				
				while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
				
				printf ( "\r\n�����ȵ�ͷ������ɹ�\r\n" );

			}
			
			while ( ! ESP8266_UnvarnishSend () );		
			
		}
	}
		
}


void ESP8266_AP_TCP_Server ( void )
{
	char cStrInput [100] = { 0 }, *pStrDelimiter[3], * pBuf, * pStr;
	u8 uc = 0;
	u32 ul = 0;

	macESP8266_CH_ENABLE();

	ESP8266_AT_Test ();

	ESP8266_Net_Mode_Choose ( AP );


	PC_Usart ("\r\n :\
			   \r\n 0 = OPEN\
			   \r\n 1 = WEP\
			   \r\n 2 = WPA_PSK\
			   \r\n 3 = WPA2_PSK\
			   \r\n 4 = WPA_WPA2_PSK\
			   \r\n \r\n" );

	scanf ( "%s", cStrInput );

	PC_Usart ( "\r\n \r\n" );

	pBuf = cStrInput;
	uc = 0;
	while ( ( pStr = strtok ( pBuf, "," ) ) != NULL ) {
		pStrDelimiter [ uc ++ ] = pStr;
		pBuf = NULL;
	}

	ENUM_AP_PsdMode_TypeDef enunPsdMode = WEP;
	ESP8266_BuildAP ( pStrDelimiter[0], pStrDelimiter[2], enunPsdMode );
	ESP8266_Cmd ( "AT+RST", "OK", "ready", 2500 ); //*


	ESP8266_Enable_MultipleId ( ENABLE );


	PC_Usart ( "\r\n \r\n" );

	scanf ( "%s", cStrInput );

	PC_Usart ( "\r\n r\n" );

	pBuf = cStrInput;
	uc = 0;
	while ( ( pStr = strtok ( pBuf, "," ) ) != NULL ) {
		pStrDelimiter [ uc ++ ] = pStr;
		pBuf = NULL;
	}

	ESP8266_StartOrShutServer ( ENABLE, pStrDelimiter[0], pStrDelimiter[1] );


	do {
		PC_Usart ( "\r\n\r\n" );
		ESP8266_Cmd ( "AT+CIFSR", "OK", "Link", 500 );

		PC_Usart ( "\r\n \r\n" );
		Delay_ms ( 20000 ) ;
	} while ( !ESP8266_Cmd ( "AT+CIPSTATUS", "+CIPSTATUS:0", 0, 500 ) );

		PC_Usart ( "\r\n\r\n" );
		scanf ( "%s", cStrInput );
		ul = strlen ( cStrInput );
		ESP8266_SendString ( DISABLE, cStrInput, ul, Multiple_ID_0 );

		PC_Usart ( "\r\n\r\n" );
		while (1) {
		  pStr = ESP8266_ReceiveString( DISABLE );
			PC_Usart( cStr );
		}
}



void ESP8266_StaTcpClient_UnvarnishTest_HTTP ( void )
{
	char *upload_http_data = NULL ;
	char *http_hex_data = NULL ;
	upload_http_data = (char *)malloc(TOTAL_DATA_LEN);
	if (NULL == upload_http_data){
		return ;
	}
	http_hex_data = (char *)malloc(MAX_DATA_LEN);
	if (NULL == http_hex_data){
		return ;
	}

	while (1) {
		memset(upload_http_data, 0, strlen(upload_http_data));
		//��װHTTPͷ�Լ�����
		int ret = Http_Data_Upload(SERVER_IP, SERVER_PORT, "2017/08/15 12:20", "12345678910", "114.0925,225.3369", upload_http_data);
		//���Ҫ�ϴ���HTTP����
		Get_Hex_Http_Packet_Data(upload_http_data, http_hex_data);
		//ͨ��WIFI͸������ʽ�����ݷ��͵�������
		ESP8266_SendString ( ENABLE, upload_http_data, 0, Single_ID_0 );               //����http����
		Delay_ms (5000);
	}
}



/*
* ��������ESP8266_WebFetch_Test
* ���� ��WF-ESP8266 ģ�����ץȡ��ҳ����
* ���� ����
* ���� : ��
* ���� �����ⲿ����
*/
void ESP8266_WebFetch_Test ( void )
{
	uint8_t ucStatus;

  printf ( "\r\n�������� ESP8266 ......\r\n" );
	macESP8266_CH_ENABLE();
	ESP8266_AT_Test ();
	ESP8266_Net_Mode_Choose ( STA );

  while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	
	ESP8266_Enable_MultipleId ( DISABLE );
	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
	while ( ! ESP8266_UnvarnishSend () );
	printf ( "\r\n���� ESP8266 ���\r\n" );
	


	PC_Usart ( "\r\n �������Է��� ��eat.weixincore.com/index.php?s=/Mobile/PrinterTest/index��Ϊ��\r\n" );

	while ( 1 )
	{

		/*
		sprintf ( cStr,
"ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n" );
		ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //��������
		*/

	  ESP8266_SendString ( ENABLE, "GET index.php?s=/Mobile/PrinterTest/index HTTP/1.1\r\n", NULL, Single_ID_0 );
	  ESP8266_SendString ( ENABLE, "Host: eat.weixincore.com\r\n", NULL, Single_ID_0 );
	  ESP8266_SendString ( ENABLE, "User-Agent: abc\r\n", NULL, Single_ID_0 );
	  ESP8266_SendString ( ENABLE, "Connection: close\r\n", NULL, Single_ID_0 );
	  ESP8266_SendString ( ENABLE, "\r\n", NULL, Single_ID_0 );
	  USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
		Delay_ms ( 100 );


		if ( ucTcpClosedFlag )                                             //����Ƿ�ʧȥ����
		{
			ESP8266_ExitUnvarnishSend ();                                    //�˳�͸��ģʽ
			
			do ucStatus = ESP8266_Get_LinkStatus ();                         //��ȡ����״̬
			while ( ! ucStatus );

			if ( ucStatus == 4 )                                             //ȷ��ʧȥ���Ӻ�����
			{
				printf ( "\r\n���������ȵ�ͷ����� ......\r\n" );
				
				while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );
				
				while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
				
				printf ( "\r\n�����ȵ�ͷ������ɹ�\r\n" );

			}
			
			while ( ! ESP8266_UnvarnishSend () );		
			
		}
	}


}


