/*
 * led.c
 *
 *  Created on: 2010/12/3
 *      Author: YellowNew
 */
#include "leds.h"

/*******************************************************************************
* 函 数 名         : LED_Init
* 函数功能		   : LED初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void LED_Init(void)
{
	EALLOW;
	SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// 开启GPIO时钟

	//LED1端口配置
	GpioCtrlRegs.GPBMUX2.bit.GPIO60=0;//设置为通用GPIO功能
	GpioCtrlRegs.GPBDIR.bit.GPIO60=1;//设置GPIO方向为输出
	GpioCtrlRegs.GPBPUD.bit.GPIO60=0;//使能GPIO上拉电阻

	//LED2端口配置
	GpioCtrlRegs.GPBMUX2.bit.GPIO61=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO61=1;
	GpioCtrlRegs.GPBPUD.bit.GPIO61=0;

	EDIS;
}
