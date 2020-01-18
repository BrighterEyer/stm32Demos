#include "nfc.h"
#include "stdio.h"
#include "bsp_SysTick.h"
#include "bsp_led.h"


/*********************************************************************
*  操作步骤：                                                        *
*  1、烧录好程序，使用战舰迷你板。MCU：stm32RCT6                     *
*  2、串口1连接科星NFC开发板                                         *
*  3、启动程序，当D0和D1，同时亮起，科星NFC开发板唤醒成功            *
*  4、按按键key0，此时 D0，反向，也就是 由亮变灭，此时放上标签2秒    *
*  5、按按键WK_UP,此时 D0，反向，也就是 由灭变亮，此时放上标签，D1灭 *
*  6、按按键key1, 此时 D0，反向，也就是 由亮变灭，此时放上标签2秒     *
*  7、按按键WK_UP,此时 D0，反向，也就是 由灭变亮，此时放上标签，D1亮 *
*                                                                    *
*                                                                    *
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*  说明：                                                            *
*   1.每次D0灯状态的变化，说明按键按下成功                           *
*   2.key0表示写标签，写入0x55；                                     *
*   3.key1表示写标签，写入0xAA；                                     *
*   4.WK_UP表示读标签，读到0x55，使D1灭；读到0xAA，使D1亮            *
*********************************************************************/

#define u8 unsigned char

extern u8 RxBuffer1[200];
extern u8 flag_1; //第一次 读出 UID码成功
extern u8 flag_2;
extern u8 flag_3;
extern u8 flag_rev_finish;//串口接收到数据的标志， 1为接收到
extern u8 flag_nfc_status;
u8 UID[4]; //存储 UID
extern u8 UID_backup[4];//UID备份  用于处理 不连续写同一卡

////55 55 00 00 00 00 00 00 00 00 00 00 00 00 00 00 FF 03 FD D4 14 01 17 00
void nfc_WakeUp(void)
{
    u8 data[24];
    u8 i;
    u8 CheckCode = 0; //数据校验码
    u8 temp = 0;

    while (1)
    {
        flag_rev_finish = 0;
        data[0] = 0x55;
        data[1] = 0x55;
        data[2] = 0x00;
        data[3] = 0x00;
        data[4] = 0x00;
        data[5] = 0x00;
        data[6] = 0x00;
        data[7] = 0x00;
        data[8] = 0x00;
        data[9] = 0x00;
        data[10] = 0x00;
        data[11] = 0x00;
        data[12] = 0x00;
        data[13] = 0x00;
        data[14] = 0x00;
        data[15] = 0x00;

        data[16] = 0xFF;
        data[17] = 0x03; //包 长度
        data[18] = 0xFD; //包 长度 校验  0x100-data[3]
        data[19] = 0xD4; //命令标识码
        data[20] = 0x14; //命令标识码
        data[21] = 0x01;
        data[22] = 0x17; //数据 校验   0x100-
        data[23] = 0x00;

        SendData(data, 24); //往USART1，发送 length长度的数据data
        Delay_ms(380);


        //00 00 FF 00 FF 00 00 00 FF 02 FE D5 15 16 00
        if (flag_rev_finish)
        {
            flag_rev_finish = 0;
            for (i = 11; i < 13; i++)
            {
                temp += RxBuffer1[i];
            }
            CheckCode = 0x100 - temp;
            if (CheckCode == RxBuffer1[13])
            {
                CheckCode = 0x100 - temp;
                CleanBuffer(40);//清除 串口接收缓冲区前30 个字节数据

                break;
            }
        }
    }
}

//配置 106 kbps type A (ISO/IEC14443 Type A),
//00 00 FF 04 FC D4 4A 01 00 E1 00
void  nfc_InListPassiveTarget(void)
{
    u8 data[11];
    u8 i;
    u8 CheckCode = 0; //数据校验码
    u8 temp = 0;
    while (1)
    {
        flag_rev_finish = 0;
        data[0] = 0x00;
        data[1] = 0x00;
        data[2] = 0xFF;
        data[3] = 0x04; //包 长度
        data[4] = 0xFC; //包 长度 校验  0x100-data[3]
        data[5] = 0xD4; //命令标识码
        data[6] = 0x4A; //命令标识码
        data[7] = 0x01;
        data[8] = 0x00;
        data[9] = 0xE1; //数据 校验   0x100-
        data[10] = 0x00;

        SendData(data, 11); //往USART1，发送 length长度的数据data
        Delay_ms(180);

        temp = 0;
        if (flag_rev_finish)
        {
            flag_rev_finish = 0;
            for (i = 11; i < 23; i++)
            {
                temp += RxBuffer1[i];
            }
            CheckCode = 0x100 - temp;
            if (CheckCode == RxBuffer1[23])
            {
                UID[0] = RxBuffer1[19];
                UID[1] = RxBuffer1[20];
                UID[2] = RxBuffer1[21];
                UID[3] = RxBuffer1[22];

                //printf(UID);

                CleanBuffer(40);//清除 串口接收缓冲区前30 个字节数据
                break;
            }
        }
    }
}


// 密码授权，验证KeyA
//00 00 FF 0F F1 D4 40 01 60 03 FF FF FF FF FF FF UID1 UID2 UID3 UID4 2A 00
//00 00 FF 0F F1 D4 40 01 60 03 FF FF FF FF FF FF 94 8A 3B 0B 2A 00
void  nfc_PsdVerifyKeyA(void)
{
    u8 data[22];
    u8 temp = 0;
    u8 i;
    u8 CheckCode = 0; //数据校验码
    flag_rev_finish = 0;
    data[0] = 0x00;
    data[1] = 0x00;
    data[2] = 0xFF;

    data[3] = 0x0F; //包 长度
    data[4] = 0xF1; //包 长度 校验  0x100-data[3]

    data[5] = 0xD4; //命令标识码
    data[6] = 0x40; //命令标识码

    data[7] = 0x01;
    data[8] = 0x60;
    data[9] = 0x03;

    data[10] = 0xFF; //KEY A 密码 FF FF FF FF FF FF
    data[11] = 0xFF;
    data[12] = 0xFF;
    data[13] = 0xFF;
    data[14] = 0xFF;
    data[15] = 0xFF;

    data[16] = UID[0];
    data[17] = UID[1];
    data[18] = UID[2];
    data[19] = UID[3];

    for (i = 5; i < 20; i++)
    {
        temp += data[i];
    }

    data[20] = 0x100 - temp; //数据 校验   0x100-

    data[21] = 0x00;

    SendData(data, 22); //往USART1，发送 length长度的数据data



    Delay_ms(180);

    // while(!flag_rev_finish)
    //  ;
    // flag_rev_finish=0;

    temp = 0;
    for (i = 11; i < 14; i++)
    {
        temp += RxBuffer1[i];
    }
    CheckCode = 0x100 - temp;
    if (CheckCode == RxBuffer1[14])
    {
        if (RxBuffer1[13] == 0x00)
        {
            CleanBuffer(40);//清除 串口接收缓冲区前40 个字节数据

            if (flag_nfc_status == 0)
            {
                nfc_read();
            }
            else if (flag_nfc_status == 1)
            {
                nfc_write(0x55);
            }
            else if (flag_nfc_status == 2)
            {
                nfc_write(0xAA);
            }

        }
    }
}



//默认 读 02区的16个字节
//00 00 FF 05 FB D4 40 01 30 02 B9 00
void  nfc_read()
{
    u8 data[12];
    u8 temp = 0;
    u8 i;
    u8 CheckCode = 0; //数据校验码

    flag_rev_finish = 0;
    data[0] = 0x00;
    data[1] = 0x00;
    data[2] = 0xFF;

    data[3] = 0x05; //包 长度
    data[4] = 0xFB; //包 长度 校验  0x100-data[3]

    data[5] = 0xD4; //命令标识码
    data[6] = 0x40; //命令标识码

    data[7] = 0x01;
    data[8] = 0x30;
    data[9] = 0x02; //读第二块的16字节数据

    data[10] = 0xB9; //数据校验
    data[11] = 0x00;

    SendData(data, 12); //往USART1，发送 length长度的数据data

    Delay_ms(180);
    //while(!flag_rev_finish)
    // ;
    // flag_rev_finish=0;

    temp = 0;
    for (i = 11; i < 30; i++)
    {
        temp += RxBuffer1[i];
    }
    CheckCode = 0x100 - temp;
    if (CheckCode == RxBuffer1[30])
    {
        if (RxBuffer1[14] == 0xAA)
        {
					GPIO_WriteBit(GPIOB, GPIO_Pin_5, 0); 
        }
        else if (RxBuffer1[14] == 0x55)
        {
					GPIO_WriteBit(GPIOB, GPIO_Pin_5, 1); 
        }

        if ((UID_backup[0] != UID[0]) | (UID_backup[1] != UID[1]) | (UID_backup[2] != UID[2]) | (UID_backup[3] != UID[3]))
        {
            GPIO_ResetBits(GPIOD, GPIO_Pin_3); //蜂鸣器 叫
            Delay_ms(150);
            GPIO_SetBits(GPIOD, GPIO_Pin_3);//蜂鸣器 不叫
        }
        UID_backup[0] = UID[0];
        UID_backup[1] = UID[1];
        UID_backup[2] = UID[2];
        UID_backup[3] = UID[3];

        CleanBuffer(40);//清除 串口接收缓冲区前40 个字节数据
    }
}


//默认往 02区写 16个字节的第一个字节
//00 00 FF 15 EB D4 40 01 A0 02 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F D1 00
void  nfc_write(u8 LedData)
{
    u8 data[28];
    u8 temp = 0;
    u8 i;
    u8 CheckCode = 0;

    data[0] = 0x00;
    data[1] = 0x00;
    data[2] = 0xFF;

    data[3] = 0x15; //包 长度
    data[4] = 0xEB; //包 长度 校验  0x100-data[3]

    data[5] = 0xD4; //命令标识码
    data[6] = 0x40; //命令标识码

    data[7] = 0x01; //读写大于6字节 就置1，看手册
    data[8] = 0xA0; //写
    data[9] = 0x02; //写第二块的16字节数据

    data[10] = LedData; //第 1 字节 数据
    data[11] = 0x00;
    data[12] = 0x00; //第 3 字节 数据
    data[13] = 0x00;
    data[14] = 0x00; //第 5 字节 数据
    data[15] = 0x00;
    data[16] = 0x00; //第 7 字节 数据
    data[17] = 0x00;
    data[18] = 0x00; //第 9 字节 数据
    data[19] = 0x00;
    data[20] = 0x00; //第 11 字节 数据
    data[21] = 0x00;
    data[22] = 0x00; //第 13 字节 数据
    data[23] = 0x00;
    data[24] = 0x00; //第 15 字节 数据
    data[25] = 0x00;

    for (i = 5; i < 26; i++)
    {
        temp += data[i];
    }
    data[26] = 0x100 - temp; //数据 校验码
    data[27] = 0x00;

    SendData(data, 28); //往USART1，发送 length长度的数据data


    Delay_ms(180);
    //while(!flag_rev_finish)
    // ;
    // flag_rev_finish=0;
    //00 00 FF 00 FF 00 00 00 FF 03 FD D5 41 00 EA 00
    temp = 0;
    for (i = 11; i < 14; i++)
    {
        temp += RxBuffer1[i];
    }
    CheckCode = 0x100 - temp;
    if (CheckCode == RxBuffer1[14])
    {
        CleanBuffer(40);//清除 串口接收缓冲区前40 个字节数据

        if ((UID_backup[0] != UID[0]) | (UID_backup[1] != UID[1]) | (UID_backup[2] != UID[2]) | (UID_backup[3] != UID[3]))
        {
            GPIO_ResetBits(GPIOD, GPIO_Pin_3); //蜂鸣器 叫
            Delay_ms(10);
            GPIO_SetBits(GPIOD, GPIO_Pin_3);//蜂鸣器 不叫

        }
        UID_backup[0] = UID[0];
        UID_backup[1] = UID[1];
        UID_backup[2] = UID[2];
        UID_backup[3] = UID[3];
    }
}

//往USART1，发送 length长度的数据data
void SendData(u8 *data, u8 length)
{
    u8 i;
    for (i = 0; i < length; i++)
    {
        USART_SendData(USART1, data[i]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) //等得发送完成
        {
        }
    }
}

void CleanBuffer(u16 num)//清零 前 多少个字节的数据
{
    u16 i = 0;

    for (i = 0; i < num; i++)
        RxBuffer1[i] = 0x00;
}




