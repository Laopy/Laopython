#include "fire.h"

//***************���洫����0�Ż�����ʹ��***********************
void FIRE_CONFIG0(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   
   RCC_APB2PeriphClockCmd(GPIO_FIRE_CLK0 , ENABLE);  //ʹ��ʱ��
   //ʹ�������źŽ����źŴ��ݣ���ʼ��Ϊ��������ģʽ
   GPIO_InitStructure.GPIO_Pin=GPIO_FIRE_Pin0;
   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; 
   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
   GPIO_Init(GPIO_FIRE_PORT0,&GPIO_InitStructure);
}
//***************���洫����1�Ż�����ʹ��***********************
void FIRE_CONFIG1(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   
   RCC_APB2PeriphClockCmd(GPIO_FIRE_CLK1 , ENABLE);  //ʹ��ʱ��
   //ʹ�������źŽ����źŴ��ݣ���ʼ��Ϊ��������ģʽ
   GPIO_InitStructure.GPIO_Pin=GPIO_FIRE_Pin1;
   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; 
   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
   GPIO_Init(GPIO_FIRE_PORT1,&GPIO_InitStructure);
}
//***************���洫������ȡ�뾯������***********************
void fire_cheak(void)
{
	
		
}


