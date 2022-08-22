/*ע�͵�����Ĵ���
#ifndef _FIRE_H
#define _FIRE_H
#include "stm32f10x.h"

//�궨��
#define GPIO_FIRE_CLK  RCC_APB2Periph_GPIOC  //ʱ��
#define GPIO_FIRE_PORT GPIOC                 //���洫�����뿪�������ӵ����ŵļĴ���
#define GPIO_FIRE_Pin  GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10          //���洫�����뿪�������ӵ����� 

void FIRE_CONFIG(void);

#endif

*/
#ifndef _FIRE_H_
#define _FIRE_H_
//**************************************************************************************//
#ifndef FIRE_GLOBALS
    #define FIRE_EXT extern
#else
    #define FIRE_EXT
#endif
//******************* ��������stm32f10x.h ��ȫ������������.c�ļ�������������*******************//	
#include "stm32f10x.h"
#include <OLED.h>
#include <math.h>
#include <stdio.h>
#include <beep.h>
#include "bmp.h"
//***********************�궨��.C������main����֮���Ӧ������*****************************************************//
		
/***********��߻���ʶ��������0��******************/		
#define GPIO_FIRE_CLK0  RCC_APB2Periph_GPIOC  //ʱ��
#define GPIO_FIRE_PORT0 GPIOC                 //���洫�����뿪�������ӵ����ŵļĴ���
#define GPIO_FIRE_Pin0  GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10          //���洫�����뿪�������ӵ����� 
		
/***********�ұ߻���ʶ��������1��*********************/
#define GPIO_FIRE_CLK1  RCC_APB2Periph_GPIOB  //ʱ��
#define GPIO_FIRE_PORT1 GPIOB                 //���洫�����뿪�������ӵ����ŵļĴ���
#define GPIO_FIRE_Pin1  GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9          //���洫�����뿪�������ӵ����� 

/***********�������ƫ�����ʶ��������***********************/
#define GPIO_FIRE_MIDDLE0 GPIO_Pin_6|GPIO_Pin_7
#define GPIO_FIRE_MIDDLE1 GPIO_Pin_8|GPIO_Pin_9
//#define GPIO_FIRE_Pin0L	GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8 //�����λ����ʶ�������	
//#define GPIO_FIRE_Pin1R  GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_7 //�ұ���λ����ʶ�������
//*************ȫ�ֺ�������*********************************************************//
		FIRE_EXT void FIRE_CONFIG0(void);//0�Ż���ʶ��ģ��
		FIRE_EXT void FIRE_CONFIG1(void);//1�û���ʶ��ģ��
		FIRE_EXT void fire_cheak(void);//����ʶ������ʾ
//*************************************************************************************//
#endif