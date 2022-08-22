
#include <Solid.h>
	
//*************ģ��ת������******************************************//
void InitADC1_15(void)
{
	//
	RCC_ADCCLKConfig( RCC_PCLK2_Div6);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );
	ADC_InitTypeDef ADC_InitStruct;//����ָ�룬�ô���ʱ��û��⣬
	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;//����ģʽ�����ã�1.����ģʽ��2.����ͬ����3.�����滻���� ����ģʽ
  ADC_InitStruct.ADC_ScanConvMode=DISABLE;//�Ƿ���Ҫ���ж�ͨ����ɨ��ʽת������ʹ��
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;//�Ƿ�����ת��ģʽ����ʹ��
  ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//ģ��ת���Ƿ���Ҫ�ⲿ�������ã�����Ҫ
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right ;//ADC���ݶ���ģʽ����
	ADC_InitStruct.ADC_NbrOfChannel=1;//ת��ͨ��������1
	
	ADC_Init(ADC1,& ADC_InitStruct);//��ʼ��ADC1�Լ�������ָ��
	ADC_Cmd(ADC1, ENABLE);//ʹ��ADC

	ADC_StartCalibration( ADC1);//��ʼУ׼ADC1
	while(ADC_GetCalibrationStatus( ADC1)==RESET)//���У׼״̬:����ɸ�λʱ��ѭ���ṹ��
		{
			
		}

}
//************��ʱ������*******************************************//

void Dta_time(unsigned int beatime)
{
		//��ʱ��TIM4��ʼ��
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		
			TIM_TimeBaseInitTypeDef time4;
//	NVIC_InitTypeDef NVIC4;
			time4.TIM_Period=beatime-1; //����һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
			time4.TIM_Prescaler=7199;//����������ΪTIM4ʱ��Ƶ�ʳ�����Ԥ��ƵֵPSC 
			time4.TIM_CounterMode=TIM_CounterMode_Up;  //TIM4����Ϊ���ϼ���ģʽ
			TIM_TimeBaseInit(TIM4,&time4);  //��ʼ��TIM4 	
	
			TIM_SetCounter(TIM4,0); //TIME2��0
			TIM_ClearFlag(TIM4,TIM_FLAG_Update);
			TIM_Cmd(TIM4,ENABLE);//ʹ�ܶ�ʱ��4  

		while(TIM_GetFlagStatus(TIM4,TIM_FLAG_Update)==RESET)
					{

					}
 		
}


//***********�����ȡ����**********************************************//

void Solid(void)
{
	
  
}
