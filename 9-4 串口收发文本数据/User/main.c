#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "KEY.h"
#include "LED.h"
#include <string.h>

 
int main(void)
{
	
	OLED_Init();
	LED_Init();
	Serial_Init();

	OLED_ShowString(1,1,"TXPacket:");
	OLED_ShowString(3,1,"RXPacket:");
	
	while(1)
    {
			if(Serial_GetRXFlag() == 1)
			{
				if(strcmp(Serial_RxPacket,"LED_ON") == 0)
				{
				LED_SET(LED1,LED_ON);
					Serial_SendString("LED_ON_OK\r\n");
						OLED_ShowString(4,1,"                ");   //覆盖上一次串口传输的内容，防止乱码
			      OLED_ShowString(4,1,Serial_RxPacket);
				}
				else if(strcmp(Serial_RxPacket,"LED_OFF") == 0)
				{
					LED_SET(LED1,LED_OFF);
					Serial_SendString("LED_OFF_OK\r\n");
						OLED_ShowString(2,1,"                ");   
			      OLED_ShowString(2,1,Serial_RxPacket);
				}
				else
				{
				Serial_SendString("ERROR_COMMAND\r\n");
					OLED_ShowString(2,1,"                ");  
			    OLED_ShowString(2,1,Serial_RxPacket);
				}
			} 
    }
}
