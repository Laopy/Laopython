#ifndef __SOLID_H
#define __SOLID_H	
//**************************************************************************************//
#ifndef SOLID_GLOBALS
    #define SOLID_EXT extern
#else
    #define SOLID_EXT
#endif
//*************************************************************************************//
#include "stm32f10x.h"
#include <OLED.h>
#include <math.h>
#include <stdio.h>
#include <beep.h>
#include "bmp.h"
#include <MLX90614.h>
#include <fire.h>
#include "Mator.h"

//*************************************************************************************//
	#define GPIO_Pin_Solid GPIO_Pin_5
	#define GPIO_Solid GPIOC


//*************************************************************************************// 		     

SOLID_EXT void InitADC1_15(void);
SOLID_EXT void Dta_time(unsigned int beatime);
//*************************************************************************************//
#endif  

/*/|*|/|*|*|/|�ر�˵��|/|*||*|/|*|*|/

.c�ļ���Ȼ��ע�ͣ����Ǵ�������ʺ���ÿվ�ȥ�Ż��Ż���û�վ����ˡ�






*/