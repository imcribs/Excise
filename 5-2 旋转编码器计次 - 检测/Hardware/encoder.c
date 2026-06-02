#include "stm32f10x.h"   // Device header

#include "Delay.h"
int16_t encoder_count;

void Encoder_Init()
{
//配置时钟 开启gpio
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	

	//配制AFIO
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
	//配置EXTI
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line =  EXTI_Line0|EXTI_Line1;//EXTI的第十四个线路配置为中断模式
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;      // 开启中断
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    //  
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //下降沿触发
	EXTI_Init(&EXTI_InitStructure); 
	
	//配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI0_IRQHandler(void)
{
if(EXTI_GetITStatus(EXTI_Line0) == SET)
{
	Delay_ms(2);
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)  //当A相触发中断时 检测 B相的
	{
	encoder_count --;
	}
	EXTI_ClearITPendingBit(EXTI_Line0);//清除中断标志位
}
}

void EXTI1_IRQHandler(void)
{
if(EXTI_GetITStatus(EXTI_Line1) == SET)
{
	Delay_ms(2);
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0)
	{
	encoder_count++;
	}
	EXTI_ClearITPendingBit(EXTI_Line1);
}

}

int16_t Encoder_Get(void)
{
	int16_t Temp;
	Temp = encoder_count;
//	encoder_count = 0;
	return Temp;
	
}

