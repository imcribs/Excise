#include "stm32f10x.h"
#include "stdio.h"
#include "stdarg.h"
#include "Serial.h"      

// 定义 DMA 接收缓冲区及完成标志
uint8_t DMA_RX_Buffer[RX_BUFFER_SIZE];
uint8_t DMA_RX_Complete = 0;

// 原有串口发送相关函数（保持不变）
void Serial_Init(void)
{
    // 使能 USART1 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    // 使能 GPIOA 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // 使能 DMA1 时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // GPIO 配置（TX 推挽复用，RX 上拉输入）
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART 配置（9600, 8N1）
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStructure);

    // 使能 USART1 的 DMA 接收请求（重要）
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

    // 配置 DMA1 通道5（USART1_RX）
    DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(DMA1_Channel5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DMA_RX_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;           // 外设作为源
    DMA_InitStructure.DMA_BufferSize = RX_BUFFER_SIZE;           // 传输 4 字节
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;      // 内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                // 单次传输（非循环）
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);

    // 配置 DMA 中断（传输完成中断）
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    // 使能 DMA 传输完成中断
    DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);

    // 启动 DMA 接收
    DMA_Cmd(DMA1_Channel5, ENABLE);

    // 使能 USART1
    USART_Cmd(USART1, ENABLE);

    // 注意：不再使能 USART_IT_RXNE，因为改用 DMA
    // 原有 NVIC 中 USART1_IRQn 可保留或不用，这里省略
}

// 发送一个字节（保持不变）
void Serial_SendByte(uint8_t Byte)
{
    USART_SendData(USART1, Byte);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

// 发送数组（保持不变）
void Serial_SendArray(uint16_t *Array, uint16_t Length)
{
    uint16_t i;
    for(i = 0; i < Length; i++)
    {
        Serial_SendByte(Array[i]);
    }
}

// 发送字符串（保持不变）
void Serial_SendString(char *String)
{
    uint8_t i;
    for(i = 0; String[i] != '\0'; i++)
    {
        Serial_SendByte(String[i]);
    }
}

// 计算幂（保持不变）
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while(Y--)
    {
        Result *= X;
    }
    return Result;
}

// 发送数字（保持不变）
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for(i = 0; i < Length; i++)
    {
        Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
    }
}

// 重定向 printf（保持不变）
int fputc(int ch, FILE *f)
{
    Serial_SendByte(ch);
    return ch;
}

// 格式化输出（保持不变）
void Serial_printf(char *format,...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    Serial_SendString(String);
}

// 启动 DMA 接收（用于重启）
void Serial_DMA_Start(void)
{
    DMA_Cmd(DMA1_Channel5, DISABLE);                    // 先关闭
    DMA_SetCurrDataCounter(DMA1_Channel5, RX_BUFFER_SIZE); // 重设计数器
    DMA_Cmd(DMA1_Channel5, ENABLE);                    // 重新使能
}

// DMA1 通道5 中断服务函数
void DMA1_Channel5_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC5) != RESET)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC5);   // 清除传输完成标志
        DMA_RX_Complete = 1;                  // 置位完成标志
        // 注意：这里不自动重启，由主循环处理完毕后调用 Serial_DMA_Start() 重新启动
    }
}
