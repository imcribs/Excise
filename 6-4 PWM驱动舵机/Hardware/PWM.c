#include "stm32f10x.h"                  // Device header

//配置定时中断
void PWM_Init(void)
{
	//使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///开启定时器
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //使能GPIO
	

	// 配置GPIO口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP ;// 使用复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	TIM_InternalClockConfig(TIM2);   //设置为内部时钟源
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimBaseInitSturcture;
	TIM_TimBaseInitSturcture.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimBaseInitSturcture.TIM_CounterMode= TIM_CounterMode_Up;
	TIM_TimBaseInitSturcture.TIM_Period= 20000 - 1;
	TIM_TimBaseInitSturcture.TIM_Prescaler=72 - 1 ;
	TIM_TimBaseInitSturcture.TIM_RepetitionCounter= 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimBaseInitSturcture);

	//配置CCR寄存器
	//结构体变量
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	TIM_OCStructInit(&TIM_OCInitStructure);  //用于初始化OCInit
	TIM_OCInitStructure.TIM_OCMode =TIM_OCMode_PWM1 ;    //输出比较模式 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //选择输出极性 ，选择为高，则是输出高电平有效。为低，则是，输出低电平为有效电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable ;  //输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;                            //初始化CCR的值
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);                    //把结构体变量交给TIM_OC1Tint,配置TIM2的输出比较通道1
	
		
	TIM_Cmd(TIM2,ENABLE);  //使能TIM2，定时器开始使用
}


void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM2,Compare);    //设置CCR1的值
}



