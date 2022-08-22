#include "Mator.h"


//********************�����������ʹ��****************************************
void MotorEn_Init(void)
{
	RCC_APB2PeriphClockCmd(GPIO_mator_CLK,ENABLE);  	//ʹ��C��GPIOʱ��

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;     		//ѡ���������
	GPIO_InitStruct.GPIO_Pin=GPIO_PIN_maotr1|GPIO_PIN_maotr2;					//ָ������PC0,MotEn2
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;			//�����������10MHz
	GPIO_Init(GPIO_PORT_mator,&GPIO_InitStruct);						//��ʼ������GPIOC�Ĵ���
	
	GPIO_ResetBits(GPIO_PORT_mator, GPIO_PIN_maotr1);//������0����ֹ�����ⷢ��
	GPIO_ResetBits(GPIO_PORT_mator, GPIO_PIN_maotr2);//������0����ֹ�����ⷢ��
}
//********************���������������***************************************
void Set_Mator(void)
{
		MotorEn_Init();  //�����������ʹ��				
	GPIO_SetBits(GPIO_PORT_mator,GPIO_PIN_maotr1);   //���1���������		
}
//********************�������رմ���***************************************
void Reset_Mator(void)
{
		MotorEn_Init();  //�����������ʹ��					
	//GPIO_ResetBits(GPIO_PORT_mator,GPIO_PIN_maotr1);   //���0���ر����		
}