

#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "bsp_SysTick.h"
#include "./ESP8266/bsp_esp8266.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_beep.h"
#include "nfc.h"

#define u8 unsigned char

extern u8 flag_nfc_status;

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
	
	u8 t = 0;

  SysTick_Init();
	// �����ж����ȼ�����
	NVIC_PriorityGroup_1;

	/*��ʼ��USART������ģʽΪ 115200 8-N-1*/
  Debug_USART_Config();
  LED_GPIO_Config();
	Key_GPIO_Config();

	printf("\r\n***Ұ��F429��ս��APP���ƿ�����ʵ��***\r\n");

	nfc_WakeUp();
	GPIO_WriteBit(GPIOB, GPIO_Pin_4, 1); 
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, 1); 

	while (1) {
		if ( Key_Scan(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON ) {
			LED1_TOGGLE;

			flag_nfc_status = 1; //KEY0  ,nfc 0x55
		}

		if ( Key_Scan(KEY2_GPIO_PORT, KEY2_PIN) == KEY_ON ) {
			LED2_TOGGLE;

			flag_nfc_status = 2; //KEY1 ,nfc 0xAA
		}

		if ( Key_Scan(KEY2_GPIO_PORT, KEY2_PIN) == KEY_ON ) {
			LED3_TOGGLE;

			flag_nfc_status = 0; //WKUP_PRES ,nfc
		}
		else {
			Delay_ms(10);
		}
		nfc_InListPassiveTarget(); //nfc(Mifare ),UID
		nfc_PsdVerifyKeyA();
	}


}

/*********************************************END OF FILE**********************/

