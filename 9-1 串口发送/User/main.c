#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

 
int main(void)
{
	
	OLED_Init();
	
	Serial_Init();
	
//	Serial_SendByte(0x41); //
//	Serial_SendByte('A');
//	printf("Num = %d\r\n",666);  //重定向，每一个串口都需要定义
	//未封装
//	char String[100];
//	sprintf(String,"Num = %d\r\n",666);
//	Serial_SendString(String);
	Serial_printf("Num = %d\r\n",666);
	Serial_printf("你好，世界\r\n");
	while(1)
    {
        
          
    }
}
·