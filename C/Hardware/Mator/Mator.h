#ifndef _MATOR_H_
#define _MATOR_H_
//**************************************************************************************//
#ifndef MATOR_GLOBALS
    #define MATOR_EXT extern
#else
    #define MATOR_EXT
#endif
//*******************************************************************************************//	
		#include "stm32f10x.h"
//*******************马达相关引脚定义**********************************************************//	
  #define GPIO_PIN_maotr1 GPIO_Pin_11
	#define GPIO_PIN_maotr2 GPIO_Pin_12
	#define GPIO_PORT_mator GPIOA		
	#define GPIO_mator_CLK  RCC_APB2Periph_GPIOA  //时钟
//****************************************************************************************//
		
		MATOR_EXT void Set_Mator(void);
		MATOR_EXT void Reset_Mator(void);
#endif
		
		
		