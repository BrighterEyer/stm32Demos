#ifndef __NFC_H__
#define __NFC_H__

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"


void  nfc_InListPassiveTarget(void);   //���� 106 kbps type A (ISO/IEC14443 Type A),
void  nfc_GetUID(void);                //��ȡ��ǩ UID
void  nfc_PsdVerifyKeyA(void);         // ������Ȩ����֤KeyA
void  nfc_read(void);                  //Ĭ�� �� 02����16���ֽ�

u8 *nfc_GetReadData(void);             //��ȡ��ǩ ����ֵ  Ĭ�ϵڶ��飨16�ֽ����ݣ�
void  nfc_write(u8 data);              //Ĭ���� 02��д 16���ֽڵĵ�һ���ֽ�

//��USART1������ length���ȵ�����data
void SendData(u8 *data,u8 length);

void nfc_WakeUp(void);//����

void CleanBuffer(u16 num);//���� ǰ ���ٸ��ֽڵ�����


extern u8 flag_nfc_status;
extern u8 UID_backup[4];//UID����  ���ڴ��� ������дͬһ��

#endif /* __NFC_H__ */
