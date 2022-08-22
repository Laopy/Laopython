#include "stm32f10x.h"
#include "beep.h"
/*注释掉方莉的代码
void BEEP_Init(void)
{
  GPIO_InitTypeDef beep;
  //2.使能 BEEP_PORT 口时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
  //3.初始化beep（BEEP_PIN）为推挽输出模式 
  beep.GPIO_Pin=BEEP_PIN;  //Beep指定引脚
  beep.GPIO_Speed=GPIO_Speed_10MHz;  //指定速度为10MHz 
  beep.GPIO_Mode=GPIO_Mode_Out_PP;  //Beep为推挽输出模式
  GPIO_Init(BEEP_PORT,&beep);
  GPIO_SetBits(BEEP_PORT,BEEP_PIN);  //输出1，开启蜂鸣器
}
*/
//************************************************************************************************//
void Buzzer_Init(void)//蜂鸣器使能
{
	GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitTypeDef beep;
//2.使能PC口时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
//3.初始化beep（PC13）为推挽输出模式     蜂鸣器必须用无源的
	beep.GPIO_Pin=BEEP_PIN;//选择端口号
	beep.GPIO_Speed=GPIO_Speed_50MHz;//设置IO口速度
	beep.GPIO_Mode=GPIO_Mode_Out_PP;//选择IO接口工作方式为推挽输出模式
	GPIO_Init(BEEP_PORT,&beep);
	GPIO_SetBits(BEEP_PORT,BEEP_PIN);		
}

//************************************************************************************************//

	void Delay(unsigned int t)
{
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	//定时器TIM3初始化
 TIM_TimeBaseInitTypeDef time3; 
 time3.TIM_Period=t-1; //在下一个更新事件装入活动的自动重装载寄存器周期的值 
 time3.TIM_Prescaler=71;  //设置用来作为TIME3时钟频率除数的预分频值PSC
 time3.TIM_CounterMode=TIM_CounterMode_Up;  //TIM3设置为向上计数模式
 TIM_TimeBaseInit(TIM3,&time3);  //跟据指定的参数初始化 
 TIM_SetCounter(TIM3,0);  //TIME3清0
 TIM_ClearFlag(TIM3,TIM_FLAG_Update);
 TIM_Cmd(TIM3,ENABLE);	//使能定时器3  //10KHz计数频率，延时1s==1000 000us	
	while(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update)==RESET);//等待TIM3标志位溢出
 TIM_ClearFlag(TIM3, TIM_FLAG_Update);//清除TIM3溢出标志位
 TIM_Cmd(TIM3,DISABLE);
}	

//************************************************************************************************//
void Play(unsigned int t,unsigned int beatime)
{
	//定时器TIM2初始化
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		
 TIM_TimeBaseInitTypeDef time2;
 time2.TIM_Period=beatime-1; //在下一个更新事件装入活动的自动重装载寄存器周期的值
 time2.TIM_Prescaler=7199; //设置用来作为TIME2时钟频率除数的预分频值PSC 
 time2.TIM_CounterMode=TIM_CounterMode_Up;  //TIM2设置为向上计数模式
 TIM_TimeBaseInit(TIM2,&time2); //跟据指定的参数初始化 
 TIM_SetCounter(TIM2,0); //TIME2清0
 TIM_ClearFlag(TIM2,TIM_FLAG_Update);
 TIM_Cmd(TIM2,ENABLE);//使能定时器2  

		while(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update)==RESET)
		{
		GPIO_ResetBits(BEEP_PORT,BEEP_PIN);//0
			Delay(t);
		GPIO_SetBits(BEEP_PORT,BEEP_PIN);//1
			Delay(t);
		}
		TIM_Cmd(TIM2,DISABLE);//关闭定时器2	
}

//************************************************************************************************//

void tip_warnings_init(void)//播放警报音乐
{ 
	Buzzer_Init();
	unsigned int i=0;
	unsigned int Tone[]={0,956,851,758,716,638,568,506,1908,1701,1515,1433,1276,1136,1012}; //中低音半周期数组
	unsigned int Music[3]={2,9,255}; 
	unsigned int beatime[4]={500,500,500};
	for(i=0;i<3;i++)
				{
					Play(Tone[Music[i]],beatime[i]);
				}
			//	GPIO_ResetBits(GPIOC,GPIO_Pin_13);	
}
