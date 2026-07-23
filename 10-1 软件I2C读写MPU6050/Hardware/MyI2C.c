#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void MyI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)BitValue);
	Delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)BitValue);
	Delay_us(10);
}

uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	//在读取数据的时候，是从机向主机发送数据，主机对SDA没有控制权了
	//即主机无法驱动SDA引脚的电平变化，为了读取数据变化，只能变成
	//输入模式，来读取引脚电平，从而接收从机发送的Bit
	Delay_us(10);
	return BitValue;
}
void MyI2C_Init(void)
{
	// 一 把SCL SDA设置为开漏输出模式

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
		//二 把SCL SDA 置为高电平
	GPIO_SetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11);
	
}
void MyI2C_Start(void)
{
	//开始条件是需要把SCL SDA全释放，就是输出1，然后SCL高电平，拉低SDA
	MyI2C_W_SDA(1);  //兼容重复起始条件 
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);    //再SCL高电平时期，先拉低SDA-> 产生起始条件
	//起始条件是，在SCL高电平时间，拉低SDA
	MyI2C_W_SCL(0);  // 拉低SCL，准备传输数据
	
}


void MyI2C_Stop(void)
{
	//终止条件是，在SCL高电平期间，拉高SDA
	//由于不确定SDA的状态，优先拉低SDA
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}
void MyI2C_SendByte(uint8_t Byte)
{
		uint8_t i;
	for(i=0;i<8;i++)
	{
		MyI2C_W_SDA(Byte &(0x80 >> i)); 
		//使用按位与的方式来取出某一位
		//利用i的递增，由高到低依次取出数据
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}
uint8_t MyI2C_ReceiveByte(void)
{
	//前面的函数把SCL拉低了
	
	uint8_t i;
	uint8_t Byte = 0x00;
	//SDA(1),使MOS管断开，引脚高阻态，不再控制总线
	MyI2C_W_SDA(1);  // 主机释放总线
	for(i = 0;i<8;i++)
	{
		MyI2C_W_SCL(1);  //拉高SCL开始读取数据
		if(MyI2C_R_SDA()== 1){Byte |= (0x80 >> i) ;}
		//如果if不成立，意思就是发送的是0，成立就是发送的是1，
		//由于初始化Byte为0x00，不成立的话，就是0x00
		MyI2C_W_SCL(0);  // 该字节接收完毕，拉低SCL继续传输下一位
	}
	return Byte;  //Byte是局部变量，需要传到外部函数需要的地方
}

void MyI2C_SendAck(uint8_t AckBit)
{
		MyI2C_W_SDA(AckBit); 
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);

}
uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA(1); 
	MyI2C_W_SCL(1); 
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);  
	return AckBit;  
}
/**
 * @brief I2C总线扫描，查找在线从设备7位地址
 */
//void I2C_ScanBus(void)
//{
//	uint8_t addr;
//	uint8_t ackFlag;
//	printf("========== I2C Scan Start ==========\r\n");
//	
//	// 7位I2C地址：0x01 ~ 0x7F
//	for(addr = 1; addr <= 0x7F; addr++)
//	{
//		MyI2C_Start();
//		// 发送【7位地址 + 写位(0)】
//		MyI2C_SendByte(addr << 1 | 0x00);
//		ackFlag = MyI2C_ReceiveAck();
//		
//		MyI2C_Stop();
//		
//		if(ackFlag == 0)  // 收到ACK，设备在线
//		{
//			printf("Found Device! 7-bit Addr: 0x%02X\r\n", addr);
//		}
//	}
//	
//	printf("========== I2C Scan Finish ==========\r\n\r\n");
//}

