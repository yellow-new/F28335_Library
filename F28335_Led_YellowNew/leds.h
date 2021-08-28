/*
 * led.h
 *
 *  Created on: 2020/12/3
 *      Author: YellowNew
 */

#ifndef _LEDS_H_
#define _LEDS_H_

#include "DSP2833x_Device.h"     // DSP2833x 头文件
#include "DSP2833x_Examples.h"   // DSP2833x 例子相关头文件


#define LED1_ON			(GpioDataRegs.GPBSET.bit.GPIO60=1)
#define LED1_OFF		(GpioDataRegs.GPBCLEAR.bit.GPIO60=1)
#define LED1_TOGGLE		(GpioDataRegs.GPBTOGGLE.bit.GPIO60=1)

#define LED2_ON			(GpioDataRegs.GPBSET.bit.GPIO61=1)
#define LED2_OFF		(GpioDataRegs.GPBCLEAR.bit.GPIO61=1)
#define LED2_TOGGLE		(GpioDataRegs.GPBTOGGLE.bit.GPIO61=1)
void LED_Init(void);


#endif /* LED_H_ */
