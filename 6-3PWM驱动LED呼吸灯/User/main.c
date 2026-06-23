#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"

uint16_t i;

int main(void)
{
	OLED_Init();
	
	OLED_ShowString(1,3,"Hello World");
	
	PWM_Init();
	
	while(1)
    {
        for(i=0;i<=100;i++)
			{
			PWM_SetCompare1(i);    //依次把定时器的CCR寄存器设置为1-100，PWM占空比以此增大
				Delay_ms(10);
			}
        for(i=0;i<=100;i++)
			{
			PWM_SetCompare1(100-i);   //相反
				Delay_ms(10);
			}
    }
}
