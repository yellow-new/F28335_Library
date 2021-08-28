/*
 * smg.h
 *
 *  Created on: 2018-1-25
 *      Author: Administrator
 */

#ifndef SMG_H_
#define SMG_H_

#include "DSP2833x_Device.h"     // DSP2833x 头文件
#include "DSP2833x_Examples.h"   // DSP2833x 例子相关头文件



#define smg_1H			(GpioDataRegs.GPASET.bit.GPIO2=1)
#define smg_1L			(GpioDataRegs.GPACLEAR.bit.GPIO2=1)
#define smg_2H			(GpioDataRegs.GPASET.bit.GPIO4=1)
#define smg_2L			(GpioDataRegs.GPACLEAR.bit.GPIO4=1)
#define smg_3H			(GpioDataRegs.GPASET.bit.GPIO6=1)
#define smg_3L			(GpioDataRegs.GPACLEAR.bit.GPIO6=1)

#define SEGA_1H			(GpioDataRegs.GPBSET.bit.GPIO50=1)
#define SEGA_1L			(GpioDataRegs.GPBCLEAR.bit.GPIO50=1)
#define SEGA_2H			(GpioDataRegs.GPBSET.bit.GPIO51=1)
#define SEGA_2L			(GpioDataRegs.GPBCLEAR.bit.GPIO51=1)
#define SEGA_3H			(GpioDataRegs.GPBSET.bit.GPIO52=1)
#define SEGA_3L			(GpioDataRegs.GPBCLEAR.bit.GPIO52=1)
#define SEGA_4H			(GpioDataRegs.GPBSET.bit.GPIO53=1)
#define SEGA_4L			(GpioDataRegs.GPBCLEAR.bit.GPIO53=1)
#define SEGA_5H			(GpioDataRegs.GPBSET.bit.GPIO54=1)
#define SEGA_5L			(GpioDataRegs.GPBCLEAR.bit.GPIO54=1)
#define SEGA_6H			(GpioDataRegs.GPBSET.bit.GPIO55=1)
#define SEGA_6L			(GpioDataRegs.GPBCLEAR.bit.GPIO55=1)
#define SEGA_7H			(GpioDataRegs.GPBSET.bit.GPIO56=1)
#define SEGA_7L			(GpioDataRegs.GPBCLEAR.bit.GPIO56=1)
#define SEGA_8H			(GpioDataRegs.GPBSET.bit.GPIO57=1)
#define SEGA_8L			(GpioDataRegs.GPBCLEAR.bit.GPIO57=1)




//#define SEGB1_SETH			(GpioDataRegs.GPASET.bit.GPIO73=1)
//#define SEGB1_SETL			(GpioDataRegs.GPACLEAR.bit.GPIO73=1)

//#define SPICLKA_SETH			(GpioDataRegs.GPBSET.bit.GPIO56=1)
//#define SPICLKA_SETL			(GpioDataRegs.GPBCLEAR.bit.GPIO56=1)

//#define SPISIMOA_SETH			(GpioDataRegs.GPBSET.bit.GPIO54=1)
//#define SPISIMOA_SETL			(GpioDataRegs.GPBCLEAR.bit.GPIO54=1)


void SMG_Init(void);
void Display(Uint16 bit,Uint16 bit2);
void SMG_display(float Voltage,float Voltage1);
//void Display(float Vout);
//void SMG_DisplayInt(Uint16 num);
//void SMG_DisplayFloat(float num,unsigned char dotnum);


#endif /* SMG_H_ */
