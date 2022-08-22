#include "Mator.h"


//********************风扇马达引脚使能****************************************
void MotorEn_Init(void)
{
	RCC_APB2PeriphClockCmd(GPIO_mator_CLK,ENABLE);  	//使能C口GPIO时钟

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;     		//选择推挽输出
	GPIO_InitStruct.GPIO_Pin=GPIO_PIN_maotr1|GPIO_PIN_maotr2;					//指定引脚PC0,MotEn2
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;			//设置输出速率10MHz
	GPIO_Init(GPIO_PORT_mator,&GPIO_InitStruct);						//初始化外设GPIOC寄存器
	
	GPIO_ResetBits(GPIO_PORT_mator, GPIO_PIN_maotr1);//引脚置0，防止有意外发生
	GPIO_ResetBits(GPIO_PORT_mator, GPIO_PIN_maotr2);//引脚置0，防止有意外发生
}
//********************风扇马达驱动代码***************************************
void Set_Mator(void)
{
		MotorEn_Init();  //引用相关引脚使能				
	GPIO_SetBits(GPIO_PORT_mator,GPIO_PIN_maotr1);   //输出1，开启马达		
}
//********************风扇马达关闭代码***************************************
void Reset_Mator(void)
{
		MotorEn_Init();  //引用相关引脚使能					
	//GPIO_ResetBits(GPIO_PORT_mator,GPIO_PIN_maotr1);   //输出0，关闭马达		
}