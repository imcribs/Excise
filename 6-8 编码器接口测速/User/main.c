#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "TIMER.h"
#include "Encoder.h"

int16_t Speed ;

int main(void)
{
	OLED_Init();
	Timer_Init();
	Encoder_Init();
	OLED_ShowString(1,1,"Speed:");
	
	
	while(1)
    {
        OLED_ShowSignedNum(1,7,Speed,5);

    }
	
}
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)   //检测是否产生了中断 判断中断是否触发
	{
		
		Speed =  Encoder_Get();
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	
	}
}
//中断时间T = (PSC+1)*(ARR+1)/72MHz

