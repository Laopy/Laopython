/*注释掉方莉的代码
#ifndef _BEEP_H_
#define _BEEP_H_
#include "stm32f10x.h" 


//宏定义
#define BEEP_GPIO GPIOC  //BEEP端口为GPIOC
#define BEEP_Pin GPIO_Pin_13 


//
//初始化
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

//*************全局函数声明*********************************************************//
		BEEP_EXT void Buzzer_Init(void);//单音警报
		BEEP_EXT void tip_warnings_init(void);//多音警报
		
	
//*************************************************************************************//
#endif