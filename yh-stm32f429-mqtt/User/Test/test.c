#include "test.h"
#include "./ESP8266/bsp_esp8266.h"
#include "bsp_SysTick.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "./DHT11/bsp_dht11.h"

#include "bsp_key.h"
#include "MQTTPacket.h"
#include "transport.h"
#include <signal.h>


volatile uint8_t ucTcpClosedFlag = 0;

char cStr [ 1500 ] = { 0 };



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

#define __wifi_init__ \
	uint8_t ucStatus; \
	printf ( "\r\n�������� ESP8266 ......\r\n" ); \
	macESP8266_CH_ENABLE(); \
	ESP8266_AT_Test (); \
	ESP8266_Net_Mode_Choose ( STA ); \
	while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) ); \
	ESP8266_Enable_MultipleId ( DISABLE ); \
	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) ); \
	while ( ! ESP8266_UnvarnishSend () ); \
	printf ( "\r\n���� ESP8266 ���\r\n" );


int ESP8266_MQTT_UnvarnishTest( void ) {
	
	__wifi_init__


	const char* MQTT_SERVER_IP_AND_PORT = "47.98.119.123:1883";
	int MQTT_MAX_BUF_SIZE = 100;
	int DelayTimes = 1000;

	u16 key_value = 0;
	u16 retry_count = 5;
	unsigned short submsgid;
	unsigned char buf[MQTT_MAX_BUF_SIZE];
	unsigned char sessionPresent;
	unsigned char connack_rc;

	const char* payload = "64";
	int payloadlen = strlen(payload);
	int ret = 0, len = 0, req_qos = 0, msgid = 1, loop = 1, granted_qos, subcount;

	MQTTString topicString = MQTTString_initializer;
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

	Delay_ms(DelayTimes);

	data.clientID.cstring = "mtopic_id";
	data.keepAliveInterval = 20;
	data.cleansession = 1;
	data.username.cstring = "admin";
	data.password.cstring = "hello";

	len = MQTTSerialize_connect(buf, MQTT_MAX_BUF_SIZE, &data);
	
	/*
	static int first_index_to_read = 0;
  static int AT_CIPSEND_ERROR = -2;
  static int AT_CGATT_ERROR = -3;
  static int AT_CGATT1_ERROR = -4;
  static int AT_CIPSTART_ERROR = -5;
  */

	char cStr [ 1500 ] = { 0 };
	sprintf ( cStr, "�ṩ�Ļ��������ֽڳ��� len %d\n", len);
	ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );                 //��������

	//���ͷ���
	ret = transport_sendPacketBuffer(MQTT_SERVER_IP_AND_PORT, buf, len);
	if ( ret != 1 ) {
		sprintf(cStr, "���䷢�Ͱ������� transport_sendPacketBuffer Error %d\n", ret);
		ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //��������
		return -1;
	}

	/* wait for connack */
	if (MQTTPacket_read(buf, MQTT_MAX_BUF_SIZE, transport_getdata) != CONNACK)
	{
		sprintf ( cStr, "MQTTPacket_read != CONNACK\n");
		ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //��������
		return -1;
	}

	if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, MQTT_MAX_BUF_SIZE) != 1 || connack_rc != 0)
	{
		sprintf( cStr, "Unable to connect, return code %d\n", connack_rc);
		ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //��������
		return -1;
	}
	
	//��������
	topicString.cstring = "mtopic";
	len = MQTTSerialize_subscribe(buf, MQTT_MAX_BUF_SIZE, 0, msgid, 1, &topicString, &req_qos);

	sprintf( cStr, "���ķ��ص����ݳ��� len %d\n", len);
	ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );                 //��������

	//���ķ������ݽ��״̬
	ret = transport_sendPacketBuffer(MQTT_SERVER_IP_AND_PORT, buf, len);
	if ( ret != 1 ) {
		sprintf( cStr, "2222 transport_sendPacketBuffer Error %d\n", ret);
		ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //��������
		return -1;
	}

	if (MQTTPacket_read(buf, MQTT_MAX_BUF_SIZE, transport_getdata) != SUBACK) {   /* wait for suback */
		sprintf( cStr, "MQTTPacket_read != SUBACK\n");
		ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //��������
		return -1;
	}

	//���ķ���
	MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, MQTT_MAX_BUF_SIZE);
	if (granted_qos != 0) {
		sprintf( cStr, "granted qos != 0, %d\n", granted_qos);
		ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //��������
		return -1;
	}

	//��������
	topicString.cstring = "mtopic";

	while ( loop ) {

		key_value = Key_Scan(KEY1_GPIO_PORT, KEY1_PIN);

		sprintf( cStr, "��ֵ key_value %d\r\n", key_value);
		ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );        //��������

		// KEY1 PA5
		if ( key_value == KEY1_PIN ) {
			sprintf( cStr, "Key1 is pressed down , exit \r\n");
			ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );      //��������
			Delay_ms ( 1000 );
			loop = 0;
		}
		/* transport_getdata() has a built-in 1 second timeout, your mileage will vary */
		if (MQTTPacket_read(buf, MQTT_MAX_BUF_SIZE, transport_getdata) == PUBLISH)
		{
			int qos, payloadlen_in;
			unsigned char dup, retained;
			unsigned short msgid;
			unsigned char* payload_in;
			MQTTString receivedTopic;

			MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic, &payload_in, &payloadlen_in, buf, MQTT_MAX_BUF_SIZE);
			sprintf( cStr, "message arrived %.*s\n", payloadlen_in, payload_in);
			ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );    //��������
			Delay_ms ( 100 );
		}

    //���ڶ�ȡ����������
		sprintf( cStr, "publishing reading\n");
		ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );    //��������
		Delay_ms ( 100 );

		len = MQTTSerialize_publish(buf, MQTT_MAX_BUF_SIZE, 0, 0, 0, 0, topicString, (unsigned char*)payload, payloadlen);

    //�������ݳ���
		sprintf( cStr, "33333 len %d\n", len);
		ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );    //��������
		Delay_ms ( 1000 );


		while ( retry_count > 0 ) {
			ret = transport_sendPacketBuffer(MQTT_SERVER_IP_AND_PORT, buf, len);
			if ( ret == 1 ) {
				break;
			}
			Delay_ms ( 1000 );
		}
		if ( !retry_count && ret != 1 ) {
			sprintf( cStr, "3333 transport_sendPacketBuffer Error %d\n", ret);
			ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );    //��������
			Delay_ms ( 1000 );
			break;
		}
		Delay_ms ( 2000 );
	}

	//sprintf( cStr, "disconnecting\n");
	sprintf( cStr, "���ڶϿ�����\n");
	ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );    //��������
	Delay_ms ( 1000 );

	len = MQTTSerialize_disconnect(buf, MQTT_MAX_BUF_SIZE);
	ret = transport_sendPacketBuffer(MQTT_SERVER_IP_AND_PORT, buf, len);
	if ( ret != 1 ) {
		sprintf( cStr, "4444 transport_sendPacketBuffer Error %d\n", ret);
		ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );    //��������
		Delay_ms ( 1000 );
		return -1;
	}
	return 0;

}


