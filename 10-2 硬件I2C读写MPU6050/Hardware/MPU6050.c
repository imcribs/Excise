#include "stm32f10x.h"                  // Device header

#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS 0xD0
void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
}

void	MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
//	MyI2C_Start();
//	MyI2C_SendByte(Address_MPU6050);
//	MyI2C_ReceiveAck();
//	MyI2C_SendByte(RegAddress);
//	MyI2C_ReceiveAck();
//	MyI2C_SendByte(Data);
//	MyI2C_ReceiveAck();
//	MyI2C_Stop();
	I2C_GenerateSTART(I2C2, ENABLE);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2, RegAddress);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	I2C_SendData(I2C2, Data);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTOP(I2C2, ENABLE);
}

uint8_t MPU6050_Readreg(uint8_t RegAddress)
{
	uint8_t Data;
	
	I2C_GenerateSTART(I2C2, ENABLE);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2, RegAddress);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTART(I2C2, ENABLE);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	
	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);
	
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
	Data = I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	
	return Data;
//	MyI2C_Start();
//	MyI2C_SendByte(Address_MPU6050);
//	MyI2C_ReceiveAck();
//	MyI2C_SendByte(RegAddress);
//	MyI2C_ReceiveAck();
//	
//	MyI2C_Start();
//	MyI2C_SendByte(Address_MPU6050 | 0x01);
//	MyI2C_ReceiveAck();
//	Data = MyI2C_ReceiveByte();
//	//这里是主机接收一个字节，按照规则需要给从机发送一个应答
//	MyI2C_SendAck(1);
//	MyI2C_Stop();
//	
//	return Data;
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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Ack =I2C_Ack_Enable ;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	I2C_InitStructure.I2C_DutyCycle =I2C_DutyCycle_2 ;
	I2C_InitStructure.I2C_Mode =I2C_Mode_I2C ;
	I2C_InitStructure.I2C_OwnAddress1 = 0X00 ;
	I2C_Init(I2C2,&I2C_InitStructure);
	
	I2C_Cmd(I2C2,ENABLE);
	
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
