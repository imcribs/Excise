#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void MyI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)BitValue);
	Delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)BitValue);
	Delay_us(10);
}

uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	//在读取数据的时候，是从机向主机发送数据，主机对SDA没有控制权了
	//即主机无法驱动SDA引脚的电平变化，为了读取数据变化，只能变成
	//输入模式，来读取引脚电平，从而接收从机发送的Bit
	Delay_us(10);
	return BitValue;
}
void MyI2C_Init()
{
	// 一 把SCL SDA设置为开漏输出模式

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
		//二 把SCL SDA 置为高电平
	GPIO_SetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11);
	
}
void MyI2C_Start(void)
{
	//开始条件是需要把SCL SDA全释放，就是输出1，然后SCL高电平，拉低SDA
	MyI2C_W_SDA(1);  //兼容重复起始条件 
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
	MyI2C_W_SDA(0);
}


void MyI2C_Stop(void)
{
	//终止条件是，在SCL高电平期间，拉高SDA
	//由于不确定SDA的状态，优先拉低SDA
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

