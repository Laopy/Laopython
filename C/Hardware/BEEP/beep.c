#include "stm32f10x.h"
#include "beep.h"
/*ע�͵�����Ĵ���
void BEEP_Init(void)
{
  GPIO_InitTypeDef beep;
  //2.ʹ�� BEEP_PORT ��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
  //3.��ʼ��beep��BEEP_PIN��Ϊ�������ģʽ 
  beep.GPIO_Pin=BEEP_PIN;  //Beepָ������
  beep.GPIO_Speed=GPIO_Speed_10MHz;  //ָ���ٶ�Ϊ10MHz 
  beep.GPIO_Mode=GPIO_Mode_Out_PP;  //BeepΪ�������ģʽ
  GPIO_Init(BEEP_PORT,&beep);
  GPIO_SetBits(BEEP_PORT,BEEP_PIN);  //���1������������
}
*/
//************************************************************************************************//
void Buzzer_Init(void)//������ʹ��
{
	GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitTypeDef beep;
//2.ʹ��PC��ʱ��
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
//3.��ʼ��beep��PC13��Ϊ�������ģʽ     ��������������Դ��
	beep.GPIO_Pin=BEEP_PIN;//ѡ��˿ں�
	beep.GPIO_Speed=GPIO_Speed_50MHz;//����IO���ٶ�
	beep.GPIO_Mode=GPIO_Mode_Out_PP;//ѡ��IO�ӿڹ�����ʽΪ�������ģʽ
	GPIO_Init(BEEP_PORT,&beep);
	GPIO_SetBits(BEEP_PORT,BEEP_PIN);		
}

//************************************************************************************************//

	void Delay(unsigned int t)
{
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	//��ʱ��TIM3��ʼ��
 TIM_TimeBaseInitTypeDef time3; 
 time3.TIM_Period=t-1; //����һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ 
 time3.TIM_Prescaler=71;  //����������ΪTIME3ʱ��Ƶ�ʳ�����Ԥ��ƵֵPSC
 time3.TIM_CounterMode=TIM_CounterMode_Up;  //TIM3����Ϊ���ϼ���ģʽ
 TIM_TimeBaseInit(TIM3,&time3);  //����ָ���Ĳ�����ʼ�� 
 TIM_SetCounter(TIM3,0);  //TIME3��0
 TIM_ClearFlag(TIM3,TIM_FLAG_Update);
 TIM_Cmd(TIM3,ENABLE);	//ʹ�ܶ�ʱ��3  //10KHz����Ƶ�ʣ���ʱ1s==1000 000us	
	while(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update)==RESET);//�ȴ�TIM3��־λ���
 TIM_ClearFlag(TIM3, TIM_FLAG_Update);//���TIM3�����־λ
 TIM_Cmd(TIM3,DISABLE);
}	

//************************************************************************************************//
void Play(unsigned int t,unsigned int beatime)
{
	//��ʱ��TIM2��ʼ��
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		
 TIM_TimeBaseInitTypeDef time2;
 time2.TIM_Period=beatime-1; //����һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
 time2.TIM_Prescaler=7199; //����������ΪTIME2ʱ��Ƶ�ʳ�����Ԥ��ƵֵPSC 
 time2.TIM_CounterMode=TIM_CounterMode_Up;  //TIM2����Ϊ���ϼ���ģʽ
 TIM_TimeBaseInit(TIM2,&time2); //����ָ���Ĳ�����ʼ�� 
 TIM_SetCounter(TIM2,0); //TIME2��0
 TIM_ClearFlag(TIM2,TIM_FLAG_Update);
 TIM_Cmd(TIM2,ENABLE);//ʹ�ܶ�ʱ��2  

		while(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update)==RESET)
		{
		GPIO_ResetBits(BEEP_PORT,BEEP_PIN);//0
			Delay(t);
		GPIO_SetBits(BEEP_PORT,BEEP_PIN);//1
			Delay(t);
		}
		TIM_Cmd(TIM2,DISABLE);//�رն�ʱ��2	
}

//************************************************************************************************//

void tip_warnings_init(void)//���ž�������
{ 
	Buzzer_Init();
	unsigned int i=0;
	unsigned int Tone[]={0,956,851,758,716,638,568,506,1908,1701,1515,1433,1276,1136,1012}; //�е�������������
	unsigned int Music[3]={2,9,255}; 
	unsigned int beatime[4]={500,500,500};
	for(i=0;i<3;i++)
				{
					Play(Tone[Music[i]],beatime[i]);
				}
			//	GPIO_ResetBits(GPIOC,GPIO_Pin_13);	
}
