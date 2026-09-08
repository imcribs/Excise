#include "stm32f10x.h"                  // Device header

uint16_t countersensor_count;
void CounterSensor_Init(void)
{   //配置时钟 开启gpio
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	

	//配制AFIO
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
	
	//配置EXTI
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line =  EXTI_Line14; //EXTI的第十四个线路配置为中断模式
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;      // 开启中断
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    //  
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //下降沿触发
	EXTI_Init(&EXTI_InitStructure); 
	
	//配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	 
}

uint16_t countersensor_Get(void)
{
	return countersensor_count;
}

void EXTI15_10_IRQHandler(void)  //对于中断首先进行中断标志位的判断
{
//对于中断首先进行中断标志位的判断
	//确保是自己想要的中断源触发的函数
	if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		countersensor_count ++;
	//使用完中断程序后 都需要清除中断标志位 否则会一直申请中断
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}
