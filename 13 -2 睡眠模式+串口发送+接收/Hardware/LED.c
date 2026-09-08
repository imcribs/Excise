#include "stm32f10x.h"                  // Device header
#include "LED.h"
void LED_Init(void) //初始化按键
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2);
}

//void LED1_ON(void)   //RED LED
//{
//	GPIO_SetBits(GPIOA,GPIO_Pin_1);
//}
//void LED1_OFF(void)
//{
//	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
//}

//void LED2_ON(void)  //BLUE LED
//{
//	GPIO_SetBits(GPIOA,GPIO_Pin_2);
//}
//void LED2_OFF(void)
//{
//	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
//}



 //定义LED SET函数
void LED_SET(LED_Index led,LED_State state)
{
	if(led==LED1)
	{
	if(state==LED_ON){GPIO_SetBits(GPIOA,GPIO_Pin_1);}
	else
	{GPIO_ResetBits(GPIOA,GPIO_Pin_1);}	
	}
	else
	{
		if(state==LED_ON){GPIO_SetBits(GPIOA,GPIO_Pin_2);}
	else
	{GPIO_ResetBits(GPIOA,GPIO_Pin_2);}	
	}
}