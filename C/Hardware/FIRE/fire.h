/*注释掉方莉的代码
#ifndef _FIRE_H
#define _FIRE_H
#include "stm32f10x.h"

//宏定义
#define GPIO_FIRE_CLK  RCC_APB2Periph_GPIOC  //时钟
#define GPIO_FIRE_PORT GPIOC                 //火焰传感器与开发板连接的引脚的寄存器
#define GPIO_FIRE_Pin  GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10          //火焰传感器与开发板连接的引脚 

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
//******************* 申明引用stm32f10x.h 的全部函数，后面.c文件无需另外申明*******************//	
#include "stm32f10x.h"
#include <OLED.h>
#include <math.h>
#include <stdio.h>
#include <beep.h>
#include "bmp.h"
//***********************宏定义.C函数与main函数之间对应的引脚*****************************************************//
		
/***********左边火焰识别器——0号******************/		
#define GPIO_FIRE_CLK0  RCC_APB2Periph_GPIOC  //时钟
#define GPIO_FIRE_PORT0 GPIOC                 //火焰传感器与开发板连接的引脚的寄存器
#define GPIO_FIRE_Pin0  GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10          //火焰传感器与开发板连接的引脚 
		
/***********右边火焰识别器——1号*********************/
#define GPIO_FIRE_CLK1  RCC_APB2Periph_GPIOB  //时钟
#define GPIO_FIRE_PORT1 GPIOB                 //火焰传感器与开发板连接的引脚的寄存器
#define GPIO_FIRE_Pin1  GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9          //火焰传感器与开发板连接的引脚 

/***********定义各边偏向火焰识别器引脚***********************/
#define GPIO_FIRE_MIDDLE0 GPIO_Pin_6|GPIO_Pin_7
#define GPIO_FIRE_MIDDLE1 GPIO_Pin_8|GPIO_Pin_9
//#define GPIO_FIRE_Pin0L	GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8 //左边三位火焰识别灯引脚	
//#define GPIO_FIRE_Pin1R  GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_7 //右边三位火焰识别灯引脚
//*************全局函数声明*********************************************************//
		FIRE_EXT void FIRE_CONFIG0(void);//0号火焰识别模块
		FIRE_EXT void FIRE_CONFIG1(void);//1好火焰识别模块
		FIRE_EXT void fire_cheak(void);//火焰识别与显示
//*************************************************************************************//
#endif