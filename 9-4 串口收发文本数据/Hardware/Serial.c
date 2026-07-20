#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "stdarg.h"



char Serial_RxPacket[100];
uint8_t Serial_RXFlag;

void Serial_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	

	USART_InitTypeDef  USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600 ;
	USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode= USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity=USART_Parity_No ;
	USART_InitStructure.USART_StopBits= USART_StopBits_1;
	USART_InitStructure.USART_WordLength= USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	
	//寮�鍚痷sart涓柇
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	//閰嶇疆NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{

	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);

}

void Serial_SendArray(uint8_t *Array,uint16_t Length)  //不拷贝数据，而只是传递一个地址，节省内存,防止栈溢出
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

void Serial_printf(char *format,...)
{
	char String[100];
	va_list arg;
	va_start(arg,format);
	vsprintf(String,format,arg);
	va_end(arg);
	Serial_SendString(String);
}


void USART1_IRQHandler(void)
{
		static uint8_t  RXState= 0;
		static uint8_t  cRXPacket= 0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
	uint8_t RXData = USART_ReceiveData(USART1);
	if(RXState == 0)
	{
		if(RXData == '@'&& Serial_RXFlag == 0) //涓婁竴娆＄殑鎺ユ敹杩樻湭瀹屾垚涔嬪墠锛孯XState涓嶇疆1锛屽嵆涓嶈兘缁х画鎺ユ敹
		{
		RXState = 1;
		cRXPacket = 0;
		}
	}
	else if(RXState == 1)
		{
			if( RXData== '\r')
			{
			RXState = 2;
			}
			else
			{
			Serial_RxPacket[cRXPacket] = RXData ;
			cRXPacket ++;
			}
		}
	else if(RXState == 2)
			{
				if(RXData == '\n')
				{
					RXState = 0;
					Serial_RxPacket[cRXPacket] = '\0';
			Serial_RXFlag = 1; 
				}
			}
		
	
		
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}
