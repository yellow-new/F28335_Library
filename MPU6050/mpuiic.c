#include "mpuiic.h"
#include "public.h"


//初始化IIC
void MPU_IIC_Init(void)
{
       EALLOW;
       SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// 开启GPIO时钟
       GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;     //上拉
       GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;     // 输出端口
       GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;    // IO口
       GpioCtrlRegs.GPAQSEL1.bit.GPIO10 = 3;   // 不同步

       GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;     //上拉
       GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;     // 输出端口
       GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;    // IO口
       GpioCtrlRegs.GPAQSEL1.bit.GPIO11 = 3;   // 不同步

       EDIS;

 
}
//产生IIC起始信号
void MPU_IIC_Start(void)
{
    MPU_SDA_OUT();     //sda线输出
    MPU_SDA_SETH;
    MPU_SCL_SETH;
    DELAY_US(5);
    MPU_SDA_SETL;//START:when CLK is high,DATA change form high to low
    DELAY_US(6);
    MPU_SCL_SETL;//钳住I2C总线，准备发送或接收数据

}	  
//产生IIC停止信号
void MPU_IIC_Stop(void)
{
	MPU_SDA_OUT();//sda线输出


	MPU_SCL_SETL;
	MPU_SDA_SETL;//STOP:when CLK is high DATA change form low to high
	MPU_SCL_SETH;
    DELAY_US(6);
    MPU_SDA_SETH;//发送I2C总线结束信号
    DELAY_US(6);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 MPU_IIC_Wait_Ack(void)
{
    unsigned char tempTime=0;
    MPU_SDA_SETH;
    DELAY_US(1);
    MPU_SDA_IN();      //SDA设置为输入
    MPU_SCL_SETH;
    DELAY_US(1);
    while(MPU_READ_SDA)
    {
        tempTime++;
        if(tempTime>250)
        {
            MPU_IIC_Stop();
            return 1;
        }
    }
    MPU_SCL_SETL;//时钟输出0
    return 0;
} 
//产生ACK应答
void MPU_IIC_Ack(void)
{
    MPU_SCL_SETL;
    MPU_SDA_OUT();//sda线输出
    MPU_SDA_SETL;
    DELAY_US(2);
    MPU_SCL_SETH;
    DELAY_US(5);
    MPU_SCL_SETL;
}
//不产生ACK应答		    
void MPU_IIC_NAck(void)
{
    MPU_SCL_SETL;
    MPU_SDA_OUT();//sda线输出
    MPU_SDA_SETH;
    DELAY_US(2);
    MPU_SCL_SETH;
    DELAY_US(5);
    MPU_SCL_SETL;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void MPU_IIC_Send_Byte(u8 txd)
{                        
    unsigned char t;
    MPU_SDA_OUT();//sda线输出
    MPU_SCL_SETL;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {
        if((txd&0x80)>0) //0x80  1000 0000
            MPU_SDA_SETH;
        else
            MPU_SDA_SETL;
        txd<<=1;
        DELAY_US(2);   //对TEA5767这三个延时都是必须的
        MPU_SCL_SETH;
        DELAY_US(2);
        MPU_SCL_SETL;
        DELAY_US(2);
    }
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 MPU_IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    MPU_SDA_IN();      //SDA设置为输入
    for(i=0;i<8;i++ )
    {
        MPU_SCL_SETL;
        DELAY_US(2);
        MPU_SCL_SETH;
        receive<<=1;
        if(MPU_READ_SDA)receive++;
        DELAY_US(1);
    }
    if (!ack)
        MPU_IIC_NAck();//发送nACK
    else
        MPU_IIC_Ack(); //发送ACK
    return receive;
}


























