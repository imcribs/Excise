#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

 uint8_t RXData;
 
int main(void)
{
	
	OLED_Init();
	
	OLED_ShowString(1,1,"RXData:");
	
	Serial_Init();
	

	while(1)
    {
        if(Serial_GetRXFlag() == 1)
				{
				RXData = Serial_GetRXData();
					Serial_SendByte(RXData);  //回传功能，把数据传回电脑
					OLED_ShowHexNum(1,8,RXData,2);
				}
          
    }
}
