#include "stm32f10x.h"                  // Device header

void Encoder_Init(void)
{
	//开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //开启TIM3的时钟
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启GOPIO的时钟
	
	//配置GPIO//GPIO初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //与外部模块输出的默认电平保持一致 //不确定时选择浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//时基单元配置
	TIM_TimeBaseInitTypeDef TIM_TimBaseInitStructure;
	TIM_TimBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
	TIM_TimBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up   ;
	TIM_TimBaseInitStructure.TIM_Period = 65535 - 1 ;  //
	TIM_TimBaseInitStructure.TIM_Prescaler=1 - 1;
	TIM_TimBaseInitStructure.TIM_RepetitionCounter= 0; 
	TIM_TimeBaseInit(TIM3,&TIM_TimBaseInitStructure);
	
	//输入捕获的配置
	TIM_ICInitTypeDef TIM_ICInitStructure ;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1 ;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising ;//高低电平极性不反转
	//用不到 
//	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1 ;
//	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI ;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	//配置编码器接口
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	
	//开启时钟
	TIM_Cmd(TIM3,ENABLE);
}
int16_t Encoder_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3,0);
	return Temp;
}
