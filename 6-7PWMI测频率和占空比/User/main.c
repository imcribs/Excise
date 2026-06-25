#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"

uint16_t i;

int main(void)
{
	OLED_Init();
	
	PWM_Init();
	
	IC_Init();
	
	OLED_ShowString(1,1,"Freq:00000Hz");
	OLED_ShowString(2,1,"Duty:00%");
	
	PWM_SetPscaler(720 - 1);     //Freq = 72M / (PSC +1) / 100
	PWM_SetCompare1(20);        //Duty =  CCR /100
//	PWM_SetPscaler(180 - 1)
	while(1)
    {
        OLED_ShowNum(1,6,IC_GetFreq(),5);
			OLED_ShowNum(2,6,IC_GetDuty(),2);
//			Delay_ms(5000);
    }
		
}
