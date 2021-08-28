/**
 *  ********************************************************************************************
 *  @file                 bsp_spi.c
 *  @file                 SK Electronics
 *  @version           V1.0
 *  @date               2020-xx-xx
 *  @brief               SPI通信应用函数接口
 *  *******************************************************************************************
 *  @attention
 *  实验平台：F28335   核心板
 * 个人博客：http://yellownew.cn 
 */
#include "bsp_spi.h"
/**
 *  @brief                   初始化SPI配置
 *  @parameter                  无
 *  @return_value               无
 */
extern Uint16 rdata;

void Init_Spi(void)
{
    InitSpiaGpio();
    EALLOW;

    SpiaRegs.SPICCR.bit.SPISWRESET = 0;
    SpiaRegs.SPICCR.all = 0x0057;
    SpiaRegs.SPICTL.all = 0x0016;
    SpiaRegs.SPISTS.all=0;
    SpiaRegs.SPIBRR = 0x0003;
    SpiaRegs.SPICCR.bit.SPISWRESET = 1;

    EDIS;
    IER |= M_INT6;
    PieCtrlRegs.PIEIER6.bit.INTx1 = 1;
}

void Spi_xmit(Uint16 a)
{
    while(SpiaRegs.SPISTS.bit.BUFFULL_FLAG);
    SpiaRegs.SPITXBUF = a;
    while(!SpiaRegs.SPISTS.bit.INT_FLAG);
    rdata = SpiaRegs.SPIRXBUF;
}
