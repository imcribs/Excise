#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

 
int main(void)
{
	
	OLED_Init();
	
	Serial_Init();
	
//	Serial_SendByte(0x41);
//	Serial_SendByte('A');
	printf("Num = %d\r\n",666);
	while(1)
    {
        
          
    }
}
