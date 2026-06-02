#ifndef __ENCODER_H
#define __ENCODER_H
#include "stm32f10x.h" 
void Encoder_Init(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
int16_t Encoder_Get(void);

#endif
