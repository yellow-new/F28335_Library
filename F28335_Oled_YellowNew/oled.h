/*
 * oled.h
 *
 *  Created on: 2021锟斤拷5锟斤拷5锟斤拷
 *      Author: 86153
 */

#ifndef OLED_OLED_H_
#define OLED_OLED_H_


#define OLED_CMD    0   //写锟斤拷锟斤拷
#define OLED_DATA   1   //写锟斤拷锟斤拷

void OLED_Init(void);
void OLED_WR_Byte(uchar dat,uchar mode);
void OLED_ColorTurn(uchar i);
void OLED_DisplayTurn(uchar i);
void OLED_ShowChar(uchar x,uchar y,uchar chr,uchar size1,uchar mode);
void OLED_DisplayTurn(uchar i);
//void OLED_ShowChinese(uchar x,uchar y,uchar num,uchar size1,uchar mode);
void OLED_ShowNum(uchar x,uchar y,Uint32 num,uchar len,uchar size1,uchar mode);
Uint32 oled_pow(uchar m,uchar n);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_ShowString(uchar x,uchar  y,uchar  *chr,uchar  size1,uchar mode);
void OLED_DrawPoint(uchar x,uchar y,uchar t);
void OLED_Refresh(void);
void OLED_ShowPicture(uchar x,uchar y,uchar sizex,uchar sizey,uchar BMP[],uchar mode);
void OLED_Clear(void);
void OLED_SetContrast(uchar ucValue);
//void OLED_ScrollDisplay(uchar num,uchar space,uchar mode);
void OLED_DrawLine(uchar x1,uchar y1,uchar x2,uchar y2,uchar mode);
void Boot_Animation(void);//锟斤拷锟斤拷锟斤拷锟斤拷
void OLED_Draw_Waveform(uchar x1,uchar x2,uchar y,uchar z,uchar data,uchar mode);//锟斤拷锟斤拷锟斤拷
void Gui_Fill_Rect(uchar x0,uchar y0,uchar x1,uchar y1,uchar show);
void Gui_Draw_Rect(uchar x0,uchar y0,uchar x1,uchar y1);
void OLED_Dram_XY(void);
void Draw_Circle(uchar x0,uchar y0,uchar r,uchar mode); //圆锟斤拷(x0,y0),锟诫径r
void Draw_fill_circle(uchar x0,uchar y0,uchar r,uchar mode);//写锟斤拷实锟斤拷圆锟斤拷(x0,y0),锟诫径r
void Run_xy(uchar x0,uchar x1,uchar y0,uchar num,uchar run);


#endif /* OLED_OLED_H_ */
