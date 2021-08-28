/**
 *  ********************************************************************************************
 *  @file                 bsp_spi.h
 *  @file                 SK Electronics
 *  @version           V1.0
 *  @date               2020-xx-xx
 *  @brief               SPI通信函数接口头文件
 *  *******************************************************************************************
 *  @attention
 *  实验平台：F28335   核心板
 * 个人博客：http://yellownew.cn 
 */

#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_
#include "DSP28x_Project.h"
void Init_Spi(void);
void Spi_xmit(Uint16 a);
#endif /*_BSP_SPI_H_ */
