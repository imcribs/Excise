#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "countersensor.h"
int main(void)
{
	OLED_Init();
	CounterSensor_Init();
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	
	
	OLED_ShowString(1,1,"Count:");
	
	while(1)
    {
	OLED_ShowNum(1,7,countersensor_Get(),5);   
			
			OLED_ShowString(2,1,"Runing");
			Delay_ms(100);
			OLED_ShowString(2,1,"      ");
			Delay_ms(100);
			
			PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);
			SystemInit();
    }
}
