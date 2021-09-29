/*
 * oled.c
 *
 *  Created on: 2021锟斤拷5锟斤拷5锟斤拷
 *      Author: 锟斤拷锟斤拷
 *
 *  ********************************************************************************************
 *  @version           V1.0
 *  @date               2021-xx-xx
 *  @brief               oled锟斤拷示锟斤拷锟斤拷锟接匡拷 SCL ->33 SDA->32
 *  *******************************************************************************************
 */
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "math.h"
#include "oledfont.h"
#include "iic.h"
#include "oled.h"

uchar OLED_GRAM[128][8];//OLED锟斤拷锟皆达拷锟斤拷锟斤拷锟斤拷

/*
 * 锟斤拷锟斤拷锟斤拷锟杰ｏ拷实锟斤拷水平锟狡讹拷
 * run:0锟斤拷锟狡讹拷锟斤拷1锟斤拷锟斤拷锟狡讹拷
 * x:锟斤拷锟斤拷锟斤拷始页
 * y:锟斤拷锟斤拷锟斤拷止页
 * 一锟斤拷0~7页
 */
/*
void Run_xy(uchar x0,uchar y0,uchar x1,uchar y1,uchar run)
{
   uchar a,b,c;
   uchar data;
   data=0;
       for(b=x0;b<x1;b++)
       {
           for(a=0;a<8;a++)
              {
                 data=(data<<1)+(OLED_GRAM[b][a]&0x01);//
                 OLED_GRAM[b][a]=OLED_GRAM[b][a]>>1;
               }
           data=(data<<1)+((data&0x80)>>7);
           for(c=0;c<8;c++)
           {
               OLED_GRAM[b][c]|=data&0x80;
               data=data<<1;
           }
       }
}
*/
void Run_xy(uchar x0,uchar x1,uchar y0,uchar num,uchar run)
{
   uchar a,b,c;
   uchar data;
   data=0;
       for(b=x0;b<x1;b++)
       {
           for(a=y0;a<(num/8);a++)
              {
                 data=(data<<1)+(OLED_GRAM[b][a]&0x01);
                 OLED_GRAM[b][a]=OLED_GRAM[b][a]>>1;
               }
           data=(data<<1)+((data&0x80)>>7);
           for(c=y0;c<(num/8);c++)
           {
               OLED_GRAM[b][c]|=data&0x80;
               data=data<<1;
           }
       }
}
//锟斤拷锟斤拷OLED锟斤拷示
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);  /* 锟斤拷锟矫筹拷锟矫ｏ拷锟斤拷锟铰革拷锟斤拷锟斤拷锟斤拷使锟矫ｏ拷 */
    OLED_WR_Byte(0X14,OLED_CMD);  /* 0x14 使锟杰筹拷锟矫ｏ拷 0x10 锟角关憋拷 */
    OLED_WR_Byte(0XAF,OLED_CMD);   /* 锟斤拷OLED锟斤拷锟� */
}
//锟截憋拷OLED锟斤拷示
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);   /* 锟斤拷锟矫筹拷锟矫ｏ拷锟斤拷锟铰革拷锟斤拷锟斤拷锟斤拷使锟矫ｏ拷*/
    OLED_WR_Byte(0X10,OLED_CMD);  /* 0x14 使锟杰筹拷锟矫ｏ拷0x10 锟角关憋拷 */
    OLED_WR_Byte(0XAE,OLED_CMD);   /* 锟截憋拷OLED锟斤拷锟� */
}

//m^n锟斤拷锟斤拷
Uint32 oled_pow(uchar m,uchar n)
{
    Uint32 result=1;
    while(n--)result*=m;
    return result;
}
/*
 *锟斤拷示锟斤拷锟斤拷
 *x,y :锟斤拷锟斤拷锟斤拷锟�
 *num:要锟斤拷示锟斤拷锟斤拷锟斤拷
 *len :锟斤拷锟街碉拷位锟斤拷
 *sizey:锟斤拷锟斤拷拇锟叫�
 *mode:0锟斤拷锟斤拷锟斤拷示锟斤拷1锟斤拷锟斤拷锟斤拷示
 */
void OLED_ShowNum(uchar x,uchar y,Uint32 num,uchar len,uchar size1,uchar mode)
{
    uchar t,temp,m=0;
    if(size1==8)m=2;
    for(t=0;t<len;t++)
    {
        temp=(num/oled_pow(10,len-t-1))%10;
            if(temp==0)
            {
                OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
      }
            else
            {
              OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
            }
  }
}

/*
 * 锟斤拷指锟斤拷锟斤拷位锟斤拷锟斤拷示一锟斤拷锟街凤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟街凤拷
 * x:0~127
 * y:0~63
 * chr锟斤拷要锟斤拷示锟斤拷锟街凤拷
 * sizey:选锟斤拷锟斤拷锟斤拷 6锟斤拷8 6锟斤拷12 8锟斤拷16  12锟斤拷24
 * mode:0锟斤拷锟斤拷锟斤拷示锟斤拷1锟斤拷锟斤拷锟斤拷示
 */
void OLED_ShowChar(uchar x,uchar y,uchar chr,uchar size1,uchar mode)
{
    uchar i,m,temp,size2,chr1;
    uchar x0=x,y0=y;
    if(size1==8)size2=6;
    else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //锟矫碉拷一锟斤拷锟街凤拷锟斤拷应锟斤拷锟斤拷锟斤拷占锟矫碉拷锟街斤拷锟斤拷
    chr1=chr-' ';  //锟斤拷锟斤拷偏锟狡猴拷锟街�
    for(i=0;i<size2;i++)
    {
        if(size1==8)
              {temp=asc2_0806[chr1][i];} //锟斤拷锟斤拷0806锟斤拷锟斤拷
        else if(size1==12)
        {temp=asc2_1206[chr1][i];} //锟斤拷锟斤拷1206锟斤拷锟斤拷
        else if(size1==16)
        {temp=asc2_1608[chr1][i];} //锟斤拷锟斤拷1608锟斤拷锟斤拷
        else if(size1==24)
        {temp=asc2_2412[chr1][i];} //锟斤拷锟斤拷2412锟斤拷锟斤拷
        else return;
        for(m=0;m<8;m++)
        {
            if(temp&0x01)OLED_DrawPoint(x,y,mode);
            else OLED_DrawPoint(x,y,!mode);
            temp>>=1;
            y++;
        }
        x++;
        if((size1!=8)&&((x-x0)==size1/2))
        {x=x0;y0=y0+8;}
        y=y0;
  }
}
/**
*  锟斤拷锟杰ｏ拷锟斤拷幕锟斤拷锟斤拷
* x0:锟斤拷锟较斤拷x锟斤拷锟斤拷
* y0:锟斤拷锟较斤拷y锟斤拷锟斤拷
* x1:锟斤拷锟较斤拷x锟斤拷锟斤拷
* y2:锟斤拷锟较斤拷y锟斤拷锟斤拷
* (x0,y0),(x1,y1)锟借定锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷,锟斤拷锟斤拷x1<x2,y1<y2;x2=0~127,y=0~7;
* direction锟斤拷0水平锟斤拷锟斤拷锟斤拷1锟斤拷直锟斤拷锟斤拷
*/
/*
void OLED_Run(uchar x0,uchar y0,uchar x1,uchar y1,uchar direction)
{
    uchar i,j;
    uchar data=0;//锟叫硷拷娲拷锟斤拷锟�
    if(direction)
    {
        OLED_GRAM[i][j]
        for(j=y0;j<(y1+1);j++)
        {
            for(i=x0;i<(x1+1);i++)
            {
                   OLED_GRAM[i][j]=OLED_GRAM[i][j];
            }
        }
    }

}
*/
/*
 * 锟斤拷示锟街凤拷锟斤拷
 * x,y:锟斤拷锟斤拷锟斤拷锟�
 * size1:锟斤拷锟斤拷锟叫�
 * *chr:锟街凤拷锟斤拷锟斤拷始锟斤拷址
 * mode:0锟斤拷锟斤拷锟斤拷示锟斤拷1锟斤拷锟斤拷锟斤拷示
 */
void OLED_ShowString(uchar x,uchar  y,uchar  *chr,uchar  size1,uchar mode)
{
    while((*chr>=' ')&&(*chr<='~'))//锟叫讹拷锟角诧拷锟角非凤拷锟街凤拷
        {
            OLED_ShowChar(x,y,*chr,size1,mode);
            if(size1==8)x+=6;
            else x+=size1/2;
            chr++;
      }
}
/*
 * 锟斤拷示锟斤拷锟斤拷
 * x,y锟斤拷锟斤拷锟斤拷锟斤拷锟�
 * num锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟叫讹拷应锟侥憋拷锟�
 * size1锟斤拷锟斤拷锟街讹拷应锟斤拷锟斤拷锟斤拷锟叫�
 * mode:0锟斤拷锟斤拷锟斤拷示锟斤拷1锟斤拷锟斤拷锟斤拷示
 */
//void OLED_ShowChinese(uchar x,uchar y,uchar num,uchar size1,uchar mode)
//{
//    uchar m,temp;
//    uchar x0=x,y0=y;
//    Uint16 i,size3=(size1/8+((size1%8)?1:0))*size1;  //锟矫碉拷一锟斤拷锟斤拷锟斤拷锟节碉拷锟斤拷锟斤拷锟斤拷占锟矫碉拷锟街斤拷锟斤拷
//    for(i=0;i<size3;i++)
//    {
//        if(size1==16)
//                {temp=Hzk1[num][i];}//锟斤拷锟斤拷16*16锟斤拷锟斤拷
//        else if(size1==24)
//                {temp=Hzk2[num][i];}//锟斤拷锟斤拷24*24锟斤拷锟斤拷
//        else if(size1==32)
//                {temp=Hzk3[num][i];}//锟斤拷锟斤拷32*32锟斤拷锟斤拷
//        else if(size1==64)
//                {temp=Hzk4[num][i];}//锟斤拷锟斤拷64*64锟斤拷锟斤拷
//        else return;
//        for(m=0;m<8;m++)
//        {
//            if(temp&0x01)OLED_DrawPoint(x,y,mode);
//            else OLED_DrawPoint(x,y,!mode);
//            temp>>=1;//锟斤拷锟斤拷锟狡讹拷1位然锟斤拷锟节革拷值锟斤拷temp
//            y++;//指锟斤拷锟斤拷一位
//        }
//        x++;
//        if((x-x0)==size1)
//        {x=x0;y0=y0+8;}
//        y=y0;
//    }
//}
/*num 锟斤拷示锟斤拷锟街的革拷锟斤拷
 *space 每一锟斤拷锟斤拷示锟侥硷拷锟�
 * mode:0锟斤拷锟斤拷锟斤拷示锟斤拷1锟斤拷锟斤拷锟斤拷示
 */
//void OLED_ScrollDisplay(uchar num,uchar space,uchar mode)
//{
//    uchar i,n,t=0,m=0,r;
//    while(1)
//    {
//        if(m==0)
//        {
//        OLED_ShowChinese(128,24,t,16,mode); //写锟斤拷一锟斤拷锟斤拷锟街憋拷锟芥到OLED_GRAM[]锟斤拷
//            t++;
//        }
//        if(t==num)
//            {
//                for(r=0;r<16*space;r++)     //锟斤拷示锟斤拷锟�
//                 {
//                    for(i=1;i<128;i++)
//                        {
//                            for(n=0;n<8;n++)
//                            {
//                                OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
//                            }
//                        }
//           OLED_Refresh();
//                 }
//        t=0;
//      }
//        m++;
//        if(m==16){m=0;}
//        for(i=1;i<128;i++)   //实锟斤拷锟斤拷锟斤拷
//        {
//            for(n=0;n<8;n++)
//            {
//                OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
//            }
//        }
//        OLED_Refresh();
//    }
//}
/*
 *锟斤拷锟皆猴拷锟斤拷
 *i:0 锟斤拷锟斤拷锟斤拷示锟斤拷1锟斤拷色锟斤拷示
 */
void OLED_ColorTurn(uchar i)
{
    if(i==0)
        {
            OLED_WR_Byte(0xA6,OLED_CMD);
        }
    if(i==1)
        {
            OLED_WR_Byte(0xA7,OLED_CMD);
        }
}
/*
*********************************************************************************************************
*        锟斤拷 锟斤拷 锟斤拷: OLED_SetContrast
*        锟斤拷锟斤拷说锟斤拷: 锟斤拷锟矫对比讹拷
*        锟斤拷    锟斤拷:  锟斤拷
*        锟斤拷 锟斤拷 值: 锟斤拷
*********************************************************************************************************
*/
void OLED_SetContrast(uchar ucValue)
{
    OLED_WR_Byte(0x81,OLED_CMD);        /* 锟斤拷锟矫对比讹拷锟斤拷锟斤拷(双锟街斤拷锟斤拷锟筋）锟斤拷锟斤拷1锟斤拷锟街斤拷锟斤拷锟斤拷锟筋，锟斤拷2锟斤拷锟街斤拷锟角对比度诧拷锟斤拷0-255 */
    OLED_WR_Byte(ucValue,OLED_CMD);     /* 锟斤拷锟矫对比度诧拷锟斤拷,缺省CF */
}

/*
 * 锟斤拷幕锟斤拷转180锟斤拷
 * i:0锟斤拷锟斤拷锟斤拷示锟斤拷1锟斤拷转锟斤拷示
 */
void OLED_DisplayTurn(uchar i)
{
    if(i==0)
        {
            OLED_WR_Byte(0xC8,OLED_CMD); /* C0 锟斤拷锟斤拷锟斤拷扫锟斤拷,锟斤拷COM0锟斤拷COM63;  C8 : 锟斤拷锟斤拷扫锟斤拷, 锟斤拷 COM63锟斤拷 COM0 */
            OLED_WR_Byte(0xA1,OLED_CMD); /* A0 锟斤拷锟叫碉拷址0映锟戒到SEG0; A1 锟斤拷锟叫碉拷址127映锟戒到SEG0 */
        }
    if(i==1)
        {
            OLED_WR_Byte(0xC0,OLED_CMD); /* C0 锟斤拷锟斤拷锟斤拷扫锟斤拷,锟斤拷COM0锟斤拷COM63;  C8 : 锟斤拷锟斤拷扫锟斤拷, 锟斤拷 COM63锟斤拷 COM0 */
            OLED_WR_Byte(0xA0,OLED_CMD);/* A0 锟斤拷锟叫碉拷址0映锟戒到SEG0; A1 锟斤拷锟叫碉拷址127映锟戒到SEG0 */
        }
}

void OLED_Init(void)
{

    OLED_WR_Byte(0xAE,OLED_CMD);/*锟截憋拷OLED锟斤拷锟斤拷锟绞撅拷锟斤拷锟斤拷撸锟�*/
    OLED_WR_Byte(0x00,OLED_CMD);/*锟斤拷锟斤拷锟叫碉拷址锟斤拷4bit*/
    OLED_WR_Byte(0x10,OLED_CMD);/*锟斤拷锟斤拷锟叫碉拷址锟斤拷4bit*/
    OLED_WR_Byte(0x40,OLED_CMD);/*锟斤拷锟斤拷锟斤拷始锟叫碉拷址锟斤拷锟斤拷5bit 0-63锟斤拷锟斤拷 硬锟斤拷锟斤拷锟�*/
    OLED_WR_Byte(0x81,OLED_CMD);/* 锟斤拷锟矫对比讹拷锟斤拷锟斤拷(双锟街斤拷锟斤拷锟筋）锟斤拷锟斤拷1锟斤拷锟街斤拷锟斤拷锟斤拷锟筋，锟斤拷2锟斤拷锟街斤拷锟角对比度诧拷锟斤拷0-255 */
    OLED_WR_Byte(0xCF,OLED_CMD);/* 锟斤拷锟矫对比度诧拷锟斤拷,缺省CF */
    OLED_WR_Byte(0xA1,OLED_CMD);/* A0 锟斤拷锟叫碉拷址0映锟戒到SEG0; A1 锟斤拷锟叫碉拷址127映锟戒到SEG0 */
    OLED_WR_Byte(0xC8,OLED_CMD);/* C0 锟斤拷锟斤拷锟斤拷扫锟斤拷,锟斤拷COM0锟斤拷COM63;  C8 : 锟斤拷锟斤拷扫锟斤拷, 锟斤拷 COM63锟斤拷 COM0 */
    OLED_WR_Byte(0xA6,OLED_CMD); /* A6 : 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷示模式; A7 : 锟斤拷锟斤拷为锟斤拷锟斤拷模式 */
    OLED_WR_Byte(0xA8,OLED_CMD); /* 锟斤拷锟斤拷COM路锟斤拷 */
    OLED_WR_Byte(0x3f,OLED_CMD); /* 1 ->锟斤拷63+1锟斤拷路 */
    OLED_WR_Byte(0xD3,OLED_CMD); /* 锟斤拷锟斤拷锟斤拷示偏锟狡ｏ拷双锟街斤拷锟斤拷锟筋）*/
    OLED_WR_Byte(0x00,OLED_CMD); /* 锟斤拷偏锟斤拷 */
    OLED_WR_Byte(0xd5,OLED_CMD);/* 锟斤拷锟斤拷锟斤拷示时锟接凤拷频系锟斤拷/锟斤拷频锟斤拷 */
    OLED_WR_Byte(0x80,OLED_CMD);/* 锟斤拷锟矫凤拷频系锟斤拷,锟斤拷4bit锟角凤拷频系锟斤拷锟斤拷锟斤拷4bit锟斤拷锟斤拷频锟斤拷 */
    OLED_WR_Byte(0xD9,OLED_CMD); /* 锟斤拷锟斤拷预锟斤拷锟斤拷锟斤拷锟� */
    OLED_WR_Byte(0xF1,OLED_CMD); /* [3:0],PHASE 1; [7:4],PHASE 2; */
    OLED_WR_Byte(0xDA,OLED_CMD); /* 锟斤拷锟斤拷COM锟斤拷硬锟斤拷锟斤拷锟竭凤拷式 */
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD); /* 锟斤拷锟斤拷 vcomh 锟斤拷压锟斤拷锟斤拷 */
    OLED_WR_Byte(0x40,OLED_CMD); /* [6:4] 000 = 0.65 x VCC; 0.77 x VCC (RESET); 0.83 x VCC  */
    OLED_WR_Byte(0x20,OLED_CMD); /*锟斤拷锟斤拷锟斤拷寻址模式锟斤拷锟斤拷锟斤拷0x20锟斤拷锟斤拷锟斤拷锟�0x02为确锟斤拷页寻址模式*/
    OLED_WR_Byte(0x02,OLED_CMD);
    OLED_WR_Byte(0x8D,OLED_CMD);/* 锟斤拷锟矫筹拷锟矫ｏ拷锟斤拷锟铰革拷锟斤拷锟斤拷锟斤拷使锟矫ｏ拷 */
    OLED_WR_Byte(0x14,OLED_CMD);/* 0x14 使锟杰筹拷锟矫ｏ拷 0x10 锟角关憋拷 */
    OLED_WR_Byte(0xA4,OLED_CMD);
    OLED_WR_Byte(0xA6,OLED_CMD);
    OLED_WR_Byte(0x2E,OLED_CMD);/*锟斤拷锟矫癸拷锟斤拷*/
    OLED_Clear();//锟斤拷锟斤拷
    OLED_WR_Byte(0xAF,OLED_CMD); /* 锟斤拷OLED锟斤拷锟� */
}
/*
 * 锟斤拷锟斤拷一锟斤拷锟街斤拷
 * mode锟斤拷锟斤拷锟斤拷/锟斤拷锟斤拷 0为锟斤拷锟捷ｏ拷1锟斤拷示锟斤拷锟斤拷
 */
void OLED_WR_Byte(uchar dat,uchar mode)
{
    IIC_Start();
    IIC_Send_Byte(0x78);//锟斤拷锟酵从伙拷锟斤拷址
    IIC_Wait_Ack();
    if(mode){IIC_Send_Byte(0x40);}//0x40锟斤拷写锟斤拷锟斤拷 Co=0 D/C=1
  else{IIC_Send_Byte(0x00);}//0x00写锟斤拷锟斤拷Co=0 D/C=0
    IIC_Wait_Ack();
    IIC_Send_Byte(dat);
    IIC_Wait_Ack();
    IIC_Stop();
}

/*
 * 锟斤拷锟斤拷锟斤拷锟斤拷源锟�
 */
void OLED_Clear(void)
{
    uchar i,n;
        for(i=0;i<8;i++)
        {
           for(n=0;n<128;n++)
                {
                 OLED_GRAM[n][i]=0;//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟�
                }
      }
        OLED_Refresh();//锟斤拷锟斤拷锟斤拷示
}
//锟斤拷锟斤拷
//x:0~127
//y:0~63
//t:1 锟斤拷锟斤拷锟斤拷示 0,锟斤拷锟斤拷锟斤拷什么锟斤拷锟捷讹拷锟斤拷锟斤拷锟斤拷
void OLED_DrawPoint(uchar x,uchar y,uchar t)
{
    uchar i,m,n;
    i=y/8;//确锟斤拷锟斤拷锟斤拷一页
    m=y%8;//确锟斤拷锟斤拷锟斤拷一页锟斤拷锟斤拷一位
    n=0x01<<m;//确锟斤拷锟斤拷锟斤拷一页锟斤拷示锟斤拷1锟斤拷锟斤拷偏锟斤拷m位锟斤拷锟斤拷
    if(x>127||y>63)return;//锟斤拷锟斤拷锟斤拷围锟斤拷
    if(t){OLED_GRAM[x][i]|=n;}
    else
    {
        OLED_GRAM[x][i]=~OLED_GRAM[x][i];//锟斤拷i页锟侥碉拷x锟叫的帮拷位锟斤拷锟斤拷去锟斤拷也锟斤拷锟斤拷0  1锟斤拷锟斤拷
        OLED_GRAM[x][i]|=n;//取锟斤拷锟襟将达拷位锟矫碉拷锟斤拷锟斤拷锟斤拷1
        OLED_GRAM[x][i]=~OLED_GRAM[x][i];//锟节斤拷锟斤拷去锟斤拷
        /*锟斤拷锟斤拷锟斤拷锟斤拷写锟斤拷锟斤拷锟斤拷镁锟斤拷锟斤拷锟斤拷锟斤拷锟绞裁达拷锟斤拷荻锟斤拷锟斤拷锟斤拷锟�*/
    }
}
/**
* @brief 锟斤拷锟斤拷锟斤拷
* @param x0:锟斤拷锟较斤拷x锟斤拷锟斤拷
* @param y0:锟斤拷锟较斤拷y锟斤拷锟斤拷
* @param x1:锟斤拷锟较斤拷x锟斤拷锟斤拷
* @param y2:锟斤拷锟较斤拷y锟斤拷锟斤拷
* @param show:0:锟斤拷锟斤拷示,1:锟斤拷示
*/
void Gui_Fill_Rect(uchar x0,uchar y0,uchar x1,uchar y1,uchar show)
{
    uchar i = 0;
    uchar j = 0;

    for (i = x0;i <= x1;i++)
    {
        for (j = y0;j <= y1;j++)
        {
            OLED_DrawPoint(i,j,show);
        }
    }
}
/**
* @brief 锟斤拷锟狡撅拷锟斤拷
* @param x0:锟斤拷锟较斤拷x锟斤拷锟斤拷
* @param y0:锟斤拷锟较斤拷y锟斤拷锟斤拷
* @param x1:锟斤拷锟较斤拷x锟斤拷锟斤拷
* @param y2:锟斤拷锟较斤拷y锟斤拷锟斤拷
*/
void Gui_Draw_Rect(uchar x0,uchar y0,uchar x1,uchar y1)
{
    OLED_DrawLine(x0,y0,x1,y0,1);
    OLED_DrawLine(x0,y0,x0,y1,1);
    OLED_DrawLine(x0,y1,x1,y1,1);
    OLED_DrawLine(x1,y0,x1,y1,1);
}

/*
 * 锟斤拷锟斤拷锟轿猴拷锟斤拷
 * x1锟斤拷x2锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷示锟侥匡拷龋锟斤拷锟斤拷锟絰1<x2
 *data要锟斤拷示锟斤拷锟斤拷锟斤拷
 *y取值锟斤拷围为0~7锟斤拷也锟斤拷锟斤拷y锟斤拷锟斤拷确锟斤拷要锟斤拷示锟斤拷锟斤拷使页
 *z锟斤拷锟斤拷锟斤拷示为位锟斤拷锟斤拷8位锟斤拷16位锟斤拷24位锟斤拷32位锟斤拷40位锟斤拷48位锟斤拷56位锟斤拷64位
 *data取值锟斤拷围为取锟斤拷锟斤拷锟斤拷示锟斤拷位锟斤拷
 *mode:0锟斤拷锟斤拷锟斤拷示锟斤拷1锟斤拷转锟斤拷示
 */
void OLED_Draw_Waveform(uchar x1,uchar x2,uchar y,uchar z,uchar data,uchar mode)
{
        uchar a,b,c;
        uchar l,m,n;
        if(mode){
                   l=7-(data/8+y);
                   m=7-data%8;
                }
        else {
                   l=data/8+y;//确锟斤拷锟斤拷锟斤拷一页
                   m=data%8;//确锟斤拷锟斤拷锟斤拷一页锟斤拷锟斤拷一位
             }
        n=1<<m;//确锟斤拷锟斤拷锟斤拷一页锟斤拷示锟斤拷1锟斤拷锟斤拷偏锟斤拷m位锟斤拷锟斤拷
        for(a=x1;a<x2;a++)//锟斤拷锟斤拷锟斤拷x锟斤拷姆锟斤拷锟斤拷锟绞撅拷姆锟轿�
        {
           for(c=y;c<(z/8+y);c++)
              {
                 OLED_GRAM[a][c]=OLED_GRAM[a+1][c];//锟斤拷锟斤拷一锟斤拷锟斤拷元
              }
        }
        for(b=y;b<(z/8+y);b++)
             {
               OLED_GRAM[x2][b]=0;//锟斤拷每页锟斤拷锟斤拷锟揭伙拷锟斤拷锟斤拷锟�
             }
        OLED_GRAM[x2][l]=n;
        //OLED_Refresh();//锟斤拷锟斤拷锟斤拷示锟斤拷锟斤拷

}
/*
 * 锟斤拷锟斤拷实锟斤拷锟斤拷锟斤拷锟斤拷械锟斤拷锟�
 */
void OLED_Dram_XY(void)
{
    OLED_DrawLine(60,4,63,0,1);
    OLED_DrawLine(66,4,63,0,1);
    OLED_DrawLine(63,56,64,0,1);
    OLED_DrawLine(63,56,127,56,1);
    OLED_DrawLine(123,53,127,56,1);
    OLED_DrawLine(123,59,127,56,1);
    OLED_ShowNum(60,57,0,1,8,1);
    OLED_ShowString(55,5,"y",8,1);
    OLED_ShowString(115,56,"x",8,1);
}
/*锟斤拷锟斤拷
 *x1,y1:锟斤拷锟斤拷锟斤拷锟�
 *x2,y2:锟斤拷锟斤拷锟斤拷锟斤拷
 *mode:0锟斤拷色锟斤拷示锟斤拷1锟斤拷锟斤拷锟斤拷锟斤拷示
 */

void OLED_DrawLine(uchar x1,uchar y1,uchar x2,uchar y2,uchar mode)
{
    Uint16 t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    delta_x=x2-x1; //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
    delta_y=y2-y1;
    uRow=x1;//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟�
    uCol=y1;

    if(delta_x>0)incx=1; //锟斤拷锟矫碉拷锟斤拷锟斤拷锟斤拷
    else if (delta_x==0)incx=0;//锟斤拷直锟斤拷
    else {incx=-1;delta_x=-delta_x;}

    if(delta_y>0)incy=1;
    else if (delta_y==0)incy=0;//水平锟斤拷
    else {incy=-1;delta_y=-delta_y;}

    if(delta_x>delta_y)distance=delta_x; //选取锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
    else distance=delta_y;

    for(t=0;t<distance+1;t++)
    {

        OLED_DrawPoint(uRow,uCol,mode);//锟斤拷锟斤拷
        xerr+=delta_x;
        yerr+=delta_y;
        if(xerr>distance)
        {
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance)
        {
            yerr-=distance;
            uCol+=incy;
        }
    }
}
/* 锟斤拷锟斤拷锟斤拷锟斤拷 */
void Boot_Animation(void)
{
        static uchar x=0,y=0;
        for(x = 63;x>=18;x--){
                OLED_DrawPoint(108-0.7*x,x,1);//锟斤拷斜锟斤拷 斜锟绞≈★拷3/3
                OLED_DrawPoint(17 +0.7*x,x,1);
                y = 64-x;
                OLED_DrawPoint(64-0.7*y,y,1);
                OLED_DrawPoint(64+0.7*y,y,1);
                DELAY_US(2);
                OLED_Refresh();//锟斤拷锟斤拷锟斤拷示锟斤拷OLED
        }

        for(x = 30;x <= 94;x++){
                OLED_DrawPoint(125-x,47,1);
                OLED_DrawPoint(x,18,1);
                DELAY_US(2);
                OLED_Refresh();//锟斤拷锟斤拷锟斤拷示锟斤拷OLED
        }

        OLED_ShowString(60,20,"A",16,1);
        OLED_Refresh();//锟斤拷锟斤拷锟斤拷示锟斤拷OLED
        DELAY_US(1000);

}

//锟斤拷锟斤拷锟斤拷示锟斤拷OLED
void OLED_Refresh(void)
{
    uchar i,n;
    for(i=0;i<8;i++)
    {
        OLED_WR_Byte(0xb0+i,OLED_CMD);//锟斤拷锟斤拷页锟斤拷址锟斤拷0~7锟斤拷
        OLED_WR_Byte(0x00,OLED_CMD);/*锟斤拷锟斤拷锟叫碉拷址锟斤拷4bit*/
        OLED_WR_Byte(0x10,OLED_CMD);/*锟斤拷锟斤拷锟叫碉拷址锟斤拷4bit*/
        IIC_Start();
        IIC_Send_Byte(0x78);//锟接伙拷锟斤拷址
        IIC_Wait_Ack();
        IIC_Send_Byte(0x40);//写锟斤拷锟斤拷
        IIC_Wait_Ack();
        for(n=0;n<128;n++)
        {
            IIC_Send_Byte(OLED_GRAM[n][i]);
            IIC_Wait_Ack();
        }
        IIC_Stop();
  }
}
/*锟斤拷锟斤拷锟斤拷圆
 *x,y:圆锟斤拷锟斤拷锟斤拷
 *r:圆锟侥半径
 *mode:0锟斤拷色锟斤拷示锟斤拷1锟斤拷锟斤拷锟斤拷锟斤拷示
 */
void Draw_Circle(uchar x0,uchar y0,uchar r,uchar mode)
{
    uchar y;
    float x;
        for(x = (x0-r);x <= (x0+r);x+=0.01)
        {
                y=sqrt(pow(r,2)-pow(x-x0,2))+y0; //(x-x0)^2+(y-y0)^2=r^2
                OLED_DrawPoint(x,y,mode);      //锟较帮拷圆
                OLED_DrawPoint(x,2*y0-y,mode);   //锟铰帮拷圆
        }
}
void Draw_fill_circle(uchar x0,uchar y0,uchar r,uchar mode)//写锟斤拷实锟斤拷圆锟斤拷(x0,y0),锟诫径r
{
        uchar y;
        float x;
            for(x = (x0-r);x <= (x0+r);x+=0.001)
            {
                    y=sqrt(pow(r,2)-pow(x-x0,2))+y0; //(x-x0)^2+(y-y0)^2=r^2
                    OLED_DrawLine(x,2*y0-y,x,y, mode);
            }
}

/*
 * x,y锟斤拷锟斤拷锟斤拷锟斤拷锟�
 * sizex,sizey,图片锟侥筹拷锟酵匡拷
 * 要写锟斤拷锟酵计拷锟斤拷锟�
 * mode:0锟斤拷色锟斤拷示锟斤拷1锟斤拷锟斤拷锟斤拷锟斤拷示
 */

void OLED_ShowPicture(uchar x,uchar y,uchar sizex,uchar sizey,uchar BMP[],uchar mode)
{
    Uint16 j=0;
    uchar i,n,temp,m;
    uchar x0=x,y0=y;
    sizey=sizey/8+((sizey%8)?1:0);
    for(n=0;n<sizey;n++)
    {
         for(i=0;i<sizex;i++)
         {
                temp=BMP[j];
                j++;
                for(m=0;m<8;m++)
                {
                    if(temp&0x01)OLED_DrawPoint(x,y,mode);
                    else OLED_DrawPoint(x,y,!mode);
                    temp>>=1;
                    y++;
                }
                x++;
                if((x-x0)==sizex)
                {
                    x=x0;
                    y0=y0+8;
                }
                y=y0;
         }
     }
}
