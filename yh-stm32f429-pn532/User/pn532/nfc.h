#ifndef __NFC_H__
#define __NFC_H__

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"


void  nfc_InListPassiveTarget(void);   //配置 106 kbps type A (ISO/IEC14443 Type A),
void  nfc_GetUID(void);                //获取标签 UID
void  nfc_PsdVerifyKeyA(void);         // 密码授权，验证KeyA
void  nfc_read(void);                  //默认 读 02区的16个字节

u8 *nfc_GetReadData(void);             //获取标签 数据值  默认第二块（16字节数据）
void  nfc_write(u8 data);              //默认往 02区写 16个字节的第一个字节

//往USART1，发送 length长度的数据data
void SendData(u8 *data,u8 length);

void nfc_WakeUp(void);//唤醒

void CleanBuffer(u16 num);//清零 前 多少个字节的数据


extern u8 flag_nfc_status;
extern u8 UID_backup[4];//UID备份  用于处理 不连续写同一卡

#endif /* __NFC_H__ */
