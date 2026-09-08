#ifndef __LED_H
#define __LED_H

typedef enum {  //枚举定义LED的名字
    LED1 = 0,
    LED2 = 1
} LED_Index;

typedef enum {          //枚举定义LED的状态
    LED_OFF = 0,
    LED_ON  = 1
} LED_State;
void LED_Init(void);
void LED_SET(LED_Index led,LED_State state);


#endif 
