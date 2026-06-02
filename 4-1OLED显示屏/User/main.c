#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
 
int main(void)
{
	OLED_Init();
	OLED_ShowChar(1,1,'A');
	OLED_ShowString(1,3,"Hello World");
	OLED_ShowNum(2,1,12345,5);
	OLED_ShowSignedNum(2,7,-66,2);   //显示带符号的十进制数字
	OLED_ShowHexNum(3,1,0xAA55,4);     	//显示十六进制的数
	OLED_ShowBinNum(4,1,0xAA55,16);		//显示二进制的数
	
	OLED_Clear();
	
	while(1)
    {
        
        
    }
}
