/*
 * oled.c
 *
 *  Created on: 2021��5��5��
 *      Author: ����
 *
 *  ********************************************************************************************
 *  @version           V1.0
 *  @date               2021-xx-xx
 *  @brief               oled��ʾ�����ӿ� SCL ->33 SDA->32
 *  *******************************************************************************************
 */
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "math.h"
#include "oledfont.h"
#include "iic.h"
#include "oled.h"

uchar OLED_GRAM[128][8];//OLED���Դ�������

/*
 * �������ܣ�ʵ��ˮƽ�ƶ�
 * run:0���ƶ���1�����ƶ�
 * x:������ʼҳ
 * y:������ֹҳ
 * һ��0~7ҳ
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
//����OLED��ʾ
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);  /* ���ó��ã����¸�������ʹ�ã� */
    OLED_WR_Byte(0X14,OLED_CMD);  /* 0x14 ʹ�ܳ��ã� 0x10 �ǹر� */
    OLED_WR_Byte(0XAF,OLED_CMD);   /* ��OLED��� */
}
//�ر�OLED��ʾ
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);   /* ���ó��ã����¸�������ʹ�ã�*/
    OLED_WR_Byte(0X10,OLED_CMD);  /* 0x14 ʹ�ܳ��ã�0x10 �ǹر� */
    OLED_WR_Byte(0XAE,OLED_CMD);   /* �ر�OLED��� */
}

//m^n����
Uint32 oled_pow(uchar m,uchar n)
{
    Uint32 result=1;
    while(n--)result*=m;
    return result;
}
/*
 *��ʾ����
 *x,y :�������
 *num:Ҫ��ʾ������
 *len :���ֵ�λ��
 *sizey:����Ĵ�С
 *mode:0������ʾ��1������ʾ
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
 * ��ָ����λ����ʾһ���ַ������������ַ�
 * x:0~127
 * y:0~63
 * chr��Ҫ��ʾ���ַ�
 * sizey:ѡ������ 6��8 6��12 8��16  12��24
 * mode:0������ʾ��1������ʾ
 */
void OLED_ShowChar(uchar x,uchar y,uchar chr,uchar size1,uchar mode)
{
    uchar i,m,temp,size2,chr1;
    uchar x0=x,y0=y;
    if(size1==8)size2=6;
    else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //�õ�һ���ַ���Ӧ������ռ�õ��ֽ���
    chr1=chr-' ';  //����ƫ�ƺ��ֵ
    for(i=0;i<size2;i++)
    {
        if(size1==8)
              {temp=asc2_0806[chr1][i];} //����0806����
        else if(size1==12)
        {temp=asc2_1206[chr1][i];} //����1206����
        else if(size1==16)
        {temp=asc2_1608[chr1][i];} //����1608����
        else if(size1==24)
        {temp=asc2_2412[chr1][i];} //����2412����
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
*  ���ܣ���Ļ����
* x0:���Ͻ�x����
* y0:���Ͻ�y����
* x1:���Ͻ�x����
* y2:���Ͻ�y����
* (x0,y0),(x1,y1)�趨����������,����x1<x2,y1<y2;x2=0~127,y=0~7;
* direction��0ˮƽ������1��ֱ����
*/
/*
void OLED_Run(uchar x0,uchar y0,uchar x1,uchar y1,uchar direction)
{
    uchar i,j;
    uchar data=0;//�м�洢����
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
 * ��ʾ�ַ���
 * x,y:�������
 * size1:�����С
 * *chr:�ַ�����ʼ��ַ
 * mode:0������ʾ��1������ʾ
 */
void OLED_ShowString(uchar x,uchar  y,uchar  *chr,uchar  size1,uchar mode)
{
    while((*chr>=' ')&&(*chr<='~'))//�ж��ǲ��ǷǷ��ַ�
        {
            OLED_ShowChar(x,y,*chr,size1,mode);
            if(size1==8)x+=6;
            else x+=size1/2;
            chr++;
      }
}
/*
 * ��ʾ����
 * x,y���������
 * num�������������ж�Ӧ�ı��
 * size1�����ֶ�Ӧ�������С
 * mode:0������ʾ��1������ʾ
 */
//void OLED_ShowChinese(uchar x,uchar y,uchar num,uchar size1,uchar mode)
//{
//    uchar m,temp;
//    uchar x0=x,y0=y;
//    Uint16 i,size3=(size1/8+((size1%8)?1:0))*size1;  //�õ�һ�������ڵ�������ռ�õ��ֽ���
//    for(i=0;i<size3;i++)
//    {
//        if(size1==16)
//                {temp=Hzk1[num][i];}//����16*16����
//        else if(size1==24)
//                {temp=Hzk2[num][i];}//����24*24����
//        else if(size1==32)
//                {temp=Hzk3[num][i];}//����32*32����
//        else if(size1==64)
//                {temp=Hzk4[num][i];}//����64*64����
//        else return;
//        for(m=0;m<8;m++)
//        {
//            if(temp&0x01)OLED_DrawPoint(x,y,mode);
//            else OLED_DrawPoint(x,y,!mode);
//            temp>>=1;//�����ƶ�1λȻ���ڸ�ֵ��temp
//            y++;//ָ����һλ
//        }
//        x++;
//        if((x-x0)==size1)
//        {x=x0;y0=y0+8;}
//        y=y0;
//    }
//}
/*num ��ʾ���ֵĸ���
 *space ÿһ����ʾ�ļ��
 * mode:0������ʾ��1������ʾ
 */
//void OLED_ScrollDisplay(uchar num,uchar space,uchar mode)
//{
//    uchar i,n,t=0,m=0,r;
//    while(1)
//    {
//        if(m==0)
//        {
//        OLED_ShowChinese(128,24,t,16,mode); //д��һ�����ֱ��浽OLED_GRAM[]��
//            t++;
//        }
//        if(t==num)
//            {
//                for(r=0;r<16*space;r++)     //��ʾ���
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
//        for(i=1;i<128;i++)   //ʵ������
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
 *���Ժ���
 *i:0 ������ʾ��1��ɫ��ʾ
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
*        �� �� ��: OLED_SetContrast
*        ����˵��: ���öԱȶ�
*        ��    ��:  ��
*        �� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_SetContrast(uchar ucValue)
{
    OLED_WR_Byte(0x81,OLED_CMD);        /* ���öԱȶ�����(˫�ֽ��������1���ֽ��������2���ֽ��ǶԱȶȲ���0-255 */
    OLED_WR_Byte(ucValue,OLED_CMD);     /* ���öԱȶȲ���,ȱʡCF */
}

/*
 * ��Ļ��ת180��
 * i:0������ʾ��1��ת��ʾ
 */
void OLED_DisplayTurn(uchar i)
{
    if(i==0)
        {
            OLED_WR_Byte(0xC8,OLED_CMD); /* C0 ������ɨ��,��COM0��COM63;  C8 : ����ɨ��, �� COM63�� COM0 */
            OLED_WR_Byte(0xA1,OLED_CMD); /* A0 ���е�ַ0ӳ�䵽SEG0; A1 ���е�ַ127ӳ�䵽SEG0 */
        }
    if(i==1)
        {
            OLED_WR_Byte(0xC0,OLED_CMD); /* C0 ������ɨ��,��COM0��COM63;  C8 : ����ɨ��, �� COM63�� COM0 */
            OLED_WR_Byte(0xA0,OLED_CMD);/* A0 ���е�ַ0ӳ�䵽SEG0; A1 ���е�ַ127ӳ�䵽SEG0 */
        }
}

void OLED_Init(void)
{

    OLED_WR_Byte(0xAE,OLED_CMD);/*�ر�OLED�����ʾ�����ߣ�*/
    OLED_WR_Byte(0x00,OLED_CMD);/*�����е�ַ��4bit*/
    OLED_WR_Byte(0x10,OLED_CMD);/*�����е�ַ��4bit*/
    OLED_WR_Byte(0x40,OLED_CMD);/*������ʼ�е�ַ����5bit 0-63���� Ӳ�����*/
    OLED_WR_Byte(0x81,OLED_CMD);/* ���öԱȶ�����(˫�ֽ��������1���ֽ��������2���ֽ��ǶԱȶȲ���0-255 */
    OLED_WR_Byte(0xCF,OLED_CMD);/* ���öԱȶȲ���,ȱʡCF */
    OLED_WR_Byte(0xA1,OLED_CMD);/* A0 ���е�ַ0ӳ�䵽SEG0; A1 ���е�ַ127ӳ�䵽SEG0 */
    OLED_WR_Byte(0xC8,OLED_CMD);/* C0 ������ɨ��,��COM0��COM63;  C8 : ����ɨ��, �� COM63�� COM0 */
    OLED_WR_Byte(0xA6,OLED_CMD); /* A6 : ����������ʾģʽ; A7 : ����Ϊ����ģʽ */
    OLED_WR_Byte(0xA8,OLED_CMD); /* ����COM·�� */
    OLED_WR_Byte(0x3f,OLED_CMD); /* 1 ->��63+1��· */
    OLED_WR_Byte(0xD3,OLED_CMD); /* ������ʾƫ�ƣ�˫�ֽ����*/
    OLED_WR_Byte(0x00,OLED_CMD); /* ��ƫ�� */
    OLED_WR_Byte(0xd5,OLED_CMD);/* ������ʾʱ�ӷ�Ƶϵ��/��Ƶ�� */
    OLED_WR_Byte(0x80,OLED_CMD);/* ���÷�Ƶϵ��,��4bit�Ƿ�Ƶϵ������4bit����Ƶ�� */
    OLED_WR_Byte(0xD9,OLED_CMD); /* ����Ԥ������� */
    OLED_WR_Byte(0xF1,OLED_CMD); /* [3:0],PHASE 1; [7:4],PHASE 2; */
    OLED_WR_Byte(0xDA,OLED_CMD); /* ����COM��Ӳ�����߷�ʽ */
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD); /* ���� vcomh ��ѹ���� */
    OLED_WR_Byte(0x40,OLED_CMD); /* [6:4] 000 = 0.65 x VCC; 0.77 x VCC (RESET); 0.83 x VCC  */
    OLED_WR_Byte(0x20,OLED_CMD); /*������Ѱַģʽ������0x20�������0x02Ϊȷ��ҳѰַģʽ*/
    OLED_WR_Byte(0x02,OLED_CMD);
    OLED_WR_Byte(0x8D,OLED_CMD);/* ���ó��ã����¸�������ʹ�ã� */
    OLED_WR_Byte(0x14,OLED_CMD);/* 0x14 ʹ�ܳ��ã� 0x10 �ǹر� */
    OLED_WR_Byte(0xA4,OLED_CMD);
    OLED_WR_Byte(0xA6,OLED_CMD);
    OLED_WR_Byte(0x2E,OLED_CMD);/*���ù���*/
    OLED_Clear();//����
    OLED_WR_Byte(0xAF,OLED_CMD); /* ��OLED��� */
}
/*
 * ����һ���ֽ�
 * mode������/���� 0Ϊ���ݣ�1��ʾ����
 */
void OLED_WR_Byte(uchar dat,uchar mode)
{
    IIC_Start();
    IIC_Send_Byte(0x78);//���ʹӻ���ַ
    IIC_Wait_Ack();
    if(mode){IIC_Send_Byte(0x40);}//0x40��д���� Co=0 D/C=1
  else{IIC_Send_Byte(0x00);}//0x00д����Co=0 D/C=0
    IIC_Wait_Ack();
    IIC_Send_Byte(dat);
    IIC_Wait_Ack();
    IIC_Stop();
}

/*
 * ��������Դ�
 */
void OLED_Clear(void)
{
    uchar i,n;
        for(i=0;i<8;i++)
        {
           for(n=0;n<128;n++)
                {
                 OLED_GRAM[n][i]=0;//�����������
                }
      }
        OLED_Refresh();//������ʾ
}
//����
//x:0~127
//y:0~63
//t:1 ������ʾ 0,������ʲô���ݶ�������
void OLED_DrawPoint(uchar x,uchar y,uchar t)
{
    uchar i,m,n;
    i=y/8;//ȷ������һҳ
    m=y%8;//ȷ������һҳ����һλ
    n=0x01<<m;//ȷ������һҳ��ʾ��1����ƫ��mλ����
    if(x>127||y>63)return;//������Χ��
    if(t){OLED_GRAM[x][i]|=n;}
    else
    {
        OLED_GRAM[x][i]=~OLED_GRAM[x][i];//��iҳ�ĵ�x�еİ�λ����ȥ��Ҳ����0  1����
        OLED_GRAM[x][i]|=n;//ȡ���󽫴�λ�õ�������1
        OLED_GRAM[x][i]=~OLED_GRAM[x][i];//�ڽ���ȥ��
        /*��������д������þ���������ʲô���ݶ�������*/
    }
}
/**
* @brief ������
* @param x0:���Ͻ�x����
* @param y0:���Ͻ�y����
* @param x1:���Ͻ�x����
* @param y2:���Ͻ�y����
* @param show:0:����ʾ,1:��ʾ
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
* @brief ���ƾ���
* @param x0:���Ͻ�x����
* @param y0:���Ͻ�y����
* @param x1:���Ͻ�x����
* @param y2:���Ͻ�y����
*/
void Gui_Draw_Rect(uchar x0,uchar y0,uchar x1,uchar y1)
{
    OLED_DrawLine(x0,y0,x1,y0,1);
    OLED_DrawLine(x0,y0,x0,y1,1);
    OLED_DrawLine(x0,y1,x1,y1,1);
    OLED_DrawLine(x1,y0,x1,y1,1);
}

/*
 * �����κ���
 * x1��x2��������������ʾ�Ŀ�ȣ�����x1<x2
 *dataҪ��ʾ������
 *yȡֵ��ΧΪ0~7��Ҳ����y����ȷ��Ҫ��ʾ����ʹҳ
 *z������ʾΪλ����8λ��16λ��24λ��32λ��40λ��48λ��56λ��64λ
 *dataȡֵ��ΧΪȡ������ʾ��λ��
 *mode:0������ʾ��1��ת��ʾ
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
                   l=data/8+y;//ȷ������һҳ
                   m=data%8;//ȷ������һҳ����һλ
             }
        n=1<<m;//ȷ������һҳ��ʾ��1����ƫ��mλ����
        for(a=x1;a<x2;a++)//������x��ķ�����ʾ�ķ�Χ
        {
           for(c=y;c<(z/8+y);c++)
              {
                 OLED_GRAM[a][c]=OLED_GRAM[a+1][c];//����һ����Ԫ
              }
        }
        for(b=y;b<(z/8+y);b++)
             {
               OLED_GRAM[x2][b]=0;//��ÿҳ�����һ������
             }
        OLED_GRAM[x2][l]=n;
        //OLED_Refresh();//������ʾ����

}
/*
 * ����ʵ��������е���
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
/*����
 *x1,y1:�������
 *x2,y2:��������
 *mode:0��ɫ��ʾ��1��������ʾ
 */

void OLED_DrawLine(uchar x1,uchar y1,uchar x2,uchar y2,uchar mode)
{
    Uint16 t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    delta_x=x2-x1; //������������
    delta_y=y2-y1;
    uRow=x1;//�����������
    uCol=y1;

    if(delta_x>0)incx=1; //���õ�������
    else if (delta_x==0)incx=0;//��ֱ��
    else {incx=-1;delta_x=-delta_x;}

    if(delta_y>0)incy=1;
    else if (delta_y==0)incy=0;//ˮƽ��
    else {incy=-1;delta_y=-delta_y;}

    if(delta_x>delta_y)distance=delta_x; //ѡȡ��������������
    else distance=delta_y;

    for(t=0;t<distance+1;t++)
    {

        OLED_DrawPoint(uRow,uCol,mode);//����
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
/* �������� */
void Boot_Animation(void)
{
        static uchar x=0,y=0;
        for(x = 63;x>=18;x--){
                OLED_DrawPoint(108-0.7*x,x,1);//��б�� б�ʡ֡�3/3
                OLED_DrawPoint(17 +0.7*x,x,1);
                y = 64-x;
                OLED_DrawPoint(64-0.7*y,y,1);
                OLED_DrawPoint(64+0.7*y,y,1);
                DELAY_US(2);
                OLED_Refresh();//������ʾ��OLED
        }

        for(x = 30;x <= 94;x++){
                OLED_DrawPoint(125-x,47,1);
                OLED_DrawPoint(x,18,1);
                DELAY_US(2);
                OLED_Refresh();//������ʾ��OLED
        }

        OLED_ShowString(60,20,"A",16,1);
        OLED_Refresh();//������ʾ��OLED
        DELAY_US(1000);

}

//������ʾ��OLED
void OLED_Refresh(void)
{
    uchar i,n;
    for(i=0;i<8;i++)
    {
        OLED_WR_Byte(0xb0+i,OLED_CMD);//����ҳ��ַ��0~7��
        OLED_WR_Byte(0x00,OLED_CMD);/*�����е�ַ��4bit*/
        OLED_WR_Byte(0x10,OLED_CMD);/*�����е�ַ��4bit*/
        IIC_Start();
        IIC_Send_Byte(0x78);//�ӻ���ַ
        IIC_Wait_Ack();
        IIC_Send_Byte(0x40);//д����
        IIC_Wait_Ack();
        for(n=0;n<128;n++)
        {
            IIC_Send_Byte(OLED_GRAM[n][i]);
            IIC_Wait_Ack();
        }
        IIC_Stop();
  }
}
/*������Բ
 *x,y:Բ������
 *r:Բ�İ뾶
 *mode:0��ɫ��ʾ��1��������ʾ
 */
void Draw_Circle(uchar x0,uchar y0,uchar r,uchar mode)
{
    uchar y;
    float x;
        for(x = (x0-r);x <= (x0+r);x+=0.01)
        {
                y=sqrt(pow(r,2)-pow(x-x0,2))+y0; //(x-x0)^2+(y-y0)^2=r^2
                OLED_DrawPoint(x,y,mode);      //�ϰ�Բ
                OLED_DrawPoint(x,2*y0-y,mode);   //�°�Բ
        }
}
void Draw_fill_circle(uchar x0,uchar y0,uchar r,uchar mode)//д��ʵ��Բ��(x0,y0),�뾶r
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
 * x,y���������
 * sizex,sizey,ͼƬ�ĳ��Ϳ�
 * Ҫд���ͼƬ����
 * mode:0��ɫ��ʾ��1��������ʾ
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
