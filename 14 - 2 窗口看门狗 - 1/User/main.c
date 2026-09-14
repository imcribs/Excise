#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "KEY.h"


int main(void)
{
	OLED_Init();
	Key_Init();
	
	OLED_ShowString(1,1,"WWDG RST");
	
	//读取 RCC 寄存器里的状态标志位，查询时钟、复位相关事件有没有发生
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST)== SET)
	{
		OLED_ShowString(2,1,"WWDGRST");
		Delay_ms(500);
		OLED_ShowString(2,1,"       ");
		Delay_ms(100);
	}
	else
	{
		OLED_ShowString(3,1,"RST");
		Delay_ms(500);
		OLED_ShowString(3,1,"   ");
		Delay_ms(100);
	}
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE); //窗口看门狗使用的是PCL 36MHZ	
	
	WWDG_SetPrescaler(WWDG_Prescaler_8);  //50ms
	WWDG_SetWindowValue(0x40 | 21);     //30ms
	WWDG_Enable(0x40 | 54);    //使能并实现第一次喂狗
	
	while (1)
{
	Key_GetNum();
	
	// 只有计数器递减到窗口值以内(计数器 <= 窗口值)才允许喂狗
	// 过早喂狗(计数器仍大于窗口值)会直接触发 WWDG 复位
	if ((WWDG->CR & 0x7F) <= (WWDG->CFR & 0x7F))
	{
		WWDG_SetCounter(0x40 | 54);    // 窗口内喂狗,重新装载计数器
		
		OLED_ShowString(4,1,"FEED");
		Delay_ms(20);
		OLED_ShowString(4,1,"    ");
		Delay_ms(20);
	}
}

}

	
