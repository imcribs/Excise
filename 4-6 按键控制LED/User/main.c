#include "stm32f10x.h"                  // Device header#incluy
#include "Delay.h"
#include "LED.h"
#include "KEY.h"

uint8_t KeyNum;
 uint8_t led_state;
 
int main(void)
{
	KEY_Init();
	LED_Init();
//	
//	uint8_t led1_state = 0;     //直接说明哪个按键控制哪个LED
//    uint8_t led2_state = 0;

//
	while(1)
    {
        KeyNum = Key_GetNum();          // 阻塞至按键释放，返回键值 1或2，无按键返回0

        if(KeyNum == 1)                 // 按键1（PA8）控制 LED1（PA1）
        {
            led_state = GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1);  //读取PIN1输出情况
            if(led_state == 1)          // 当前亮，则熄灭     //按一下 灯关 再按一下 灯开 就是给led的端口取反
                LED_SET(LED1, LED_OFF);          //LED 是低电平点亮
            else                        // 当前灭，则点亮
                LED_SET(LED1, LED_ON);
        }

        if(KeyNum == 2)                 // 按键2（PA11）控制 LED2（PA2）
        {
            led_state = GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2);
            if(led_state == 1)
                LED_SET(LED2, LED_OFF);
            else
                LED_SET(LED2, LED_ON);
        }
    }
}
