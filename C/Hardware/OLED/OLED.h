//**************************************************************************************//
#ifndef __OLED_H
#define __OLED_H	
//**************************************************************************************//
#ifndef OLED_GLOBALS
    #define OLED_EXT extern
#else
    #define OLED_EXT
#endif
//*************************************************************************************//
#include "stm32f10x.h"
//*************************************************************************************//
#define SCL_PORT    GPIOB
#define SCL_PIN     GPIO_Pin_13
#define SDA_PORT    GPIOB
#define SDA_PIN     GPIO_Pin_15

#define SCK_RCC_CLK RCC_APB2Periph_GPIOB
#define SDA_RCC_CLK RCC_APB2Periph_GPIOB
//*************************************************************************************//			   
#define OLED_SCL_Reset()    GPIO_ResetBits(SCL_PORT,SCL_PIN)
#define OLED_SCL_Set()      GPIO_SetBits(SCL_PORT,SCL_PIN)

#define OLED_SDA_Reset()    GPIO_ResetBits(SDA_PORT,SDA_PIN)
#define OLED_SDA_Set()      GPIO_SetBits(SDA_PORT,SDA_PIN)
//*************************************************************************************// 		     
OLED_EXT void OLED_Init(void);
OLED_EXT void OLED_Clear(void);
OLED_EXT void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *pStr);
OLED_EXT void OLED_ShowString1(u8 x,u8 y,u8 *chr);
OLED_EXT void OLED_ShowPicture(unsigned char x0, unsigned char y0,unsigned char length, unsigned char width,unsigned char BMP[]);
//*************************************************************************************//
#endif  
//*************************************************************************************//
