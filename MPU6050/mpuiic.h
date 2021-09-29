#ifndef __MPUIIC_H
#define __MPUIIC_H
#include "public.h"


//IO方向设置
#define MPU_SDA_IN()  {EALLOW;GpioCtrlRegs.GPADIR.bit.GPIO11 = 0;EDIS;}
#define MPU_SDA_OUT() {EALLOW;GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;EDIS;}

//IO操作函数	 
#define MPU_SCL_SETL  (GpioDataRegs.GPACLEAR.bit.GPIO10=1)
#define MPU_SCL_SETH  (GpioDataRegs.GPASET.bit.GPIO10=1)

#define MPU_SDA_SETL  (GpioDataRegs.GPACLEAR.bit.GPIO11=1)
#define MPU_SDA_SETH  (GpioDataRegs.GPASET.bit.GPIO11=1)



#define MPU_READ_SDA  (GpioDataRegs.GPADAT.bit.GPIO11)		//输入SDA

//IIC所有操作函数
void MPU_IIC_Delay(void);				//MPU IIC延时函数
void MPU_IIC_Init(void);                //初始化IIC的IO口				 
void MPU_IIC_Start(void);				//发送IIC开始信号
void MPU_IIC_Stop(void);	  			//发送IIC停止信号
void MPU_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 MPU_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void MPU_IIC_Ack(void);					//IIC发送ACK信号
void MPU_IIC_NAck(void);				//IIC不发送ACK信号

void IMPU_IC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















