#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"


void	MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(Address_MPU6050);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
	
}

uint8_t MPU6050_Readreg(uint8_t RegAddress)
{
	uint8_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(Address_MPU6050);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(Address_MPU6050 | 0x01);
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();
	//这里是主机接收一个字节，按照规则需要给从机发送一个应答
	MyI2C_SendAck(1);
	MyI2C_Stop();
	
	return Data;
}

// 发送数组的完整字节
//void MPU6050_WriteRegs(uint8_t RegAddress, uint8_t *Data, uint8_t Len)
//{
//    uint8_t i;
//    
//    MyI2C_Start();                          // 产生起始信号
//    MyI2C_SendByte(Address_MPU6050);        // 发送设备地址（写方向）
//    MyI2C_ReceiveAck();                     // 等待从机应答
//    MyI2C_SendByte(RegAddress);             // 发送寄存器起始地址
//    MyI2C_ReceiveAck();                     // 等待从机应答
//    
//    for (i = 0; i < Len; i++)               // 依次发送数组中的每个字节
//    {
//        MyI2C_SendByte(Data[i]);            // 发送数据字节
//        MyI2C_ReceiveAck();                 // 等待从机应答（可增加超时或错误处理）
//    }
//    
//    MyI2C_Stop();                           // 产生停止信号
//}


void MPU6050_Init(void)
{
	MyI2C_Init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
}

void MPU6050_GetData(int16_t *AccX,int16_t *AccY,int16_t *AccZ,
											int16_t *GYROX,int16_t *GYROY,int16_t *GYROZ)
{
	uint16_t DataH,DataL;
	
	DataH = MPU6050_Readreg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_Readreg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;
	
	DataH = MPU6050_Readreg(MPU6050_ACCEL_YOUT_H);
	DataL =MPU6050_Readreg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH =MPU6050_Readreg(MPU6050_ACCEL_ZOUT_H);
	DataL =MPU6050_Readreg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	DataH =MPU6050_Readreg(MPU6050_GYRO_XOUT_H);
	DataL =MPU6050_Readreg(MPU6050_GYRO_XOUT_L);
	*GYROX = (DataH << 8) | DataL;
	
	DataH =MPU6050_Readreg(MPU6050_GYRO_YOUT_H);
	DataL =MPU6050_Readreg(MPU6050_GYRO_YOUT_L);
	*GYROY = (DataH << 8) | DataL;
	
	DataH =MPU6050_Readreg(MPU6050_GYRO_ZOUT_H);
	DataL =MPU6050_Readreg(MPU6050_GYRO_ZOUT_L);
	*GYROZ = (DataH << 8) | DataL;
}
//使用I2C连续读取多字节

//void MPU6050_GetData(uint16_t *AccX, uint16_t *AccY, uint16_t *AccZ,
//                     uint16_t *GyroX, uint16_t *GyroY, uint16_t *GyroZ)
//{
//    uint8_t buf[6];    // 临时缓冲区，每次读6字节
//    
//    // 读取加速度计（起始地址 0x3B，连续6字节）
//    MPU6050_ReadRegs(MPU6050_ACCEL_XOUT_H, buf, 6);
//    *AccX = (buf[0] << 8) | buf[1];   // ACCEL_XOUT_H, L
//    *AccY = (buf[2] << 8) | buf[3];   // ACCEL_YOUT_H, L
//    *AccZ = (buf[4] << 8) | buf[5];   // ACCEL_ZOUT_H, L
//    
//    // 读取陀螺仪（起始地址 0x43，连续6字节）
//    MPU6050_ReadRegs(MPU6050_GYRO_XOUT_H, buf, 6);
//    *GyroX = (buf[0] << 8) | buf[1];  // GYRO_XOUT_H, L
//    *GyroY = (buf[2] << 8) | buf[3];  // GYRO_YOUT_H, L
//    *GyroZ = (buf[4] << 8) | buf[5];  // GYRO_ZOUT_H, L
//}
uint8_t MPU6050_GetID(void)
{
	MPU6050_Readreg(MPU6050_WHO_AM_I);
	return MPU6050_Readreg(MPU6050_WHO_AM_I);
}
