#include "stm32f10x.h"
#include <OLED.h>
#include <math.h>
#include <stdio.h>
#include <beep.h>
#include "bmp.h"
#include <MLX90614.h>
#include <fire.h>
#include "Mator.h"
#include "USART.h"
#include "Delay_ms.h"
#include <Solid.h>
//**********************************************************
int main()
{
  float volt=0;//���ѹ���м����
	unsigned	int s;//���ε�ѹ��ֵ
	unsigned short value;//����value
	double sum=0;//��ѹ��ƽ�����ֵ
  double Dvlot=0;//һ������ǰ�ĵ�ѹֵ
	double Dta=0;	//��ѹ���ֵ
	float temp;//����һ������������������������¶�
	double ppm=0;
	int i;	//��ѹ��ƽ��ֵ���м����	
//********��������**********************************
	OLED_Init(); //��ʼ��OLED
	InitADC1_15();//ADC��ʼ��	
	SMBus_Init();//MLX90614��ʼ��
//**************************************************
	Usart1_Init(115200);//���ò����ʣ������ڵ���������
//**************************************************		
		while(1)
		{
//***********MQ2��ѹֵ��ȡ***************************************
			//ģ��ת��
				ADC_RegularChannelConfig(ADC1,ADC_Channel_15,1,ADC_SampleTime_28Cycles5);//���ñ��ɼ���ͨ��
				ADC_SoftwareStartConvCmd(ADC1,ENABLE);//�������ת��
			while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET)//��ȡת��������־��״̬
						{
						}
			value=ADC_GetConversionValue(ADC1);
			//������ε�ѹֵ
      volt=(float)value*(3.3/4096);
			s=volt*1000;	
 /*    warning������Dvlotһ��Ҫ��sumǰ�涨�壬��Ȼ���bug   */						
			 Dvlot=sum; //����һ������ǰ�ĵ�ѹֵ	
						
		 //����ѹֵ��ƽ�� ��ֹ����С�������λ����	
			for(i=0;i<25;i++)//ѭ��25�Σ���25�ε������ۼ�
				 {
					 sum=sum+s;
				 }
			sum=sum/25;//��25���ڵ�����ƽ��ֵ
			sum=sum/1000;//��mVת����V
			ppm = pow((11.5428 * 35.904 * sum)/(16.83-5.1* sum),0.6549);//���㻷��ppֵ
//*******************��ʾPPMֵ**************************//	
		printf("<Temp:%.2f>,<PPM:%.2f>\r\n",temp,ppm);//������������¶�	
		//	printf("PPMֵΪ:%.2f\r\n",ppm);//�����������PPM
		//	printf("%.2f\r\n",ppm);//�����������PPM 
			char strBuffer[10] = "";//����һ�����飬���������ַ���	
		  sprintf(strBuffer,"%.2f",ppm);		
		  OLED_ShowString(0,0,"ppm:");
			OLED_ShowString(0,2,(unsigned char *)strBuffer);	//������ת�ַ�����ʾ
			OLED_ShowString(40,2,"%");
//*******************��ȡʵʱ��ѹ����һ������ǰ����ֵDvlot**************************//
			Dta_time(10000);//��ʱ���趨һ������ʱ��				 
//*******************������ǰ�ĵ�ѹֵ��ʵʱ��ѹֵ���д�С�Ա��ж�**************************//
			Dta=sum-(Dvlot+0.1);//����һ��double��������������ʵʱ��ѹ��Dta��ѹ�Ĳ			
			if(Dta*10>0 ||sum*10>=10)
					{ 
						
					  int k;
						for(k=0;k<20;k++)
						 {
						  printf("<smoke:alarm>\r\n");//������������� 
				      tip_warnings_init();//����������
							OLED_ShowPicture(0,4,118,28,Solid_warning);//���𾯸�ͼ��					
			//				Set_Mator();//�������
						
						 }
						
					}
			else
					{					 
					  OLED_ShowPicture(0,4,118,28,None);//��ͼƬ	
						Reset_Mator();//�ر����
					}
					 
//*******************��ʾ�¶�**************************//					 
			char str[10];
			temp=SMBus_ReadTemp();			
			sprintf(str,"%.2f",temp);
			OLED_ShowPicture(60,0,33,16,temper);//	�¶�
			OLED_ShowString(60,2,(unsigned char *)str);	//������ת�ַ�����ʾ
			OLED_ShowPicture(100,2,16,16,du);	//��ʾ���϶ȵ�λ	 
//************�жϲ⵽���¶��Ƿ�ﵽ��ֵ**************
			if(temp>=60)//�������⵽�������¶ȴ���100�棬���������¾���
				{
					printf("<temp:alarm>\r\n");//�¶Ⱦ����������
					int k;
		//			for(k=0;k<100;k++)//�˷��������ô�������⵽һ��>100��ͻ����һ��ʱ�䴥������
							{
					      tip_warnings_init();//��������   //���1������������
					      OLED_ShowPicture(0,4,126,30,fire_warning);//���𾯸�ͼ��
								Set_Mator();//�������
								
							}
				
				}
			else
				{
					OLED_ShowPicture(0,4,126,30,None_Fire);//��ͼƬ	
					GPIO_ResetBits(BEEP_PORT,BEEP_PIN);   //���0���رշ�����	
					Reset_Mator();//�ر����
				}						
//*******************OLED�ָ���**************************//					 
			OLED_ShowString(50,0,"|"); 
		  OLED_ShowString(50,1,"|"); 
			OLED_ShowString(50,2,"|"); 
			OLED_ShowString(50,3,"|"); 
//*****************************************************//				
//**********************�������ʶ���뾯��****||star||**********************************

			if(GPIO_ReadInputDataBit(GPIO_FIRE_PORT0,GPIO_FIRE_Pin0)==1) //0�Ż��洫������⵽�ߵ�ƽ
				{
					if(GPIO_ReadInputDataBit(GPIO_FIRE_PORT0,GPIO_FIRE_MIDDLE0)==1)//0�Ż��м�������
						{
							printf("<fire:F>\r\n");//���澯���������
							int k;
					//		for(k=0;k<100;k++)//�˷��������ô�������⵽һ�λ���ͻ����һ��ʱ�䴥������
								{
									tip_warnings_init();//��������   //���1������������
									OLED_ShowPicture(0,4,126,30,fire_warning);//���𾯸�ͼ��
									Set_Mator();//�������
									
								}										
						}
						else
						{
							printf("<fire:R>\r\n");//���澯���������
							int k;
					//		for(k=0;k<100;k++)//�˷��������ô�������⵽һ�λ���ͻ����һ��ʱ�䴥������
								{
									tip_warnings_init();//��������   //���1������������
									OLED_ShowPicture(0,4,126,30,fire_warning);//���𾯸�ͼ��
									Set_Mator();//�������
									
								}																						
						}
					
				}
				
			else
				{
					OLED_ShowPicture(0,4,126,30,None_Fire);//��ͼƬ	
					GPIO_ResetBits(BEEP_PORT,BEEP_PIN);   //���0���رշ�����
					Reset_Mator();//�ر����
				}						
			if(GPIO_ReadInputDataBit(GPIO_FIRE_PORT1,GPIO_FIRE_Pin1)==1) //1�Ż��洫������⵽�ߵ�ƽ
				{
					if(GPIO_ReadInputDataBit(GPIO_FIRE_PORT1,GPIO_FIRE_MIDDLE1)==1)//0�Ż��м�������
						{
							printf("<fire:F>\r\n");//���澯���������
							int k;
						//	for(k=0;k<100;k++)//�˷��������ô�������⵽һ�λ���ͻ����һ��ʱ�䴥������
								{
									tip_warnings_init();//��������   //���1������������
									OLED_ShowPicture(0,4,126,30,fire_warning);//���𾯸�ͼ��
									Set_Mator();//�������
									
								}										
						}
						else
						{
							printf("<fire:L>\r\n");//���澯���������
							int k;
					//		for(k=0;k<100;k++)//�˷��������ô�������⵽һ�λ���ͻ����һ��ʱ�䴥������
								{
									tip_warnings_init();//��������   //���1������������
									OLED_ShowPicture(0,4,126,30,fire_warning);//���𾯸�ͼ��
									Set_Mator();//�������
									
								}																						
						}
				}
			else
				{
				  OLED_ShowPicture(0,4,126,30,None_Fire);//��ͼƬ	
					GPIO_ResetBits(BEEP_PORT,BEEP_PIN);   //���0���رշ�����
					Reset_Mator();//�ر����
				}
	

//******************�������ʶ���뾯��*****||end||**************************
		}//while��1���Ľ�������


}//main�Ľ�������


