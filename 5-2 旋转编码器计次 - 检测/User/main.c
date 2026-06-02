#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "encoder.h"
 
int16_t Num;
uint8_t encoderPower; // 编码器上电状态

int main(void)
{
    OLED_Init();
    Encoder_Init();
    
    // 初始化后立即检测上电状态
    encoderPower = Encoder_CheckPower();
    
    // OLED显示界面
    OLED_ShowString(1,1,"Num:");        // 第1行：计数数值
    OLED_ShowString(2,1,"Power:");      // 第2行：上电状态
    if(encoderPower == 1)
    {
        OLED_ShowString(2,7,"OK");      // 上电正常
    }
    else
    {
        OLED_ShowString(2,7,"ERR");     // 未上电/硬件异常
    }
	
    while(1)
    {
        Num += Encoder_Get();
        OLED_ShowSignedNum(1,5,Num,5);  // 显示计数
        
        // 实时检测上电状态（可选，动态更新）
        encoderPower = Encoder_CheckPower();
        if(encoderPower == 1)
        {
            OLED_ShowString(2,7,"OK ");
        }
        else
        {
            OLED_ShowString(2,7,"ERR");
        }
    }
}
