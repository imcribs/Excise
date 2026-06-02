#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "TIMER.h"

uint16_t Num ;

int main(void)
{
	OLED_Init();
	Timer_Init();
	OLED_ShowString(1,1,"Num:");
	OLED_ShowString(2,1,"CNT:");
	
	while(1)
    {
        OLED_ShowNum(1,5,Num,6);
        OLED_ShowNum(2,5,TIM_GetCounter(TIM2),6);//TIM_GetCounter () 就是直接读硬件 CNT 寄存器的 “库函数包装”，所以用它就能在 OLED 上实时显示 CNT 当前值。
    }
	
}
uint16_t  TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)   //检测是否产生了中断 判断中断是否触发
	{
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		Num ++ ;
	}
	
}

