#include "stm32f10x.h"                  // Device header

//配置定时中断
void PWM_Init(void)
{
	//使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///开启定时器
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //使能GPIO
	//
	// 配置GPIO口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU ;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	TIM_InternalClockConfig(TIM2);   //设置为内部时钟源
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimBaseInitSturcture;
	TIM_TimBaseInitSturcture.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimBaseInitSturcture.TIM_CounterMode= TIM_CounterMode_Up;
	TIM_TimBaseInitSturcture.TIM_Period= 10000 - 1;
	TIM_TimBaseInitSturcture.TIM_Prescaler=7200 - 1 ;
	TIM_TimBaseInitSturcture.TIM_RepetitionCounter= 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimBaseInitSturcture);
	
	//因为 stm32硬件会自动进行一次触发中断 所以需要加上清除中断标志位  然后进行正常的中断
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
}
