#include "nfc.h"
#include "stdio.h"
#include "bsp_SysTick.h"
#include "bsp_led.h"


/*********************************************************************
*  �������裺                                                        *
*  1����¼�ó���ʹ��ս������塣MCU��stm32RCT6                     *
*  2������1���ӿ���NFC������                                         *
*  3���������򣬵�D0��D1��ͬʱ���𣬿���NFC�����廽�ѳɹ�            *
*  4��������key0����ʱ D0������Ҳ���� �������𣬴�ʱ���ϱ�ǩ2��    *
*  5��������WK_UP,��ʱ D0������Ҳ���� �����������ʱ���ϱ�ǩ��D1�� *
*  6��������key1, ��ʱ D0������Ҳ���� �������𣬴�ʱ���ϱ�ǩ2��     *
*  7��������WK_UP,��ʱ D0������Ҳ���� �����������ʱ���ϱ�ǩ��D1�� *
*                                                                    *
*                                                                    *
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*  ˵����                                                            *
*   1.ÿ��D0��״̬�ı仯��˵���������³ɹ�                           *
*   2.key0��ʾд��ǩ��д��0x55��                                     *
*   3.key1��ʾд��ǩ��д��0xAA��                                     *
*   4.WK_UP��ʾ����ǩ������0x55��ʹD1�𣻶���0xAA��ʹD1��            *
*********************************************************************/

#define u8 unsigned char

extern u8 RxBuffer1[200];
extern u8 flag_1; //��һ�� ���� UID��ɹ�
extern u8 flag_2;
extern u8 flag_3;
extern u8 flag_rev_finish;//���ڽ��յ����ݵı�־�� 1Ϊ���յ�
extern u8 flag_nfc_status;
u8 UID[4]; //�洢 UID
extern u8 UID_backup[4];//UID����  ���ڴ��� ������дͬһ��

////55 55 00 00 00 00 00 00 00 00 00 00 00 00 00 00 FF 03 FD D4 14 01 17 00
void nfc_WakeUp(void)
{
    u8 data[24];
    u8 i;
    u8 CheckCode = 0; //����У����
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
        data[17] = 0x03; //�� ����
        data[18] = 0xFD; //�� ���� У��  0x100-data[3]
        data[19] = 0xD4; //�����ʶ��
        data[20] = 0x14; //�����ʶ��
        data[21] = 0x01;
        data[22] = 0x17; //���� У��   0x100-
        data[23] = 0x00;

        SendData(data, 24); //��USART1������ length���ȵ�����data
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
                CleanBuffer(40);//��� ���ڽ��ջ�����ǰ30 ���ֽ�����

                break;
            }
        }
    }
}

//���� 106 kbps type A (ISO/IEC14443 Type A),
//00 00 FF 04 FC D4 4A 01 00 E1 00
void  nfc_InListPassiveTarget(void)
{
    u8 data[11];
    u8 i;
    u8 CheckCode = 0; //����У����
    u8 temp = 0;
    while (1)
    {
        flag_rev_finish = 0;
        data[0] = 0x00;
        data[1] = 0x00;
        data[2] = 0xFF;
        data[3] = 0x04; //�� ����
        data[4] = 0xFC; //�� ���� У��  0x100-data[3]
        data[5] = 0xD4; //�����ʶ��
        data[6] = 0x4A; //�����ʶ��
        data[7] = 0x01;
        data[8] = 0x00;
        data[9] = 0xE1; //���� У��   0x100-
        data[10] = 0x00;

        SendData(data, 11); //��USART1������ length���ȵ�����data
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

                CleanBuffer(40);//��� ���ڽ��ջ�����ǰ30 ���ֽ�����
                break;
            }
        }
    }
}


// ������Ȩ����֤KeyA
//00 00 FF 0F F1 D4 40 01 60 03 FF FF FF FF FF FF UID1 UID2 UID3 UID4 2A 00
//00 00 FF 0F F1 D4 40 01 60 03 FF FF FF FF FF FF 94 8A 3B 0B 2A 00
void  nfc_PsdVerifyKeyA(void)
{
    u8 data[22];
    u8 temp = 0;
    u8 i;
    u8 CheckCode = 0; //����У����
    flag_rev_finish = 0;
    data[0] = 0x00;
    data[1] = 0x00;
    data[2] = 0xFF;

    data[3] = 0x0F; //�� ����
    data[4] = 0xF1; //�� ���� У��  0x100-data[3]

    data[5] = 0xD4; //�����ʶ��
    data[6] = 0x40; //�����ʶ��

    data[7] = 0x01;
    data[8] = 0x60;
    data[9] = 0x03;

    data[10] = 0xFF; //KEY A ���� FF FF FF FF FF FF
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

    data[20] = 0x100 - temp; //���� У��   0x100-

    data[21] = 0x00;

    SendData(data, 22); //��USART1������ length���ȵ�����data



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
            CleanBuffer(40);//��� ���ڽ��ջ�����ǰ40 ���ֽ�����

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



//Ĭ�� �� 02����16���ֽ�
//00 00 FF 05 FB D4 40 01 30 02 B9 00
void  nfc_read()
{
    u8 data[12];
    u8 temp = 0;
    u8 i;
    u8 CheckCode = 0; //����У����

    flag_rev_finish = 0;
    data[0] = 0x00;
    data[1] = 0x00;
    data[2] = 0xFF;

    data[3] = 0x05; //�� ����
    data[4] = 0xFB; //�� ���� У��  0x100-data[3]

    data[5] = 0xD4; //�����ʶ��
    data[6] = 0x40; //�����ʶ��

    data[7] = 0x01;
    data[8] = 0x30;
    data[9] = 0x02; //���ڶ����16�ֽ�����

    data[10] = 0xB9; //����У��
    data[11] = 0x00;

    SendData(data, 12); //��USART1������ length���ȵ�����data

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
            GPIO_ResetBits(GPIOD, GPIO_Pin_3); //������ ��
            Delay_ms(150);
            GPIO_SetBits(GPIOD, GPIO_Pin_3);//������ ����
        }
        UID_backup[0] = UID[0];
        UID_backup[1] = UID[1];
        UID_backup[2] = UID[2];
        UID_backup[3] = UID[3];

        CleanBuffer(40);//��� ���ڽ��ջ�����ǰ40 ���ֽ�����
    }
}


//Ĭ���� 02��д 16���ֽڵĵ�һ���ֽ�
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

    data[3] = 0x15; //�� ����
    data[4] = 0xEB; //�� ���� У��  0x100-data[3]

    data[5] = 0xD4; //�����ʶ��
    data[6] = 0x40; //�����ʶ��

    data[7] = 0x01; //��д����6�ֽ� ����1�����ֲ�
    data[8] = 0xA0; //д
    data[9] = 0x02; //д�ڶ����16�ֽ�����

    data[10] = LedData; //�� 1 �ֽ� ����
    data[11] = 0x00;
    data[12] = 0x00; //�� 3 �ֽ� ����
    data[13] = 0x00;
    data[14] = 0x00; //�� 5 �ֽ� ����
    data[15] = 0x00;
    data[16] = 0x00; //�� 7 �ֽ� ����
    data[17] = 0x00;
    data[18] = 0x00; //�� 9 �ֽ� ����
    data[19] = 0x00;
    data[20] = 0x00; //�� 11 �ֽ� ����
    data[21] = 0x00;
    data[22] = 0x00; //�� 13 �ֽ� ����
    data[23] = 0x00;
    data[24] = 0x00; //�� 15 �ֽ� ����
    data[25] = 0x00;

    for (i = 5; i < 26; i++)
    {
        temp += data[i];
    }
    data[26] = 0x100 - temp; //���� У����
    data[27] = 0x00;

    SendData(data, 28); //��USART1������ length���ȵ�����data


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
        CleanBuffer(40);//��� ���ڽ��ջ�����ǰ40 ���ֽ�����

        if ((UID_backup[0] != UID[0]) | (UID_backup[1] != UID[1]) | (UID_backup[2] != UID[2]) | (UID_backup[3] != UID[3]))
        {
            GPIO_ResetBits(GPIOD, GPIO_Pin_3); //������ ��
            Delay_ms(10);
            GPIO_SetBits(GPIOD, GPIO_Pin_3);//������ ����

        }
        UID_backup[0] = UID[0];
        UID_backup[1] = UID[1];
        UID_backup[2] = UID[2];
        UID_backup[3] = UID[3];
    }
}

//��USART1������ length���ȵ�����data
void SendData(u8 *data, u8 length)
{
    u8 i;
    for (i = 0; i < length; i++)
    {
        USART_SendData(USART1, data[i]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) //�ȵ÷������
        {
        }
    }
}

void CleanBuffer(u16 num)//���� ǰ ���ٸ��ֽڵ�����
{
    u16 i = 0;

    for (i = 0; i < num; i++)
        RxBuffer1[i] = 0x00;
}




