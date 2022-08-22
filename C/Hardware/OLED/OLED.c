//******************************************************************************//
#define OLED_GLOBALS
#include "OLED.h"
#include "OLED_Font.h"
//******************************************************************************//
void IIC_Start()
{
	OLED_SCL_Set() ;
	OLED_SDA_Set();
	OLED_SDA_Reset();
	OLED_SCL_Reset();
}
//******************************************************************************//
void IIC_Stop()
{
    OLED_SCL_Set() ;
	OLED_SDA_Reset();
	OLED_SDA_Set();
}
//******************************************************************************//
void IIC_Wait_Ack()
{
	OLED_SCL_Set() ;
	OLED_SCL_Reset();
}
//******************************************************************************//
void Write_IIC_Byte(unsigned char da)
{
	unsigned char i;
	OLED_SCL_Reset();
	for(i=0;i<8;i++){
		if(da&0x80){
            OLED_SDA_Set();
        }
		else{
            OLED_SDA_Reset();
        }
        da=da<<1;
		OLED_SCL_Set();
		OLED_SCL_Reset();
    }
}
//******************************************************************************//



void OLED_Cmd(unsigned char cmd)
{
    IIC_Start();
    //Slave address,SA0=0
    Write_IIC_Byte(0x78);            
	IIC_Wait_Ack();	
    //write command
    Write_IIC_Byte(0x00);
	IIC_Wait_Ack();	
    Write_IIC_Byte(cmd);
    IIC_Wait_Ack();	
    IIC_Stop();
}
//******************************************************************************//
void OLED_Data(unsigned char data)
{
    IIC_Start();
    //Slave address,SA0=0
    Write_IIC_Byte(0x78);
    IIC_Wait_Ack();	
    //write data
    Write_IIC_Byte(0x40);
	IIC_Wait_Ack();	
    Write_IIC_Byte(data);
	IIC_Wait_Ack();	
    IIC_Stop();
}
//******************************************************************************//
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{
    OLED_Cmd(0xb0+y);
    OLED_Cmd(((x&0xf0)>>4)|0x10);
	OLED_Cmd((x&0x0f)); 
}   	  
//******************************************************************************//  
void OLED_Display_On(void)
{
	OLED_Cmd(0X8D);  //SET DCDC命令
	OLED_Cmd(0X14);  //DCDC ON
	OLED_Cmd(0XAF);  //DISPLAY ON
}
//******************************************************************************//     
void OLED_Display_Off(void)
{
	OLED_Cmd(0X8D);  //SET DCDC命令
	OLED_Cmd(0X10);  //DCDC OFF
	OLED_Cmd(0XAE);  //DISPLAY OFF
}		   			 
//******************************************************************************//  
void OLED_Clear(void)  
{  
	unsigned char  i,n;		    
	for(i=0;i<8;i++)  {  
		OLED_Cmd (0xb0+i);    //设置页地址（0~7）
		OLED_Cmd (0x00);      //设置显示位置—列低地址
		OLED_Cmd (0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++){
            OLED_Data(0); 
        }
	}
}

//******************************************************************************//
void OLED_Delay(unsigned int t)
{
	unsigned int i;
	while(t--){	
        i=10000;
        while(i--);
	}
}
//******************************************************************************//
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char ch)
{
    unsigned char i=0;
    unsigned int index=ch-' ';
    OLED_Set_Pos(x,y);	
    for(i=0;i<8;i++){
        OLED_Data(F8X16[(index<<4)+i]);
    }
    OLED_Set_Pos(x,y+1);
    for(i=0;i<8;i++){
        OLED_Data(F8X16[(index<<4)+8+i]);
    }
}
//******************************************************************************//
void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *pStr)
{
	unsigned char j=0;
	while (pStr[j]!='\0'){
        OLED_ShowChar(x,y,pStr[j]);
        x+=8;
        if(x>120){
            x=0;
            y+=2;
        }
        j++;
	}
}
//显示一个字符号串
void OLED_ShowString1(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//******************************************************************************//			 
void OLED_ShowPicture(unsigned char x0, unsigned char y0,unsigned char length, unsigned char width,unsigned char BMP[])
{  
 unsigned int j=0;
 unsigned char x,y;

  if(width%8==0)
  {      
      width=width/8;      
  }
  else 
  {
      width=width/8+1;
  }
  for(y=0;y<width;y++)
  {
      OLED_Set_Pos(x0,y0+y);
      for(x=0;x<length;x++)
      {      
          OLED_Data(BMP[j++]);      
      }
  }
}
//******************************************************************************//
void OLED_Init(void)
{ 		 
 	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(SCK_RCC_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SCL_PIN;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(SCL_PORT, &GPIO_InitStructure);
 
	RCC_APB2PeriphClockCmd(SDA_RCC_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SDA_PIN;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(SDA_PORT, &GPIO_InitStructure);

    
 	OLED_SCL_Set();
    OLED_SDA_Set(); 
    OLED_Delay(800);
    OLED_Cmd(0xAE);//--display off
	OLED_Cmd(0x00);//---set low column address
	OLED_Cmd(0x10);//---set high column address
	OLED_Cmd(0x40);//--set start line address  
	OLED_Cmd(0xB0);//--set page address
	OLED_Cmd(0x81); // contract control
	OLED_Cmd(0xFF);//--128   
	OLED_Cmd(0xA1);//set segment remap 
	OLED_Cmd(0xA6);//--normal / reverse
	OLED_Cmd(0xA8);//--set multiplex ratio(1 to 64)
	OLED_Cmd(0x3F);//--1/32 duty
	OLED_Cmd(0xC8);//Com scan direction
	OLED_Cmd(0xD3);//-set display offset
	OLED_Cmd(0x00);//
	OLED_Cmd(0xD5);//set osc division
	OLED_Cmd(0x80);//
	OLED_Cmd(0xD8);//set area color mode off
	OLED_Cmd(0x05);//
	OLED_Cmd(0xD9);//Set Pre-Charge Period
	OLED_Cmd(0xF1);//
	OLED_Cmd(0xDA);//set com pin configuartion
	OLED_Cmd(0x12);//
	OLED_Cmd(0xDB);//set Vcomh
	OLED_Cmd(0x30);//
	OLED_Cmd(0x8D);//set charge pump enable
	OLED_Cmd(0x14);//
	OLED_Cmd(0xAF);//--turn on oled panel
    
    OLED_Clear();
}  
//******************************************************************************//
