#include "stm32f10x.h"                  // Device header

void MySPI_W_SS(uint8_t BitValue)  //反转电平
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)BitValue);
}

void MySPI_W_SCK(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)BitValue);
}

void MySPI_W_MOSI(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)BitValue);
}

uint8_t MySPI_R_MISO(void)  //需要读取电平值，所以需要返回值
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
}

void MySPI_Init(void)  //使用的是SPI模式0
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	MySPI_W_SS(1);
	MySPI_W_SCK(0);
}

void MySPI_Start(void)
{
	MySPI_W_SS(0);
}

void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}

uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	
//	uint8_t ByteReceive = 0x00;
//	
//	MySPI_W_MOSI(ByteSend & 0x80);//把最高位放到MOSI上
//	MySPI_W_SCK(1);
//	if(MySPI_R_MISO()==1){ByteReceive |= 0x80;}
	//	//当读到MOSI上是高电平的时候，把ByteReceive的最高位置1.低电平不用动，还是保持为0
//	MySPI_W_SCK(0);
//	return ByteReceive;
	
	
//	//使用掩码，依次挑出每一位进行操作
	uint8_t i,ByteReceive = 0x00;
	for(i =0;i<8;i++)
	{
	MySPI_W_MOSI(ByteSend & (0x80>>i));
	MySPI_W_SCK(1);
	if(MySPI_R_MISO()==1){ByteReceive |= (0x80>>i);}
	MySPI_W_SCK(0);
	}
	
	return ByteReceive;
}
	//方法二 使用移位数据本身进行操作
//	uint8_t i;
//	for(i =0;i<8;i++)
//	{
//	MySPI_W_MOSI(ByteSend & 0x80);
//	ByteSend <<= 1; //移出最高位，然后低位自动补零
//	MySPI_W_SCK(1);
//	if(MySPI_R_MISO()==1){ByteSend |= 0x01;}
//	//如果从机输出到主机的为高电平即输出1，则是把低位变为1
//	//整个的输出逻辑是主机把高位移出去，移到从机的低位，从机则是把高位移动到主机的低位
//	//交换完的数据则进入for循环，直到完成依次字节交换
//	MySPI_W_SCK(0);
//	
//}
//	return ByteSend;
//	}
