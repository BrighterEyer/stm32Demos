#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "nfc.h"
//ALIENTEK Mini STM32�����巶������
//
//����NFC�������д��ʵ��
// ����NFC���� ��www.heybird.net
/********************************************************************
*  �������裺                                                       *
*  1����¼�ó���ʹ��ս������塣MCU��stm32RCT6                    *
*  2������1���ӿ���NFC������                                        *
*  3���������򣬵�D0��D1��ͬʱ���𣬿���NFC�����廽�ѳɹ�           *
*  4��������key0����ʱ D0������Ҳ���� �������𣬴�ʱ���ϱ�ǩ2��   *
*  5��������WK_UP,��ʱ D0������Ҳ���� �����������ʱ���ϱ�ǩ��D1��*
*  6��������key1,��ʱ D0������Ҳ���� �������𣬴�ʱ���ϱ�ǩ2��    *
*  7��������WK_UP,��ʱ D0������Ҳ���� �����������ʱ���ϱ�ǩ��D1��*
*                                                                   *
*                                                                   *
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*  ˵����                                                           *
*   1.ÿ��D0��״̬�ı仯��˵���������³ɹ�                          *
*   2.key0��ʾд��ǩ��д��0x55��                                    *
*   3.key1��ʾд��ǩ��д��0xAA��                                    *
*   4.WK_UP��ʾ����ǩ������0x55��ʹD1�𣻶���0xAA��ʹD1��           *
*********************************************************************/


extern u8 flag_nfc_status;


int main(void)
{
	u8 t = 0;
	delay_init();	    	 //��ʱ������ʼ��
	NVIC_Configuration();// �����ж����ȼ�����
	TIM2_Configuration();
	uart_init(115200);	 //���ڳ�ʼ��Ϊ9600
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();          	//��ʼ���밴�����ӵ�Ӳ���ӿ�

	nfc_WakeUp();//���� PN532 оƬ
	LED0 = 0;
	LED1 = 0;
	while (1)
	{
		t = KEY_Scan(0);		//�õ���ֵ
		switch (t)
		{
		case KEY0_PRES:
			LED0 = !LED0;
			flag_nfc_status = 1; //KEY0  ��nfcд0x55.����
			break;
		case KEY1_PRES:
			LED0 = !LED0;
			flag_nfc_status = 2; //KEY1 ��nfcд0xAA������
			break;
		case WKUP_PRES:
			LED0 = !LED0;
			flag_nfc_status = 0; //WKUP_PRES ��nfc��״̬
			break;
		default:
			delay_ms(10);
		}
		nfc_InListPassiveTarget(); //Ѱ�� nfc��ǩ��Mifare ��������ȡUID
		nfc_PsdVerifyKeyA();

	}
}


