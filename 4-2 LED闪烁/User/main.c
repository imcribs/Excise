#include "stm32f10x.h"                  // Device header#incluy
#include "Delay.h"
int main(void)
{
	//外设时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//GPIO初始化
	GPIO_InitTypeDef GPIO_InitStructure;                       
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//开漏输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//写入输出值
	//GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
	//GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
	while(1)
	{
		//使用resetbits 和 setbits 来实现小灯闪烁
//		GPIO_ResetBits(GPIOA,GPIO_Pin_0);
//		Delay_ms(500);
//		GPIO_SetBits(GPIOA,GPIO_Pin_0);
//		Delay_ms(500);
		//使用writebit 来实现 小灯闪烁
//		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
//		Delay_ms(100);
//		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
//		Delay_ms(100);
		
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,(BitAction) 0);
		Delay_ms(500);
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,(BitAction) 1);
		Delay_ms(500);
	}
}
