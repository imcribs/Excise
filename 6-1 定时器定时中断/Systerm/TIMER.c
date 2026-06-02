#include "stm32f10x.h"                  // Device header
#include "TIMER.h"
// extern uint16_t Num ;

//中断初始化
void Timer_Init(void)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);   //使能APB1总线上的外设时钟
	
	TIM_InternalClockConfig(TIM2);  //将时钟源设置为内部时钟//定时器上电后默认是使用内部时钟
	//配置时基单元   
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode= TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period= 10000 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler=7200 - 1 ;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter= 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	//使能更新中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	//
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	//开启NVIC
	//确定优先级分组
	//优先级分组确定了抢占优先级和子优先级各占几位
	//不需要重复设置 整个系统只需要设置一次
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//定义NVIC初始化结构体并配置
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 1;
	
	NVIC_Init(&NVIC_InitStructure);
	
	
	//启动定时器
	
	TIM_Cmd(TIM2,ENABLE);
}

//中断函数
//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)   //检测是否产生了中断 判断中断是否触发
//	{
//	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
//		Num ++ ;
//	}
//}

		
		
		
	
	
