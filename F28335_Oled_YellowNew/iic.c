/*
 * iic.c
 *
 *  Created on: 2021/4/23
 *      Author: YellowNEW
 */
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "iic.h"


/*******************************************************************************
* 函 数 名         : IIC_Init
* 函数功能		   : IIC初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IICA_Init(void)
{
	EALLOW;
	SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// 开启GPIO时钟
	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;	  	//上拉
	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;   	// 输出端口
	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;  	// IO口
	GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3; 	// 不同步

	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;	  	//上拉
	GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;   	// 输出端口
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;  	// IO口
	GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;   // 不同步
	EDIS;
}


/*******************************************************************************
* 函 数 名         : IIC_Start
* 函数功能		   : 产生IIC起始信号
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IIC_Start(void)
{

	IIC_SDA_SETH;    //先将SDA置高
	IIC_SCL_SETH;    //再将SCL置低
	DELAY_US(5);     //延时一小段时间稳定一下
	IIC_SDA_SETL;    //在SCL为高电平期间SDA由高变低完成起使信号
	DELAY_US(6);     //延时一小段时间稳定一下
	IIC_SCL_SETL;    //钳住I2C总线，准备发送或接收数据
}

/*******************************************************************************
* 函 数 名         : IIC_Stop
* 函数功能		   : 产生IIC停止信号
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IIC_Stop(void)
{

	IIC_SDA_SETL;//先将SDA置低
	IIC_SCL_SETH;//然后在将SCL置为高电平
 	DELAY_US(6); //延时一段时间稳定一下
 	IIC_SDA_SETH;//然后在SCL为高电平期间将SDA置低电平实现停止信号
	DELAY_US(6); //延时一小段时间稳定一下
}

/*******************************************************************************
* 函 数 名         : IIC_Wait_Ack
* 函数功能		   : 等待应答信号到来
* 输    入         : 无
* 输    出         : 1，接收应答失败
        			 0，接收应答成功
*******************************************************************************/
unsigned char IIC_Wait_Ack(void)
{
	unsigned char tempTime=0;
	IIC_SDA_SETH;//主机将SDA释放，及将SDA置为高电平
	DELAY_US(1);
	IIC_SCL_SETH;//主机将SCL置高等待从机控制SDA产生一个应答信号
	DELAY_US(1);
	while(READ_SDA)//如果读取（READ_SDA）为低电平说明从机响应的应答
	{
		tempTime++;
		if(tempTime>250)
		{
			IIC_Stop();//如果从机没有将SDA置为低电平说明没有产生应答此时主机需要产生停止信号
			return 1;
		}
	}
	IIC_SCL_SETL;//时钟输出0
	return 0;
}
/*
 * 因为OLED我们通常都是写操作并不会取读取数据因此下面的函数在本次工程中并没有使用
 */

/*******************************************************************************
* 函 数 名         : IIC_Ack
* 函数功能		   : 产生ACK应答
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IIC_Ack(void)
{
	IIC_SCL_SETL;
	IIC_SDA_SETL;
	DELAY_US(2);
	IIC_SCL_SETH;
	DELAY_US(5);
	IIC_SCL_SETL;
}

/*******************************************************************************
* 函 数 名         : IIC_NAck
* 函数功能		   : 产生NACK非应答
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IIC_NAck(void)
{
	IIC_SCL_SETL;
	IIC_SDA_SETH;
	DELAY_US(2);
	IIC_SCL_SETH;
	DELAY_US(5);
	IIC_SCL_SETL;
}

/*******************************************************************************
* 函 数 名         : IIC_Send_Byte
* 函数功能		   : IIC发送一个字节
* 输    入         : txd：发送一个字节
* 输    出         : 无
*******************************************************************************/
void IIC_Send_Byte(unsigned char txd)
{
	unsigned char t;
	IIC_SCL_SETL;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {
        if(txd&0x80) //0x80  1000 0000
        	IIC_SDA_SETH;
		else
			IIC_SDA_SETL;
        txd<<=1;
        DELAY_US(2);
        IIC_SCL_SETH;//时钟拉高将数据传送输出
		DELAY_US(2);
		IIC_SCL_SETL;//时钟再次拉低准备就受下一个SDA的跳变
		DELAY_US(2);
    }
}

/*******************************************************************************
* 函 数 名       : IIC_Read_Byte
* 函数功能     : IIC读一个字节
* 输    入         : ack=1时，发送ACK，ack=0，发送nACK
* 输    出         : 应答或非应答
*******************************************************************************/
unsigned char IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
    for(i=0;i<8;i++ )
	{
    	IIC_SCL_SETL;
    	DELAY_US(2);
        IIC_SCL_SETH;
        receive<<=1;
        if(READ_SDA)receive++;
        DELAY_US(1);
    }
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK
    return receive;
}

