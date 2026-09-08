#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MYRTC.h"

int main(void)
{
	
	OLED_Init();
	My_RTCInit();
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);  //防止依赖性
	
	OLED_ShowString(1,1,"CNT :");
	OLED_ShowString(2,1,"ALR :");
	OLED_ShowString(3,1,"ALRF:");

	PWR_WakeUpPinCmd(ENABLE);
	
	uint32_t Alarm;
	Alarm = RTC_GetCounter() + 10 ;
	OLED_ShowNum(2,6,Alarm,10);
	
	while (1)
	{
		OLED_ShowNum(1, 6, RTC_GetCounter(), 10);
		OLED_ShowNum(3,6,RTC_GetFlagStatus(RTC_FLAG_ALR),1);
		
		OLED_ShowString(4,1,"Running");
		Delay_ms(100);
		OLED_ShowString(4,1,"       ");
		Delay_ms(100);
		
		OLED_Clear();
		
		PWR_EnterSTANDBYMode();
		
	}
}



//#include "stm32f10x.h"                  // Device header
//#include "Delay.h"
//#include "OLED.h"
//#include "MYRTC.h"

//int main(void)
//{
//	OLED_Init();
//	My_RTCInit();
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
//	
//	// 判断是不是从Standby唤醒（WKUP标志）
//	if(PWR_GetFlagStatus(PWR_FLAG_WU) == SET)
//	{
//		// Standby唤醒进来，清除唤醒标志
//		PWR_ClearFlag(PWR_FLAG_WU);
//		OLED_ShowString(1,1,"Wake from STANDBY");
//		Delay_ms(1000);	// 屏幕点亮1s，作为提示，之后进入待机
//	}
//	
//	OLED_Clear();
//	OLED_ShowString(1,1,"CNT :");
//	OLED_ShowString(2,1,"ALR :");
//	
//	uint32_t Alarm = RTC_GetCounter() + 10 ;
//	RTC_SetAlarm(Alarm);	// 设置RTC闹钟
//	OLED_ShowNum(2,6,Alarm,10);
//	
//	// 开启RTC闹钟作为待机唤醒源
//	PWR_WakeUpPinCmd(DISABLE);	// 不用WKUP引脚，关掉
//	
//	
//	OLED_ShowString(4,1,"Enter Standby...");
//	Delay_ms(500);
//	OLED_Clear();
//	
//	PWR_EnterSTANDBYMode();
//	
//	// 到这里不会执行，待机唤醒会复位
//	while(1)
//	{
//		
//	}
//}
