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
  float volt=0;//测电压的中间变量
	unsigned	int s;//单次电压的值
	unsigned short value;//定义value
	double sum=0;//电压求平均后的值
  double Dvlot=0;//一个周期前的电压值
	double Dta=0;	//电压差的值
	float temp;//定义一个变量，用来储存读出来的温度
	double ppm=0;
	int i;	//电压求平均值的中间变量	
//********函数调用**********************************
	OLED_Init(); //初始化OLED
	InitADC1_15();//ADC初始化	
	SMBus_Init();//MLX90614初始化
//**************************************************
	Usart1_Init(115200);//设置波特率，给串口调试助手用
//**************************************************		
		while(1)
		{
//***********MQ2电压值读取***************************************
			//模数转换
				ADC_RegularChannelConfig(ADC1,ADC_Channel_15,1,ADC_SampleTime_28Cycles5);//设置被采集的通道
				ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件启动转换
			while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET)//获取转换结束标志的状态
						{
						}
			value=ADC_GetConversionValue(ADC1);
			//测出单次电压值
      volt=(float)value*(3.3/4096);
			s=volt*1000;	
 /*    warning！！：Dvlot一定要再sum前面定义，不然会出bug   */						
			 Dvlot=sum; //储存一个周期前的电压值	
						
		 //将电压值求平均 防止数据小数点后两位抖动	
			for(i=0;i<25;i++)//循环25次，将25次的数据累加
				 {
					 sum=sum+s;
				 }
			sum=sum/25;//求25次内的数的平均值
			sum=sum/1000;//将mV转换成V
			ppm = pow((11.5428 * 35.904 * sum)/(16.83-5.1* sum),0.6549);//计算环境pp值
//*******************显示PPM值**************************//	
		printf("<Temp:%.2f>,<PPM:%.2f>\r\n",temp,ppm);//串口助手输出温度	
		//	printf("PPM值为:%.2f\r\n",ppm);//串口助手输出PPM
		//	printf("%.2f\r\n",ppm);//串口助手输出PPM 
			char strBuffer[10] = "";//定义一个数组，用来储存字符串	
		  sprintf(strBuffer,"%.2f",ppm);		
		  OLED_ShowString(0,0,"ppm:");
			OLED_ShowString(0,2,(unsigned char *)strBuffer);	//浮点型转字符串显示
			OLED_ShowString(40,2,"%");
//*******************获取实时电压函数一个周期前的数值Dvlot**************************//
			Dta_time(10000);//定时器设定一个周期时长				 
//*******************将周期前的电压值与实时电压值进行大小对比判断**************************//
			Dta=sum-(Dvlot+0.1);//定义一个double变量，用来储存实时电压与Dta电压的差。			
			if(Dta*10>0 ||sum*10>=10)
					{ 
						
					  int k;
						for(k=0;k<20;k++)
						 {
						  printf("<smoke:alarm>\r\n");//烟雾警报串口输出 
				      tip_warnings_init();//蜂鸣器警报
							OLED_ShowPicture(0,4,118,28,Solid_warning);//暗火警告图案					
			//				Set_Mator();//驱动马达
						
						 }
						
					}
			else
					{					 
					  OLED_ShowPicture(0,4,118,28,None);//空图片	
						Reset_Mator();//关闭马达
					}
					 
//*******************显示温度**************************//					 
			char str[10];
			temp=SMBus_ReadTemp();			
			sprintf(str,"%.2f",temp);
			OLED_ShowPicture(60,0,33,16,temper);//	温度
			OLED_ShowString(60,2,(unsigned char *)str);	//浮点型转字符串显示
			OLED_ShowPicture(100,2,16,16,du);	//显示摄氏度单位	 
//************判断测到的温度是否达到阈值**************
			if(temp>=60)//如果被检测到的物体温度大于100℃，将进行以下警报
				{
					printf("<temp:alarm>\r\n");//温度警报串口输出
					int k;
		//			for(k=0;k<100;k++)//此方法可以让传感器检测到一次>100℃就会持续一段时间触发警报
							{
					      tip_warnings_init();//多音警报   //输出1，开启蜂鸣器
					      OLED_ShowPicture(0,4,126,30,fire_warning);//明火警告图案
								Set_Mator();//驱动马达
								
							}
				
				}
			else
				{
					OLED_ShowPicture(0,4,126,30,None_Fire);//空图片	
					GPIO_ResetBits(BEEP_PORT,BEEP_PIN);   //输出0，关闭蜂鸣器	
					Reset_Mator();//关闭马达
				}						
//*******************OLED分割线**************************//					 
			OLED_ShowString(50,0,"|"); 
		  OLED_ShowString(50,1,"|"); 
			OLED_ShowString(50,2,"|"); 
			OLED_ShowString(50,3,"|"); 
//*****************************************************//				
//**********************红外火焰识别与警报****||star||**********************************

			if(GPIO_ReadInputDataBit(GPIO_FIRE_PORT0,GPIO_FIRE_Pin0)==1) //0号火焰传感器检测到高电平
				{
					if(GPIO_ReadInputDataBit(GPIO_FIRE_PORT0,GPIO_FIRE_MIDDLE0)==1)//0号机中间两个灯
						{
							printf("<fire:F>\r\n");//火焰警报串口输出
							int k;
					//		for(k=0;k<100;k++)//此方法可以让传感器检测到一次火焰就会持续一段时间触发警报
								{
									tip_warnings_init();//多音警报   //输出1，开启蜂鸣器
									OLED_ShowPicture(0,4,126,30,fire_warning);//明火警告图案
									Set_Mator();//驱动马达
									
								}										
						}
						else
						{
							printf("<fire:R>\r\n");//火焰警报串口输出
							int k;
					//		for(k=0;k<100;k++)//此方法可以让传感器检测到一次火焰就会持续一段时间触发警报
								{
									tip_warnings_init();//多音警报   //输出1，开启蜂鸣器
									OLED_ShowPicture(0,4,126,30,fire_warning);//明火警告图案
									Set_Mator();//驱动马达
									
								}																						
						}
					
				}
				
			else
				{
					OLED_ShowPicture(0,4,126,30,None_Fire);//空图片	
					GPIO_ResetBits(BEEP_PORT,BEEP_PIN);   //输出0，关闭蜂鸣器
					Reset_Mator();//关闭马达
				}						
			if(GPIO_ReadInputDataBit(GPIO_FIRE_PORT1,GPIO_FIRE_Pin1)==1) //1号火焰传感器检测到高电平
				{
					if(GPIO_ReadInputDataBit(GPIO_FIRE_PORT1,GPIO_FIRE_MIDDLE1)==1)//0号机中间两个灯
						{
							printf("<fire:F>\r\n");//火焰警报串口输出
							int k;
						//	for(k=0;k<100;k++)//此方法可以让传感器检测到一次火焰就会持续一段时间触发警报
								{
									tip_warnings_init();//多音警报   //输出1，开启蜂鸣器
									OLED_ShowPicture(0,4,126,30,fire_warning);//明火警告图案
									Set_Mator();//驱动马达
									
								}										
						}
						else
						{
							printf("<fire:L>\r\n");//火焰警报串口输出
							int k;
					//		for(k=0;k<100;k++)//此方法可以让传感器检测到一次火焰就会持续一段时间触发警报
								{
									tip_warnings_init();//多音警报   //输出1，开启蜂鸣器
									OLED_ShowPicture(0,4,126,30,fire_warning);//明火警告图案
									Set_Mator();//驱动马达
									
								}																						
						}
				}
			else
				{
				  OLED_ShowPicture(0,4,126,30,None_Fire);//空图片	
					GPIO_ResetBits(BEEP_PORT,BEEP_PIN);   //输出0，关闭蜂鸣器
					Reset_Mator();//关闭马达
				}
	

//******************红外火焰识别与警报*****||end||**************************
		}//while（1）的结束括号


}//main的结束括号


