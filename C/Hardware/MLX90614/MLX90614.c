#include "stm32f10x.h"
#include "MLX90614.h"


void SMBus_Delay(u16 time)
{
    u16 i, j;
    for (i=0; i<5; i++)
    {
        for (j=0; j<time; j++);
    }
}

void SMBus_StartBit(void)
{
    SMBUS_SDA_H();             //SDA置高
    SMBus_Delay(8);            //延时
    SMBUS_SCK_H();             //SCL置高
    SMBus_Delay(8);            //延时
    SMBUS_SDA_L();             //SDA置低
    SMBus_Delay(8);            //等待模块回应
    SMBUS_SCK_L();             //SCL置低
    SMBus_Delay(8);            //延时
}

void SMBus_StopBit(void)
{
    SMBUS_SCK_L();             //SCL置低
    SMBus_Delay(8);            //延时
    SMBUS_SDA_L();             //SDA置低
    SMBus_Delay(8);            //延时
    SMBUS_SCK_H();             //SCL置高
    SMBus_Delay(8);            //延时
    SMBUS_SDA_H();             //SDA置高
}

u8 SMBus_ReceiveBit(void)
{
    u8 Ack_bit;
		
    SMBUS_SDA_H();          	 //SDA置高
    SMBus_Delay(6);            //拉高后等待一段时间
    SMBUS_SCK_H();             //SCL置高
    SMBus_Delay(8);            //再延时等待一段时间
    if(SMBUS_SDA_PIN())			 	 //将SDA模式转换为接收//检测有无应答
    {
        Ack_bit=1;						 //有应答返回1
    }
    else
    {
        Ack_bit=0;						 //没有应答返回0
    }
    SMBUS_SCK_L();             //SCL置低
    SMBus_Delay(8);            //延时进入下一段

    return        Ack_bit;		 //返回值
}

void SMBus_SendBit(u8 bit_out)
{
    if(bit_out==0)						 //检测Tx_buffer状态
    {
        SMBUS_SDA_L();				 //写入0
    }
    else
    {
        SMBUS_SDA_H();				 //写入1
    }
    SMBus_Delay(6);            //延时
    SMBUS_SCK_H();             //SCL置高
    SMBus_Delay(8);            //延时
    SMBUS_SCK_L();             //SCL置地
    SMBus_Delay(8);            //延时
    return;										 //返回
}

u8 SMBus_SendByte(u8 Tx_buffer)
{
    u8        Bit_counter;
    u8        Ack_bit;
    u8        bit_out;

    for(Bit_counter=8;Bit_counter;Bit_counter--)//从左到右循环八次
    {
        if (Tx_buffer&0x80)		 //检测Tx_buffer是否为1
        {
            bit_out=1;  			 //如果Tx_buffer的当前位为1，设置bit_out为1
        }
        else
        {
            bit_out=0;  			 //不是则设置bit_out为0
        }
        SMBus_SendBit(bit_out);//写入字节
        Tx_buffer<<=1;         //向左移一位
    }
    Ack_bit=SMBus_ReceiveBit();//读取确认位
    return        Ack_bit;		 //返回
}

u8 SMBus_ReceiveByte(u8 ack_nack)
{
    u8         RX_buffer;
    u8        Bit_Counter;

    for(Bit_Counter=8;Bit_Counter;Bit_Counter--)//从左到右循环八次
    {
        if(SMBus_ReceiveBit()) 
        {
            RX_buffer <<= 1;    
            RX_buffer |=0x01;
        }
        else
        {
            RX_buffer <<= 1;  
            RX_buffer &=0xfe;
        }
    }
    SMBus_SendBit(ack_nack);   
    return RX_buffer;
}

u8 PEC_Calculation(u8 pec[])
{
    u8         crc[6];
    u8        BitPosition=47;
    u8        shift;
    u8        i;
    u8        j;
    u8        temp;

    do
    {
        /*Load pattern value 0x000000000107*/
        crc[5]=0;
        crc[4]=0;
        crc[3]=0;
        crc[2]=0;
        crc[1]=0x01;
        crc[0]=0x07;

        /*Set maximum bit position at 47 ( six bytes byte5...byte0,MSbit=47)*/
        BitPosition=47;

        /*Set shift position at 0*/
        shift=0;

        /*Find first "1" in the transmited message beginning from the MSByte byte5*/
        i=5;
        j=0;
        while((pec[i]&(0x80>>j))==0 && i>0)
        {
            BitPosition--;
            if(j<7)
            {
                j++;
            }
            else
            {
                j=0x00;
                i--;
            }
        }/*End of while */

        /*Get shift value for pattern value*/
        shift=BitPosition-8;

        /*Shift pattern value */
        while(shift)
        {
            for(i=5; i<0xFF; i--)
            {
                if((crc[i-1]&0x80) && (i>0))
                {
                    temp=1;
                }
                else
                {
                    temp=0;
                }
                crc[i]<<=1;
                crc[i]+=temp;
            }/*End of for*/
            shift--;
        }/*End of while*/

        /*Exclusive OR between pec and crc*/
        for(i=0; i<=5; i++)
        {
            pec[i] ^=crc[i];
        }/*End of for*/
    }
    while(BitPosition>8); /*End of do-while*/

    return pec[0];
}

u16 SMBus_ReadMemory(u8 slaveAddress, u8 command)
{
    u16 data;                        // Data storage (DataH:DataL)
    u8 Pec;                                // PEC byte storage
    u8 DataL=0;                        // Low data byte storage
    u8 DataH=0;                        // High data byte storage
    u8 arr[6];                        // Buffer for the sent bytes
    u8 PecReg;                        // Calculated PEC byte storage
    u8 ErrorCounter;        // Defines the number of the attempts for communication with MLX90614

    ErrorCounter=0x00;                                // Initialising of ErrorCounter
        slaveAddress <<= 1;        //2-7???????
        
    do
    {
repeat:
        SMBus_StopBit();                            //If slave send NACK stop comunication
        --ErrorCounter;                                    //Pre-decrement ErrorCounter
        if(!ErrorCounter)                             //ErrorCounter=0?
        {
            break;                                            //Yes,go out from do-while{}
        }

        SMBus_StartBit();                                //Start condition
        if(SMBus_SendByte(slaveAddress))//Send SlaveAddress ???Wr=0????????
        {
            goto        repeat;                            //Repeat comunication again
        }
        if(SMBus_SendByte(command))            //Send command
        {
            goto        repeat;                            //Repeat comunication again
        }

        SMBus_StartBit();                                        //Repeated Start condition
        if(SMBus_SendByte(slaveAddress+1))        //Send SlaveAddress ???Rd=1????????
        {
            goto        repeat;                     //Repeat comunication again
        }

        DataL = SMBus_ReceiveByte(ACK);        //Read low data,master must send ACK
        DataH = SMBus_ReceiveByte(ACK); //Read high data,master must send ACK
        Pec = SMBus_ReceiveByte(NACK);        //Read PEC byte, master must send NACK
        SMBus_StopBit();                                //Stop condition

        arr[5] = slaveAddress;                //
        arr[4] = command;                        //
        arr[3] = slaveAddress+1;        //Load array arr
        arr[2] = DataL;                                //
        arr[1] = DataH;                                //
        arr[0] = 0;                                        //
        PecReg=PEC_Calculation(arr);//Calculate CRC
    }
    while(PecReg != Pec);                //If received and calculated CRC are equal go out from do-while{}

        data = (DataH<<8) | DataL;        //data=DataH:DataL
    return data;
}

void SMBus_Init()
{
    GPIO_InitTypeDef    GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SMBUS_PORT, ENABLE);
    GPIO_InitStructure.GPIO_Pin = SMBUS_SCK | SMBUS_SDA;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SMBUS_PORT, &GPIO_InitStructure);
    SMBUS_SCK_H();
    SMBUS_SDA_H();
}

float SMBus_ReadTemp(void)
{   
        return (SMBus_ReadMemory(SA, RAM_ACCESS|RAM_TOBJ1)*0.02-273.15);
}


