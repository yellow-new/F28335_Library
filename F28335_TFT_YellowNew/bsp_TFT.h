/**
 *  ********************************************************************************************
 *  @file                 bsp_TFT.h
 *  @file                 SK Electronics
 *  @version           V1.0
 *  @date               2021-xx-xx
 *  @brief               TFT函数接口头文件
 *  *******************************************************************************************
 *  @attention
 *  实验平台：F28335   核心板
 * 个人博客：http://yellownew.cn 
 */

#ifndef _BSP_TFT_H_
#define _BSP_TFT_H_
#include "DSP28x_Project.h"


#define USE_HORIZONTAL 0  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define TFT_W 240
#define TFT_H 320

#else
#define TFT_W 320
#define TFT_H 240
#endif

#define Uint8                unsigned char
#define TFT_RES_SET()        GpioDataRegs. GPASET.bit.GPIO20=1
#define TFT_RES_CLR()        GpioDataRegs. GPACLEAR.bit.GPIO20=1

#define TFT_DC_SET()        GpioDataRegs. GPASET.bit.GPIO21=1
#define TFT_DC_CLR()        GpioDataRegs. GPACLEAR.bit.GPIO21=1

#define TFT_BLK_SET()        GpioDataRegs. GPASET.bit.GPIO22=1
#define TFT_BLK_CLR()        GpioDataRegs. GPACLEAR.bit.GPIO22=1

#define TFT_CS_SET()        GpioDataRegs. GPASET.bit.GPIO23=1
#define TFT_CS_CLR()        GpioDataRegs. GPACLEAR.bit.GPIO23=1

#define TFT_SDA_SET()        GpioDataRegs. GPASET.bit.GPIO16=1
#define TFT_SDA_CLR()        GpioDataRegs. GPACLEAR.bit.GPIO16=1

#define TFT_SCK_SET()        GpioDataRegs. GPASET.bit.GPIO18=1
#define TFT_SCK_CLR()        GpioDataRegs. GPACLEAR.bit.GPIO18=1

void GPIO_TFT(void);
void TFT_WR_DATA8(Uint8 dat);
void TFT_WR_DATA16(Uint16 dat);
void TFT_WR_REG(Uint8 dat);
void TFT_Address_SET(Uint16 x1, Uint16 y1, Uint16 x2, Uint16 y2);
void Init_TFT(void);
void TFT_Fill(Uint16 xsta,Uint16 ysta,Uint16 xend,Uint16 yend,Uint16 color);
void TFT_DrawPoint(Uint16 x,Uint16 y,Uint16 color);
void TFT_DrawLine(Uint16 x1,Uint16 y1,Uint16 x2,Uint16 y2,Uint16 color);
void TFT_DrawRectangle(Uint16 x1,Uint16 y1,Uint16 x2,Uint16 y2,Uint16 color);
void Draw_Circle(Uint16 x0,Uint16 y0,Uint8 r,Uint16 color);
void TFT_ShowChinese(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode);
void TFT_ShowChinese12x12(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode);
void TFT_ShowChinese16x16(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode);
void TFT_ShowChinese24x24(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode);
void TFT_ShowChinese32x32(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode);
void TFT_SPIChinese(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode);
void TFT_ShowChineseSPI(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode);
void TFT_SPIChinese(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode);
void TFT_ShowChar(Uint16 x,Uint16 y,Uint8 num,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode);
void TFT_ShowString(Uint16 x,Uint16 y,const Uint8 *p,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode);
Uint32 mypow(Uint8 m,Uint8 n);
void TFT_ShowIntNum(Uint16 x,Uint16 y,Uint16 num,Uint8 len,Uint16 fc,Uint16 bc,Uint8 sizey);
void TFT_ShowFloatNum1(Uint16 x,Uint16 y,float num,Uint8 len,Uint16 fc,Uint16 bc,Uint8 sizey);
void TFT_ShowPicture(Uint16 x,Uint16 y,Uint16 length,Uint16 width,const Uint8 pic[]);
void TFT_Writ_Bus(Uint8 dat);
//画笔颜色
#define WHITE            0xFFFF
#define BLACK            0x0000
#define BLUE             0x001F
#define BRED             0XF81F
#define GRED             0XFFE0
#define GBLUE            0X07FF
#define RED              0xF800
#define MAGENTA          0xF81F
#define GREEN            0x07E0
#define CYAN             0x7FFF
#define YELLOW           0xFFE0
#define BROWN            0XBC40 //棕色
#define BRRED            0XFC07 //棕红色
#define GRAY             0X8430 //灰色
#define DARKBLUE         0X01CF //深蓝色
#define LIGHTBLUE        0X7D7C //浅蓝色
#define GRAYBLUE         0X5458 //灰蓝色
#define LIGHTGREEN       0X841F //浅绿色
#define LGRAY            0XC618 //浅灰色(PANNEL)，窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
#endif /*_BSP_TFT_H_ */
