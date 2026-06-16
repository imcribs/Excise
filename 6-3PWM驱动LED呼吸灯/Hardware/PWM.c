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
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP ;// 使用复用推挽输出
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
	
	//启动定时器
	TIM_Cmd(TIM2,ENABLE);
	//配置CCR寄存器
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	TIM_OCStructInit(&TIM_OCInitStructure);  //用于初始化OCInit
	TIM_OCInitStructure.TIM_OCMode =TIM_OCMode_PWM1 ;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable ;
	TIM_OCInitStructure.TIM_Pulse = ;     //CCR
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	
}
