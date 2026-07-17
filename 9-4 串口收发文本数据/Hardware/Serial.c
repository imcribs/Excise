#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "stdarg.h"

uint8_t Serial_TxPacket[4];
uint8_t Serial_RxPacket[4];
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
	
	//å¼€å¯usartä¸­æ–­
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	//é…ç½®NVIC
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

void Serial_SendArray(uint8_t *Array,uint16_t Length)  //²»¿½±´Êı¾İ£¬¶øÖ»ÊÇ´«µİÒ»¸öµØÖ·£¬½ÚÊ¡ÄÚ´æ,·ÀÖ¹Õ»Òç³ö
{
	uint16_t i;
	for(i = 0;i < Length;i++)
	{
	Serial_SendByte(Array[i]);
	}
		
}

void Serial_SendString(char *String) //µ×²ã±¾ÖÊ£¬×Ö·û´®ÔÚcÓïÑÔÖĞ±¾Éí¾ÍÊÇÖ¸Õë
	                                   // Í¬Àí²»ÓÃ´«¶îÍâ³¤¶È£¬Ö»´«×Ö½ÚµØÖ·£¬½ÚÊ¡Õ»ÄÚ´æ
{
	uint8_t i;
	for(i = 0;String[i] != '\0';i++)  //×Ö·û´®ÒÔ\0Îª½áÎ²£¬Ö»ĞèÅĞ¶ÏÊÇ·ñÎª0£¬´Ó¶øÅĞ¶Ï×Ö·û´®µÄ´óĞ¡
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
uint8_t Serial_GetRXFlag(void)
{
	if(Serial_RXFlag == 1)
	{
	Serial_RXFlag = 0;
		return 1;
	}
	return 0;
}

void Serial_SendPacket(void)
{
	Serial_SendByte(0xFF);
	Serial_SendArray(Serial_TxPacket,4);
	Serial_SendByte(0xFE);
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
		if(RXData == 0xFF)
		{
		RXState = 1;
		cRXPacket = 0;
		}
	}
	else if(RXState == 1)
		{
			Serial_RxPacket[cRXPacket] = RXData ;
			cRXPacket ++;
			if(cRXPacket>=4)
			{
			RXState = 2;
				   //åœ¨ifè¯­å¥é‡Œé¢åˆ™æ˜¯åœ¨cRXPacketå¤§äºç­‰äº4çš„æ—¶å€™ï¼Œç»™cRXPacketæ¸…é›¶ï¼Œä½†åœ¨ä¹‹å‰å·²ç»æŠŠæ•°æ®å­˜å‚¨åˆ°äº†HEXæ•°æ®åŒ…ä¸­ï¼Œå‘é€åˆ°äº†ä¸²å£
			}
			
		}
	else if(RXState == 2)
			{
				if(RXData == 0xFE)
				{
					RXState = 0;
			Serial_RXFlag = 1;
				}
			}
		
	
		
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}
