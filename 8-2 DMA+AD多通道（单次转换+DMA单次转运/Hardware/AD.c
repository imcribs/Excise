#include "stm32f10x.h"                  // Device header


uint16_t AD_Value[4] ;

void AD_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);  
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
	
		
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //ADC外部触发转换源选择 //软件触发  //硬件触发
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;   //单次转换 or 连续转换
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;    //是否开启扫描模式
	ADC_InitStructure.ADC_NbrOfChannel = 4;      //ADC通道数量
	ADC_Init(ADC1, &ADC_InitStructure);
	
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;     
	DMA_InitStructure.DMA_PeripheralDataSize= DMA_PeripheralDataSize_HalfWord;  // 选择HalfWord是因为DR寄存器中，低16位是ADC！数据存储的地方，高16位是ADC2的存储位置
	DMA_InitStructure.DMA_PeripheralInc= DMA_PeripheralInc_Disable  ;   
	DMA_InitStructure.DMA_MemoryBaseAddr= (uint32_t)AD_Value;   
	DMA_InitStructure.DMA_MemoryDataSize= DMA_MemoryDataSize_HalfWord;   
	DMA_InitStructure.DMA_MemoryInc= DMA_MemoryInc_Enable;    //是否递增
	DMA_InitStructure.DMA_BufferSize= 4;      //DMA传输数据单元总数 //传输寄存器的大小  //理论最大是65535 ，但是内存达不到
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC  ;        //传输的方向   //
	DMA_InitStructure.DMA_M2M= DMA_M2M_Disable;     //软件触发
	DMA_InitStructure.DMA_Mode= DMA_Mode_Normal ;      //是否开启重装寄存器
	DMA_InitStructure.DMA_Priority=  DMA_Priority_Medium ;     //优先级
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	
	DMA_Cmd(DMA1_Channel1,DISABLE);   //开启DMA
	ADC_DMACmd(ADC1,ENABLE);          //开启ADC1的DMA
	ADC_Cmd(ADC1, ENABLE);             //开启ADC1
	
	
	ADC_ResetCalibration(ADC1);     //复位校准寄存器
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);   //等待复位校准标志位完成
	ADC_StartCalibration(ADC1);                //启动ADC自校准
	while (ADC_GetCalibrationStatus(ADC1) == SET);    //等待ADC自校准完成
}

void AD_GetValue(void)
{
	DMA_Cmd(DMA1_Channel1,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1,4);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //因为ADC是单次转换
	
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);	
}
