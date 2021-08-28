/*
 * iic.h
 *
 *  Created on: 2018-2-5
 *      Author: Administrator
 */

#ifndef IIC_H_
#define IIC_H_


#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File


#define IIC_SCL_SETH	(GpioDataRegs.GPBSET.bit.GPIO33=1)
#define IIC_SCL_SETL	(GpioDataRegs.GPBCLEAR.bit.GPIO33=1)

#define IIC_SDA_SETH	(GpioDataRegs.GPBSET.bit.GPIO32=1)
#define IIC_SDA_SETL	(GpioDataRegs.GPBCLEAR.bit.GPIO32=1)

#define READ_SDA   		(GpioDataRegs.GPBDAT.bit.GPIO32)



//IIC所有操作函数
void IICA_Init(void);                //初始化IIC的IO口
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(unsigned char txd);			//IIC发送一个字节
unsigned char IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
unsigned char IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号


#endif /* IIC_H_ */
