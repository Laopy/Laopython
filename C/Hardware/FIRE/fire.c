#include "fire.h"

//***************火焰传感器0号机引脚使能***********************
void FIRE_CONFIG0(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   
   RCC_APB2PeriphClockCmd(GPIO_FIRE_CLK0 , ENABLE);  //使能时钟
   //使用数字信号进行信号传递，初始化为浮空输入模式
   GPIO_InitStructure.GPIO_Pin=GPIO_FIRE_Pin0;
   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; 
   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
   GPIO_Init(GPIO_FIRE_PORT0,&GPIO_InitStructure);
}
//***************火焰传感器1号机引脚使能***********************
void FIRE_CONFIG1(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   
   RCC_APB2PeriphClockCmd(GPIO_FIRE_CLK1 , ENABLE);  //使能时钟
   //使用数字信号进行信号传递，初始化为浮空输入模式
   GPIO_InitStructure.GPIO_Pin=GPIO_FIRE_Pin1;
   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; 
   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
   GPIO_Init(GPIO_FIRE_PORT1,&GPIO_InitStructure);
}
//***************火焰传感器读取与警报代码***********************
void fire_cheak(void)
{
	
		
}


