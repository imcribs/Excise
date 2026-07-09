#include "stm32f10x.h"                  // Device header
#include "stdio.h"

void Serial_Init(void)
{
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	
	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//配置Usart
	USART_InitTypeDef  USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600 ;
	USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode= USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No ;
	USART_InitStructure.USART_StopBits= USART_StopBits_1;
	USART_InitStructure.USART_WordLength= USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	
	//shineng
	USART_Cmd(USART1,ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{

	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);

}

void Serial_SendArray(uint16_t *Array,uint16_t Length)  //不拷贝数据，而只是传递一个地址，节省内存,防止栈溢出
{
	uint16_t i;
	for(i = 0;i < Length;i++)
	{
	Serial_SendByte(Array[i]);
	}
		
}

void Serial_SendString(char *String) //底层本质，字符串在c语言中本身就是指针
	                                   // 同理不用传额外长度，只传字节地址，节省栈内存
{
	uint8_t i;
	for(i = 0;String[i] != '\0';i++)  //字符串以\0为结尾，只需判断是否为0，从而判断字符串的大小
	{
	Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X,uint32_t Y)
{
	uint32_t Result = 1;
	while(Y--)
	{
	Result *= X;
	}
	return Result;

}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for(i = 0; i < Length ; i++)
	{
	Serial_SendByte(Number / Serial_Pow(10,Length - i - 1) % 10 + '0');
	}
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

