#ifndef __ENCODER_H
#define __ENCODER_H
#include "stm32f10x.h" 

void Encoder_Init(void);
int16_t Encoder_Get(void);
// 这里是强制声明，100%解决警告
uint8_t Encoder_CheckPower(void);

#endif