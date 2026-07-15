#ifndef __SERIAL_H
#define __SERIAL_H
#include "stdio.h"

#define RX_BUFFER_SIZE 4   // 接收固定长度 4 字节

extern uint8_t DMA_RX_Buffer[RX_BUFFER_SIZE];
extern uint8_t DMA_RX_Complete;  // 标志：1 表示一帧接收完成

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint16_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_printf(char *format,...);
// DMA 接收相关
void Serial_DMA_Start(void);    // 启动 DMA 接收（可外部调用，也可内部自动重启）

#endif
