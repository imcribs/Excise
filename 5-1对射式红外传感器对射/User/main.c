#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "countersensor.h"
int main(void)
{
	OLED_Init();
	CounterSensor_Init();
	OLED_ShowString(1,1,"Count:");
	
	while(1)
    {
	OLED_ShowNum(1,7,countersensor_Get(),5);   
    }
}
