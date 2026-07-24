#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"


int16_t AX,AY,AZ,GX,GY,GZ;
uint8_t ID;

int main(void)
{
	
//	OLED_Init();
//	
//	MPU6050_Init();
//	
	// 使用寄存器直接操作
	//在使用IMPU写入之前，要结束睡眠状态
//	MPU6050_WriteReg(0x6B,0x00);
//	
//	MPU6050_WriteReg(0x19,0xAA);
//	
//	uint8_t ID =  MPU6050_Readreg(0x19);
//	
//	OLED_ShowHexNum(1,1,ID,2);  //16进制
	
	OLED_Init();
	MPU6050_Init();
	OLED_ShowString(1,1,"ID:");
	ID = MPU6050_GetID();
	OLED_ShowHexNum(1,4,ID,2);
	while (1)
	{
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		OLED_ShowSignedNum(2, 1, AX, 5);
		OLED_ShowSignedNum(3, 1, AY, 5);
		OLED_ShowSignedNum(4, 1, AZ, 5);
		OLED_ShowSignedNum(2, 8, GX, 5);
		OLED_ShowSignedNum(3, 8, GY, 5);
		OLED_ShowSignedNum(4, 8, GZ, 5);
	}
}
