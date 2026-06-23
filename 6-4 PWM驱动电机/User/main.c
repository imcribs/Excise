#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"

uint16_t i;

int main(void)
{
	OLED_Init();
	OLED_ShowString(1,5,"Hello");
	PWM_Init();
	
	
	
	while(1)
    {
       PWM_SetCompare2(1500);
    }
}
 