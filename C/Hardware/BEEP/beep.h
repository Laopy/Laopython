/*ע�͵�����Ĵ���
#ifndef _BEEP_H_
#define _BEEP_H_
#include "stm32f10x.h" 


//�궨��
#define BEEP_GPIO GPIOC  //BEEP�˿�ΪGPIOC
#define BEEP_Pin GPIO_Pin_13 


//
//��ʼ��
; 
void BEEP_Init(void); 
#endif
*/
#ifndef _BEEP_H_
#define _BEEP_H_
//**************************************************************************************//
#ifndef BEEP_GLOBALS
    #define BEEP_EXT extern
#else
    #define BEEP_EXT
#endif
//*************************************************************************************//	
#include "stm32f10x.h"
//*************************************************************************************//
	#define BEEP_PIN     GPIO_Pin_13	
  #define BEEP_PORT    GPIOC

//*************ȫ�ֺ�������*********************************************************//
		BEEP_EXT void Buzzer_Init(void);//��������
		BEEP_EXT void tip_warnings_init(void);//��������
		
	
//*************************************************************************************//
#endif