#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "Motor.h"
#include "KEY.h"
uint8_t keyNum;
int8_t Speed;

int main(void)
{
	OLED_Init();
	Motor_Init();
	KEY_Init();
	OLED_ShowString(1,3,"Speed:");
	

	
	while(1)
    {
       keyNum = Key_GetNum();
			if(keyNum==1)
			{
			Speed += 20;
				if(Speed > 100)
				{
				Speed = -100;
				}
				Motor_SetSpeed(Speed);
				OLED_ShowSignedNum(1,7,Speed,3);
			}
		}
	}

