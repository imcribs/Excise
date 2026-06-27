#include "stm32f10x.h"                  // Device header
 
void AD_Init()
{
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//配置ADCCLK
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //72MHz/6 = 12MHz
	
	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//选择规则组的输入通道
	ADC_RegularChannelConfig(ADC10,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	
	//结构体初始化ADC
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right ;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None ;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_InitStructure.ADC_ScanConvMode =  DISABLE;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	//开启ADC电源
	ADC_Cmd(ADC1,ENABLE);
	
	////对ADC进行校准
	
	//复位校准
	ADC_ResetCalibration(ADC1);
	//等待复位校准完成
	while( ADC_GetResetCalibrationStatus(ADC1) == SET)；//因为单片机执行完复位校准函数后，
	//会立即执行下一条代码，所以要加上while循环，等复位完成后再执行下面的函数
	//开始校准
	ADC_StartCalibration(ADC1);
	//等待校准完成
	while(ADC_GetCalibrationStatus(ADC1) == SET);
}
