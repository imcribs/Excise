#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void KEY_Init(void) //初始化按键
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_8|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
}
 
uint16_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==0)//输出的值就是寄存器某一位的值 0 或 1
		//按键是上拉输入 低电平才工作
	{
	Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==0);
		Delay_ms(20);
		KeyNum = 1;
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0)//输出的值就是寄存器某一位的值 0 或 1
		//按键是上拉输入 低电平才工作
	{
	Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0);
		Delay_ms(20);
		KeyNum = 2;
	}
	return KeyNum;
}
