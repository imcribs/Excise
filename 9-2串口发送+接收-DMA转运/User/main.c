#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

int main(void)
{
    OLED_Init();
    OLED_ShowString(1, 1, "RXData:");

    Serial_Init();    // 初始化串口及 DMA，自动启动第一次接收

    while(1)
    {
        if(DMA_RX_Complete)   // 接收到 4 个字节
        {
            DMA_RX_Complete = 0;   // 清除标志

            // 将接收缓冲区作为字符串显示（缓冲区内是 ASCII 字符，如 '4','1','4','1'）
            // 由于缓冲区长度固定为4，且末尾无'\0'，需要构造字符串
            char str[5];
            for(int i = 0; i < RX_BUFFER_SIZE; i++)
            {
                str[i] = DMA_RX_Buffer[i];
            }
            str[RX_BUFFER_SIZE] = '\0';   // 添加结束符

            OLED_ShowString(1, 8, str);   // 显示

            // 可选：回传接收到的数据（方便调试）
            Serial_SendString(str);
            Serial_SendByte('\n');        // 换行

            // 重新启动 DMA 接收，准备下一帧
            Serial_DMA_Start();
        }
        // 可加入其他任务
    }
}