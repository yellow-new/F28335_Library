/**
 *  ********************************************************************************************
 *  @file                 bsp_TFT.c
 *  @file                 SK Electronics
 *  @version           V1.0
 *  @date               2021-xx-xx
 *  @brief               TFT显示函数接口
 *  *******************************************************************************************
 *  @attention
 *  实验平台：F28335   核心板
 * 个人博客：http://yellownew.cn 
 */
#include "bsp_TFT.h"
#include "bsp_SPI.h"
#include "lcdfont.h"
#include "pic.h"

extern void delay_1ms(Uint16 t);
extern Uint16 rdata;

void GPIO_TFT(void)//由于是硬件SPI，只配置RES,BLK,DC和CS管脚。软件SPI则SCLK，SDA都要配置
{
    EALLOW;

    /*配置RES输出管脚IO20*/
    GpioCtrlRegs.GPAMUX2.bit.GPIO20=0;
    GpioCtrlRegs.GPAPUD.bit.GPIO20=0;
    GpioCtrlRegs.GPADIR.bit.GPIO20=1;

    /*配置DC输出管脚IO21*/
    GpioCtrlRegs.GPAMUX2.bit.GPIO21=0;
    GpioCtrlRegs.GPAPUD.bit.GPIO21=0;
    GpioCtrlRegs.GPADIR.bit.GPIO21=1;

    /*配置BLK输出管脚IO22*/
    GpioCtrlRegs.GPAMUX2.bit.GPIO22=0;
    GpioCtrlRegs.GPAPUD.bit.GPIO22=0;
    GpioCtrlRegs.GPADIR.bit.GPIO22=1;

    /*配置CS输出管脚IO23*/
    GpioCtrlRegs.GPAMUX2.bit.GPIO23=0;
    GpioCtrlRegs.GPAPUD.bit.GPIO23=0;
    GpioCtrlRegs.GPADIR.bit.GPIO23=1;
    EDIS;
}


void TFT_Writ_Bus(Uint8 dat)
{
    TFT_CS_CLR();
    Spi_xmit(dat);
    TFT_CS_SET();
}

/**
 *  @brief                   TFT串行写入数据函数
 *  @parameter               dat 要写入的数据
 *  @return_value               无
 */
void TFT_WR_DATA8(Uint8 dat)
{

    TFT_Writ_Bus(dat<<8);

}

void TFT_WR_DATA16(Uint16 dat)
{
    TFT_Writ_Bus(dat);
    TFT_Writ_Bus(dat<<8);
}

/**
 *  @brief                   TFT写入命令函数
 *  @parameter               dat 要写入的命令
 *  @return_value               无
 */
void TFT_WR_REG(Uint8 dat)
{
    TFT_DC_CLR();//选择写命令
    TFT_Writ_Bus(dat<<8);
    TFT_DC_SET();//选择写数据
}

/**
 *  @brief                   设置起始和结束地址
 *  @parameter               x1,x2设置列的起始和结束地址；y1，y2设置行的起始和结束地址
 *  @return_value               无
 */
void TFT_Address_SET(Uint16 x1, Uint16 y1, Uint16 x2, Uint16 y2)
{
    TFT_WR_REG(0x2A);//列地址设置
    TFT_WR_DATA16(x1);
    TFT_WR_DATA16(x2);
    TFT_WR_REG(0x2B);//行地址设置
    TFT_WR_DATA16(y1);
    TFT_WR_DATA16(y2);
    TFT_WR_REG(0x2C);//储存器写
}

/**
 *  @brief                   初始化TFT配置
 *  @parameter                  无
 *  @return_value               无
 */
void Init_TFT(void)
{
    Init_Spi();//初始化SPI
    GPIO_TFT();//初始化TFT相关IO口

    TFT_RES_CLR();
    delay_1ms(100);
    TFT_RES_SET();
    delay_1ms(100);

    TFT_BLK_SET();
    delay_1ms(100);

    //************* Start Initial Sequence **********//
    TFT_WR_REG(0x11); //Sleep out
    delay_1ms(120);              //Delay 120ms
    //************* Start Initial Sequence **********//
    TFT_WR_REG(0xCF);
    TFT_WR_DATA8(0x00);
    TFT_WR_DATA8(0xC1);
    TFT_WR_DATA8(0X30);
    TFT_WR_REG(0xED);
    TFT_WR_DATA8(0x64);
    TFT_WR_DATA8(0x03);
    TFT_WR_DATA8(0X12);
    TFT_WR_DATA8(0X81);
    TFT_WR_REG(0xE8);
    TFT_WR_DATA8(0x85);
    TFT_WR_DATA8(0x00);
    TFT_WR_DATA8(0x79);
    TFT_WR_REG(0xCB);
    TFT_WR_DATA8(0x39);
    TFT_WR_DATA8(0x2C);
    TFT_WR_DATA8(0x00);
    TFT_WR_DATA8(0x34);
    TFT_WR_DATA8(0x02);
    TFT_WR_REG(0xF7);
    TFT_WR_DATA8(0x20);
    TFT_WR_REG(0xEA);
    TFT_WR_DATA8(0x00);
    TFT_WR_DATA8(0x00);
    TFT_WR_REG(0xC0); //Power control
    TFT_WR_DATA8(0x1D); //VRH[5:0]
    TFT_WR_REG(0xC1); //Power control
    TFT_WR_DATA8(0x12); //SAP[2:0];BT[3:0]
    TFT_WR_REG(0xC5); //VCM control
    TFT_WR_DATA8(0x33);
    TFT_WR_DATA8(0x3F);
    TFT_WR_REG(0xC7); //VCM control
    TFT_WR_DATA8(0x92);
    TFT_WR_REG(0x3A); // Memory Access Control
    TFT_WR_DATA8(0x55);
    TFT_WR_REG(0x36); // Memory Access Control
    if(USE_HORIZONTAL==0)TFT_WR_DATA8(0x08);
    else if(USE_HORIZONTAL==1)TFT_WR_DATA8(0xC8);
    else if(USE_HORIZONTAL==2)TFT_WR_DATA8(0x78);
    else TFT_WR_DATA8(0xA8);
    TFT_WR_REG(0xB1);
    TFT_WR_DATA8(0x00);
    TFT_WR_DATA8(0x12);
    TFT_WR_REG(0xB6); // Display Function Control
    TFT_WR_DATA8(0x0A);
    TFT_WR_DATA8(0xA2);

    TFT_WR_REG(0x44);
    TFT_WR_DATA8(0x02);

    TFT_WR_REG(0xF2); // 3Gamma Function Disable
    TFT_WR_DATA8(0x00);
    TFT_WR_REG(0x26); //Gamma curve selected
    TFT_WR_DATA8(0x01);
    TFT_WR_REG(0xE0); //Set Gamma
    TFT_WR_DATA8(0x0F);
    TFT_WR_DATA8(0x22);
    TFT_WR_DATA8(0x1C);
    TFT_WR_DATA8(0x1B);
    TFT_WR_DATA8(0x08);
    TFT_WR_DATA8(0x0F);
    TFT_WR_DATA8(0x48);
    TFT_WR_DATA8(0xB8);
    TFT_WR_DATA8(0x34);
    TFT_WR_DATA8(0x05);
    TFT_WR_DATA8(0x0C);
    TFT_WR_DATA8(0x09);
    TFT_WR_DATA8(0x0F);
    TFT_WR_DATA8(0x07);
    TFT_WR_DATA8(0x00);
    TFT_WR_REG(0XE1); //Set Gamma
    TFT_WR_DATA8(0x00);
    TFT_WR_DATA8(0x23);
    TFT_WR_DATA8(0x24);
    TFT_WR_DATA8(0x07);
    TFT_WR_DATA8(0x10);
    TFT_WR_DATA8(0x07);
    TFT_WR_DATA8(0x38);
    TFT_WR_DATA8(0x47);
    TFT_WR_DATA8(0x4B);
    TFT_WR_DATA8(0x0A);
    TFT_WR_DATA8(0x13);
    TFT_WR_DATA8(0x06);
    TFT_WR_DATA8(0x30);
    TFT_WR_DATA8(0x38);
    TFT_WR_DATA8(0x0F);
    TFT_WR_REG(0x29); //Display on
}

/**
 *  @brief                   在指定区域填充颜色
 *  @parameter               xsta,ysta 起始坐标；xend,yend 终止坐标；color 要填充的颜色
 *  @return_value               无
 */
void TFT_Fill(Uint16 xsta,Uint16 ysta,Uint16 xend,Uint16 yend,Uint16 color)
{
    Uint16 i,j;
    TFT_Address_SET(xsta,ysta,xend-1,yend-1);//设置显示范围
    for(i=ysta;i<yend;i++)
    {
        for(j=xsta;j<xend;j++)
        {
            TFT_WR_DATA16(color);
        }
    }
}

/**
 *  @brief                   在指定位置画点
 *  @parameter               x,y画点坐标；color 点的颜色
 *  @return_value               无
 */
void TFT_DrawPoint(Uint16 x,Uint16 y,Uint16 color)
{
    TFT_Address_SET(x,y,x,y);//设置光标位置
    TFT_WR_DATA16(color);
}

/**
 *  @brief                   画线
 *  @parameter               x1,y1 起始坐标；x2,y2 终止坐标；color 线的颜色
 *  @return_value               无
 */
void TFT_DrawLine(Uint16 x1,Uint16 y1,Uint16 x2,Uint16 y2,Uint16 color)
{
    Uint16 t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    delta_x=x2-x1;//计算坐标增量
    delta_y=y2-y1;
    uRow=x1;//画线的起点坐标
    uCol=y1;
    if(delta_x>0)incx=1;//设置单步方向
    else if (delta_x==0)incx=0;//垂直线
    else {incx=-1;delta_x=-delta_x;}
    if(delta_y>0)incy=1;
    else if (delta_y==0)incy=0;//水平线
    else {incy=-1;delta_y=-delta_y;}
    if(delta_x>delta_y)distance=delta_x;//选取基本增量坐标轴
    else distance=delta_y;
    for(t=0;t<distance+1;t++)
    {
        TFT_DrawPoint(uRow,uCol,color);//画点
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

/**
 *  @brief                   画矩形
 *  @parameter               x1,y1 起始坐标；x2,y2 终止坐标；color 矩形的颜色
 *  @return_value               无
 */
void TFT_DrawRectangle(Uint16 x1,Uint16 y1,Uint16 x2,Uint16 y2,Uint16 color)
{
    TFT_DrawLine(x1,y1,x2,y1,color);
    TFT_DrawLine(x1,y1,x1,y2,color);
    TFT_DrawLine(x1,y2,x2,y2,color);
    TFT_DrawLine(x2,y1,x2,y2,color);
}

/**
 *  @brief                   画圆
 *  @parameter               x0,y0 圆心坐标；r 半径；color 圆的颜色
 *  @return_value               无
 */
void Draw_Circle(Uint16 x0,Uint16 y0,Uint8 r,Uint16 color)
{
    int a,b;
    a=0;b=r;
    while(a<=b)
    {
        TFT_DrawPoint(x0-b,y0-a,color);             //3
        TFT_DrawPoint(x0+b,y0-a,color);             //0
        TFT_DrawPoint(x0-a,y0+b,color);             //1
        TFT_DrawPoint(x0-a,y0-b,color);             //2
        TFT_DrawPoint(x0+b,y0+a,color);             //4
        TFT_DrawPoint(x0+a,y0-b,color);             //5
        TFT_DrawPoint(x0+a,y0+b,color);             //6
        TFT_DrawPoint(x0-b,y0+a,color);             //7
        a++;
        if((a*a+b*b)>(r*r))//判断要画的点是否过远
        {
            b--;
        }
    }
}

/**
 *  @brief                   显示汉字串
 *  @parameter               x,y 显示坐标；*s 要显示的汉字串；fc 字的颜色；bc 字的背景颜色；sizey 字号 可选 16 24 32；mode 0非叠加模式 1叠加模式
 *  @return_value               无
 */
void TFT_ShowChinese(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode)
{
    while(*s!=0)
    {
        if(sizey==12) TFT_ShowChinese12x12(x,y,s,fc,bc,sizey,mode);
        else if(sizey==16) TFT_ShowChinese16x16(x,y,s,fc,bc,sizey,mode);
        else if(sizey==24) TFT_ShowChinese24x24(x,y,s,fc,bc,sizey,mode);
        else if(sizey==32) TFT_ShowChinese32x32(x,y,s,fc,bc,sizey,mode);
        else return;
        s+=2;
        x+=sizey;
    }
}

/**
 *  @brief                   显示单个12x12的汉字
 *  @parameter               x,y 显示坐标；*s 要显示的汉字；fc 字的颜色；bc 字的背景颜色；sizey 字号 可选 16 24 32；mode 0非叠加模式 1叠加模式
 *  @return_value               无
 */
void TFT_ShowChinese12x12(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode)
{
    Uint8 i,j,m=0;
    Uint16 k;
    Uint16 HZnum;//汉字的数目
    Uint16 TypefaceNum;//一个字符所占字节大小
    Uint16 x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;

    HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);  //统计汉字数目
    for(k=0;k<HZnum;k++)
    {
        if ((tfont12[k].Index[0]==(*(s))+0xFF00)&&(tfont12[k].Index[1]==(*(s+1))+0xFF00))
        {
            TFT_Address_SET(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//非叠加式
                    {
                        if(tfont12[k].Msk[i]&(0x01<<j))TFT_WR_DATA16(fc);
                        else TFT_WR_DATA16(bc);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//叠加式
                    {
                        if(tfont12[k].Msk[i]&(0x01<<j)) TFT_DrawPoint(x,y,fc);//画一个点
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/**
 *  @brief                   显示单个16x16的汉字
 *  @parameter               x,y 显示坐标；*s 要显示的汉字；fc 字的颜色；bc 字的背景颜色；sizey 字号 可选 16 24 32；mode 0非叠加模式 1叠加模式
 *  @return_value               无
 */
void TFT_ShowChinese16x16(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode)
{
    Uint8 i,j,m=0;
    Uint16 k;
    Uint16 HZnum;//汉字数目
    Uint16 TypefaceNum;//一个字符所占字节大小
    Uint16 x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);  //统计汉字数目
    for(k=0;k<HZnum;k++)
    {
        if ((tfont16[k].Index[0]==(*(s))+0xFF00)&&(tfont16[k].Index[1]==(*(s+1))+0xFF00))
        {
            TFT_Address_SET(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//非叠加方式
                    {
                        if(tfont16[k].Msk[i]&(0x01<<j))TFT_WR_DATA16(fc);
                        else TFT_WR_DATA16(bc);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if(tfont16[k].Msk[i]&(0x01<<j)) TFT_DrawPoint(x,y,fc);//画一个点
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/**
 *  @brief                   显示单个24x24的汉字
 *  @parameter               x,y 显示坐标；*s 要显示的汉字；fc 字的颜色；bc 字的背景颜色；sizey 字号 可选 16 24 32；mode 0非叠加模式 1叠加模式
 *  @return_value               无
 */
void TFT_ShowChinese24x24(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode)
{
    Uint8 i,j,m=0;
    Uint16 k;
    Uint16 HZnum;//汉字数目
    Uint16 TypefaceNum;//一个字符所占字节的大小
    Uint16 x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);  //统计汉字数目
    for(k=0;k<HZnum;k++)
    {
        if ((tfont24[k].Index[0]==(*(s))+0xFF00)&&(tfont24[k].Index[1]==(*(s+1))+0xFF00))
        {
            TFT_Address_SET(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//非叠加方式
                    {
                        if(tfont24[k].Msk[i]&(0x01<<j))TFT_WR_DATA16(fc);
                        else TFT_WR_DATA16(bc);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if(tfont24[k].Msk[i]&(0x01<<j)) TFT_DrawPoint(x,y,fc);//画一个点
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/**
 *  @brief                   显示单个32x32的汉字
 *  @parameter               x,y 显示坐标；*s 要显示的汉字；fc 字的颜色；bc 字的背景颜色；sizey 字号 可选 16 24 32；mode 0非叠加模式 1叠加模式
 *  @return_value               无
 */
void TFT_ShowChinese32x32(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode)
{
    Uint8 i,j,m=0;
    Uint16 k;
    Uint16 HZnum;//汉字数目
    Uint16 TypefaceNum;//一个字符所占字节大小
    Uint16 x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);  //统计汉字数目
    for(k=0;k<HZnum;k++)
    {
        if ((tfont32[k].Index[0]==(*(s))+0xFF00)&&(tfont32[k].Index[1]==(*(s+1))+0xFF00))
        {
            TFT_Address_SET(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//非叠加方式
                    {
                        if(tfont32[k].Msk[i]&(0x01<<j))TFT_WR_DATA16(fc);
                        else TFT_WR_DATA16(bc);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if(tfont32[k].Msk[i]&(0x01<<j)) TFT_DrawPoint(x,y,fc);//画一个点
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/**
 *  @brief                   SPI三个字
 *  @parameter               x,y 显示坐标；*s 要显示的汉字；fc 字的颜色；bc 字的背景颜色；sizey 字号 选48；mode 0非叠加模式 1叠加模式
 *  @return_value               无
 */
void TFT_SPIChinese(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode)
{
    while(*s!=0)
    {
        TFT_ShowChineseSPI(x,y,s,fc,bc,sizey,mode);
        s+=2;
        x+=sizey;
    }
}

/**
 *  @brief                   显示单个48x48的汉字
 *  @parameter               x,y 显示坐标；*s 要显示的汉字；fc 字的颜色；bc 字的背景颜色；sizey 字号 选 48；mode 0非叠加模式 1叠加模式
 *  @return_value               无
 */
void TFT_ShowChineseSPI(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode)
{
    Uint8 i,j,m=0;
    Uint16 k;
    Uint16 HZnum;//汉字数目
    Uint16 TypefaceNum;//一个字符所占字节大小
    Uint16 x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum=sizeof(tfont32SPI)/sizeof(typFNT_GBSPI);  //统计汉字数目
    for(k=0;k<HZnum;k++)
    {
        if ((tfont32SPI[k].Index[0]==(*(s))+0xFF00)&&(tfont32SPI[k].Index[1]==(*(s+1))+0xFF00))
        {
            TFT_Address_SET(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//非叠加方式
                    {
                        if(tfont32SPI[k].Msk[i]&(0x01<<j))TFT_WR_DATA16(fc);
                        else TFT_WR_DATA16(bc);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if(tfont32SPI[k].Msk[i]&(0x01<<j)) TFT_DrawPoint(x,y,fc);//画一个点
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}
/**
 *  @brief                   显示单个字符
 *  @parameter               x,y 显示坐标；num 要显示的字符；fc 字的颜色；bc 字的背景颜色；sizey 字号 可选 16 24 32；mode 0非叠加模式 1叠加模式
 *  @return_value               无
 */
void TFT_ShowChar(Uint16 x,Uint16 y,Uint8 num,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode)
{
    Uint8 temp,sizex,t,m=0;
    Uint16 i,TypefaceNum;//一个字符所占字节大小
    Uint16 x0=x;
    sizex=sizey/2;
    TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
    num=num-' ';    //得到偏移后的值
    TFT_Address_SET(x,y,x+sizex-1,y+sizey-1);  //设置光标位置
    for(i=0;i<TypefaceNum;i++)
    {
        if(sizey==12)temp=ascii_1206[num][i];            //调用6x12字体
        else if(sizey==16)temp=ascii_1608[num][i];       //调用8x16字体
        else if(sizey==24)temp=ascii_2412[num][i];       //调用12x24字体
        else if(sizey==32)temp=ascii_3216[num][i];       //调用16x32字体
        else return;
        for(t=0;t<8;t++)
        {
            if(!mode)//非叠加模式
            {
                if(temp&(0x01<<t))TFT_WR_DATA16(fc);
                else TFT_WR_DATA16(bc);
                m++;
                if(m%sizex==0)
                {
                    m=0;
                    break;
                }
            }
            else//叠加模式
            {
                if(temp&(0x01<<t))TFT_DrawPoint(x,y,fc);//画一个点
                x++;
                if((x-x0)==sizex)
                {
                    x=x0;
                    y++;
                    break;
                }
            }
        }
    }
}

/**
 *  @brief                   显示字符串
 *  @parameter               x,y 显示坐标；*p 要显示的字符串；fc 字的颜色；bc 字的背景颜色；sizey 字号 可选 16 24 32；mode 0非叠加模式 1叠加模式
 *  @return_value               无
 */
void TFT_ShowString(Uint16 x,Uint16 y,const Uint8 *p,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode)
{
    while(*p!='\0')
    {
        TFT_ShowChar(x,y,*p,fc,bc,sizey,mode);
        x+=sizey/2;
        p++;
    }
}

/**
 *  @brief                   显示指数
 *  @parameter               m底数，n指数
 *  @return_value               32位
 */
Uint32 mypow(Uint8 m,Uint8 n)
{
    Uint32 result=1;
    while(n--)result*=m;
    return result;
}

/**
 *  @brief                   显示整数变量
 *  @parameter               x,y 显示坐标；num 要显示整数变量；len 要显示的位数；fc 字的颜色；bc 字的背景颜色；sizey
 *  @return_value               无
 */
void TFT_ShowIntNum(Uint16 x,Uint16 y,Uint16 num,Uint8 len,Uint16 fc,Uint16 bc,Uint8 sizey)
{
    Uint8 t,temp;
    Uint8 enshow=0;
    Uint8 sizex=sizey/2;
    for(t=0;t<len;t++)
    {
        temp=(num/mypow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                TFT_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
                continue;
            }else enshow=1;

        }
        TFT_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
    }
}

/**
 *  @brief                   显示两位小数变量
 *  @parameter               x,y 显示坐标；num 要显示整数变量；len 要显示的位数；fc 字的颜色；bc 字的背景颜色；sizey
 *  @return_value               无
 */
void TFT_ShowFloatNum1(Uint16 x,Uint16 y,float num,Uint8 len,Uint16 fc,Uint16 bc,Uint8 sizey)
{
    Uint8 t,temp,sizex;
    Uint16 num1;
    sizex=sizey/2;
    num1=num*100;
    for(t=0;t<len;t++)
    {
        temp=(num1/mypow(10,len-t-1))%10;
        if(t==(len-2))
        {
            TFT_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);
            t++;
            len+=1;
        }
        TFT_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
    }
}

/**
 *  @brief                   显示图片
 *  @parameter               x,y 起点坐标；length 图片长度；width 图片宽度；pic[] 图片数组
 *  @return_value               无
 */
void TFT_ShowPicture(Uint16 x,Uint16 y,Uint16 length,Uint16 width,const Uint8 pic[])
{
    Uint16 i,j;
    Uint32 k=0;
    TFT_Address_SET(x,y,x+length-1,y+width-1);
    for(i=0;i<length;i++)
    {
        for(j=0;j<width;j++)
        {
            TFT_WR_DATA8(pic[k*2]);
            TFT_WR_DATA8(pic[k*2+1]);
            k++;
        }
    }
}
