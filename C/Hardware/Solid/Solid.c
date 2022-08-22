
#include <Solid.h>
	
//*************模数转换代码******************************************//
void InitADC1_15(void)
{
	//
	RCC_ADCCLKConfig( RCC_PCLK2_Div6);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );
	ADC_InitTypeDef ADC_InitStruct;//定义指针，用处暂时还没理解，
	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;//工作模式（常用：1.独立模式，2.常规同步，3.快速替换）： 独立模式
  ADC_InitStruct.ADC_ScanConvMode=DISABLE;//是否需要进行多通道的扫描式转换：不使能
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;//是否连续转换模式：不使能
  ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//模数转化是否需要外部触发启用：不需要
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right ;//ADC数据对齐模式：右
	ADC_InitStruct.ADC_NbrOfChannel=1;//转化通道数量：1
	
	ADC_Init(ADC1,& ADC_InitStruct);//初始化ADC1以及定义其指针
	ADC_Cmd(ADC1, ENABLE);//使能ADC

	ADC_StartCalibration( ADC1);//开始校准ADC1
	while(ADC_GetCalibrationStatus( ADC1)==RESET)//获得校准状态:不完成复位时（循环结构）
		{
			
		}

}
//************定时器代码*******************************************//

void Dta_time(unsigned int beatime)
{
		//定时器TIM4初始化
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		
			TIM_TimeBaseInitTypeDef time4;
//	NVIC_InitTypeDef NVIC4;
			time4.TIM_Period=beatime-1; //在下一个更新事件装入活动的自动重装载寄存器周期的值
			time4.TIM_Prescaler=7199;//设置用来作为TIM4时钟频率除数的预分频值PSC 
			time4.TIM_CounterMode=TIM_CounterMode_Up;  //TIM4设置为向上计数模式
			TIM_TimeBaseInit(TIM4,&time4);  //初始化TIM4 	
	
			TIM_SetCounter(TIM4,0); //TIME2清0
			TIM_ClearFlag(TIM4,TIM_FLAG_Update);
			TIM_Cmd(TIM4,ENABLE);//使能定时器4  

		while(TIM_GetFlagStatus(TIM4,TIM_FLAG_Update)==RESET)
					{

					}
 		
}


//***********烟雾读取代码**********************************************//

void Solid(void)
{
	
  
}
