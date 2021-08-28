/*
 *  smg.c
 *
 *  Created on: 2021-3-5
 *      Author: Huang'xin and zou'yong
 */
#include "DSP2833x_Device.h"   //DSP2833x Headerfile
#include "DSP2833x_Examples.h"   //DSP2833x Examples
#include "smg.h"
#include "LED.h"

Uint16 s = 0,VOUT,bit1[6],t = 0,bit2;

  //数码管段选设定
void Display(Uint16 bit,Uint16 bit2)
{

	switch(bit)
	{
        case(0): SEGA_1L;SEGA_2H;SEGA_3H;SEGA_4H;
                 SEGA_5L;SEGA_6H;SEGA_7H;SEGA_8H;
                 break;
        case(1): SEGA_1L;SEGA_2L;SEGA_3H;SEGA_4H;
                 SEGA_5L;SEGA_6L;SEGA_7L;SEGA_8L;
                 break;
        case(2): SEGA_1H;SEGA_2H;SEGA_3L;SEGA_4H;
                 SEGA_5L;SEGA_6L;SEGA_7H;SEGA_8H;
                 break;
        case(3): SEGA_1H;SEGA_2H;SEGA_3H;SEGA_4H;
                 SEGA_5L;SEGA_6L;SEGA_7L;SEGA_8H;
                 break;
        case(4): SEGA_1H;SEGA_2L;SEGA_3H;SEGA_4H;
                 SEGA_5L;SEGA_6H;SEGA_7L;SEGA_8L;
                 break;
        case(5): SEGA_1H;SEGA_2H;SEGA_3H;SEGA_4L;
                 SEGA_5L;SEGA_6H;SEGA_7L;SEGA_8H;
                 break;
        case(6): SEGA_1H;SEGA_2H;SEGA_3H;SEGA_4L;
                 SEGA_5L;SEGA_6H;SEGA_7H;SEGA_8H;
                 break;
        case(7): SEGA_1L;SEGA_2L;SEGA_3H;SEGA_4H;
                 SEGA_5L;SEGA_6L;SEGA_7L;SEGA_8H;
                 break;
        case(8): SEGA_1H;SEGA_2H;SEGA_3H;SEGA_4H;
                 SEGA_5L;SEGA_6H;SEGA_7H;SEGA_8H;
                 break;
        case(9): SEGA_1H;SEGA_2H;SEGA_3H;SEGA_4H;
                 SEGA_5L;SEGA_6H;SEGA_7L;SEGA_8H;
                 break;
}
            if(bit2 == 1) SEGA_5H;
            if(bit2 == 3) SEGA_5H;
}


//数码管初始化
void SMG_Init(void)
{
	EALLOW;
	SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// 开启GPIO时钟
	//SMG IO口 数据控制端口配置
	GpioCtrlRegs.GPAMUX1.bit.GPIO2=0;
	GpioCtrlRegs.GPADIR.bit.GPIO2=1;
	GpioCtrlRegs.GPAPUD.bit.GPIO2=0;

//    GpioCtrlRegs.GPAMUX1.bit.GPIO1=0;
//    GpioCtrlRegs.GPADIR.bit.GPIO1=1;
//    GpioCtrlRegs.GPAPUD.bit.GPIO1=0;


//	GpioCtrlRegs.GPAMUX1.bit.GPIO3=0;
//	GpioCtrlRegs.GPADIR.bit.GPIO3=1;
//	GpioCtrlRegs.GPAPUD.bit.GPIO3=0;

	GpioCtrlRegs.GPAMUX1.bit.GPIO4=0;
	GpioCtrlRegs.GPADIR.bit.GPIO4=1;
	GpioCtrlRegs.GPAPUD.bit.GPIO4=0;

//	GpioCtrlRegs.GPAMUX1.bit.GPIO5=0;
//	GpioCtrlRegs.GPADIR.bit.GPIO5=1;
//	GpioCtrlRegs.GPAPUD.bit.GPIO5=0;

	GpioCtrlRegs.GPAMUX1.bit.GPIO6=0;
	GpioCtrlRegs.GPADIR.bit.GPIO6=1;
	GpioCtrlRegs.GPAPUD.bit.GPIO6=0;

//	GpioCtrlRegs.GPAMUX1.bit.GPIO7=0;
//	GpioCtrlRegs.GPADIR.bit.GPIO7=1;
//	GpioCtrlRegs.GPAPUD.bit.GPIO7=0;

	GpioCtrlRegs.GPBMUX2.bit.GPIO50=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO50=1;
	GpioCtrlRegs.GPBPUD.bit.GPIO50=0;


	GpioCtrlRegs.GPBMUX2.bit.GPIO51;
	GpioCtrlRegs.GPBDIR.bit.GPIO51=1;
	GpioCtrlRegs.GPBPUD.bit.GPIO51=0;

	GpioCtrlRegs.GPBMUX2.bit.GPIO52=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO52=1;
	GpioCtrlRegs.GPBPUD.bit.GPIO52=0;

	GpioCtrlRegs.GPBMUX2.bit.GPIO53=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO53=1;
	GpioCtrlRegs.GPBPUD.bit.GPIO53=0;

	GpioCtrlRegs.GPBMUX2.bit.GPIO54=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO54=1;
	GpioCtrlRegs.GPBPUD.bit.GPIO54=0;

	GpioCtrlRegs.GPBMUX2.bit.GPIO55=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO55=1;
	GpioCtrlRegs.GPBPUD.bit.GPIO55=0;

	GpioCtrlRegs.GPBMUX2.bit.GPIO56=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO56=1;
	GpioCtrlRegs.GPBPUD.bit.GPIO56=0;

	GpioCtrlRegs.GPBMUX2.bit.GPIO57=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO57=1;
	GpioCtrlRegs.GPBPUD.bit.GPIO57=0;

//	GpioCtrlRegs.GPAMUX1.bit.GPIO8=0;
//	GpioCtrlRegs.GPADIR.bit.GPIO8=1;
//	GpioCtrlRegs.GPAPUD.bit.GPIO8=0;

//	GpioCtrlRegs.GPAMUX1.bit.GPIO9=0;
//	GpioCtrlRegs.GPADIR.bit.GPIO9=1;
//	GpioCtrlRegs.GPAPUD.bit.GPIO9=0;
	EDIS;
//  控制位选引脚
	GpioDataRegs.GPACLEAR.bit.GPIO2=1;
	GpioDataRegs.GPACLEAR.bit.GPIO4=1;
	GpioDataRegs.GPACLEAR.bit.GPIO6=1;
//	GpioDataRegs.GPACLEAR.bit.GPIO8=1;
//	GpioDataRegs.GPACLEAR.bit.GPIO3=1;
//	GpioDataRegs.GPACLEAR.bit.GPIO5=1;
//	GpioDataRegs.GPACLEAR.bit.GPIO7=1;
//	GpioDataRegs.GPACLEAR.bit.GPIO9=1;
}


//数码管显示函数
void SMG_display(float Voltage,float Voltage1)
{
    if(t == 0)
    {
        VOUT = Voltage * 133;
        bit1[0] = VOUT/100;
        bit1[1] = VOUT%100/10;
        bit1[2] = VOUT%100%10;
        VOUT = Voltage1 * 200;
        bit1[3] = VOUT/100;
        bit1[4] = VOUT%100/10;
        bit1[5] = VOUT%100%10;
        t = 200;
    }
    t--;
    Display(bit1[s],s);
    switch(s)
    {
        case(0): smg_1L;smg_2L;smg_3L;break;
        case(1): smg_1L;smg_2L;smg_3H;break;
        case(2): smg_1L;smg_2H;smg_3L;break;

        case(3): smg_1H;smg_2L;smg_3L;break;
        case(4): smg_1H;smg_2L;smg_3H;break;
        case(5): smg_1H;smg_2H;smg_3L;break;

        default: break;
    }
    s++;
    if(s == 6)
    {
        s = 0;
    }
}
