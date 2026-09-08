#include "stm32f10x.h"                  // Device header
#include <Time.h>
#include "MyRTC.h"

uint16_t MyRTC_Time[] = {2026,9,5,11,47,00};

void My_RTCInit(void)
{
	
	//1 开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);
	
	if(BKP_ReadBackupRegister(BKP_DR1) != 0x5AA5)
	{
		//2 配置RTC
	RCC_LSEConfig(RCC_LSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);
	
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	
	RTC_WaitForSynchro();     //等待同步
	RTC_WaitForLastTask();    //等待工作完成
	
	//3 配置预分频器
	RTC_SetPrescaler(32768 - 1);
	RTC_WaitForLastTask();   //写入操作最好是等待工作完成
	
		
		
		MyRTC_SetTime();
		
		
		
		BKP_WriteBackupRegister(BKP_DR1,0x5AA5);
		
		
	}
	else
	{
	RTC_WaitForSynchro(); 
	RTC_WaitForLastTask();
	
	}
	
	//4 设置计数器
//	RTC_SetCounter(1788577948);
//	RTC_WaitForLastTask();
	
	
	
}


void  MyRTC_SetTime(void)
{
		time_t time_cnt;
	struct tm time_date;
	
	time_date.tm_year = MyRTC_Time[0] - 1900;
	time_date.tm_mon = MyRTC_Time[1] - 1;
	time_date.tm_mday = MyRTC_Time[2];
	time_date.tm_hour = MyRTC_Time[3];
	time_date.tm_min = MyRTC_Time[4];
	time_date.tm_sec = MyRTC_Time[5];
	
	time_cnt = mktime(&time_date) - 8 * 60 * 60;
	
	RTC_SetCounter(time_cnt);
	RTC_WaitForLastTask();

}

void MyRTC_ReadTime(void)
{
	
	time_t time_cnt;		//定义秒计数器数据类型
	struct tm time_date;	//定义日期时间数据类型
	
	time_cnt = RTC_GetCounter() + 8 * 60 * 60;		//读取RTC的CNT，获取当前的秒计数器
													//+ 8 * 60 * 60为东八区的时区调整
	
	time_date = *localtime(&time_cnt);				//使用localtime函数，将秒计数器转换为日期时间格式
	
	MyRTC_Time[0] = time_date.tm_year + 1900;		//将日期时间结构体赋值给数组的时间
	MyRTC_Time[1] = time_date.tm_mon + 1;
	MyRTC_Time[2] = time_date.tm_mday;
	MyRTC_Time[3] = time_date.tm_hour;
	MyRTC_Time[4] = time_date.tm_min;
	MyRTC_Time[5] = time_date.tm_sec;
}
