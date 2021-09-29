
#include "spioled.h"

#include "oledfont.h"

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127


/**********************************************
// IIC Write byte
**********************************************/

void OLED_Write_SPI_Byte(unsigned char SPI_Byte)
{
    u8 i=8;
    LCD_DC_SET();
       delay(1);
       LCD_SCL_CLR();
       delay(1);

    while(i--)
      {
        if(SPI_Byte&0x80)
        {
            LCD_SDA_SET();
            delay(1);
        }
        else{
            LCD_SDA_CLR();
            delay(1);
        }
        LCD_SCL_SET();
        delay(2);
        LCD_SCL_CLR();
        SPI_Byte<<=1;
        delay(2);
      }




}
/**********************************************
// IIC Write Command
**********************************************/
void OLED_Write_SPI_Command(unsigned char SPI_Command)
{
    u8 i=8;
    LCD_DC_CLR();
    delay(1);
    LCD_SCL_CLR();
    delay(2);


    while(i--)
    {
        if(SPI_Command&0x80)
        {
            LCD_SDA_SET();
            delay(1);
        }
        else{
            LCD_SDA_CLR();
            delay(1);
        }
        LCD_SCL_SET();
        delay(2);
        LCD_SCL_CLR();
        delay(1);
        SPI_Command<<=1;;
        delay(1);
     }

}
/**********************************************
// IIC Write Data
**********************************************/
void OLED_Write_SPI_Data(unsigned char SPI_Data)
{

       // OLED_Write_SPI_Byte(0x78);          //D/C#=0; R/W#=0

       // OLED_Write_SPI_Byte(0x40);          //write data

        OLED_Write_SPI_Byte(SPI_Data);


}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
    if(cmd)
    {
   OLED_Write_SPI_Data(dat);
    }
    else
    {
   OLED_Write_SPI_Command(dat);
    }
}


/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
    unsigned char m,n;
    for(m=0;m<8;m++)
    {
        OLED_WR_Byte(0xb0+m,0);     //page0-page1
        OLED_WR_Byte(0x00,0);       //low column start address
        OLED_WR_Byte(0x10,0);       //high column start address
        for(n=0;n<128;n++)
            {
                OLED_WR_Byte(fill_Data,1);
            }
    }
}

//坐标设置

    void OLED_Set_Pos(unsigned char x, unsigned char y)
{   OLED_WR_Byte(0xb0+y,OLED_CMD);
    OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD);
}
//开启OLED显示
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
    OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
    OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
    OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
    OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void)
{
    u8 i,n;
    for(i=0;i<8;i++)
    {
        OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
        OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
        OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址
        for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA);
    } //更新显示
}
void OLED_On(void)
{
    u8 i,n;
    for(i=0;i<8;i++)
    {
        OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
        OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
        OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址
        for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA);
    } //更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 16/12
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{
    unsigned char c=0,i=0;
        c=chr-' ';//得到偏移后的值
        if(x>Max_Column-1){x=0;y=y+2;}
        if(Char_Size ==16)
            {
            OLED_Set_Pos(x,y);
            for(i=0;i<8;i++)
            OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
            OLED_Set_Pos(x,y+1);
            for(i=0;i<8;i++)
            OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
            }
            else {
                OLED_Set_Pos(x,y);
                for(i=0;i<6;i++)
                OLED_WR_Byte(F6x8[c][i],OLED_DATA);

            }
}
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
    u32 result=1;
    while(n--)result*=m;
    return result;
}
//显示2个数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式   0,填充模式;1,叠加模式
//num:数值(0~4294967295);
void OLED_ShowNumber(u8 x,u8 y,u32 num,u8 len,u8 size2)
{
    u8 t,temp;
    u8 enshow=0;
    for(t=0;t<len;t++)
    {
        temp=(num/oled_pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
                continue;
            }else enshow=1;

        }
        OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2);
    }
}
//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
    while (*chr!='\0')
    {       OLED_ShowChar(x,y,*chr,Char_Size);
            x+=8;
        if(x>120){x=0;y+=2;}
            chr++;
    }
}
//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{
    u8 t,adder=0;
    OLED_Set_Pos(x,y);
    for(t=0;t<16;t++)
        {
                OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
                adder+=1;
     }
        OLED_Set_Pos(x,y+1);
    for(t=0;t<16;t++)
            {
                OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
                adder+=1;
      }
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{
 unsigned int j=0;
 unsigned char x,y;

  if(y1%8==0) y=y1/8;
  else y=y1/8+1;
    for(y=y0;y<y1;y++)
    {
        OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
        {
            OLED_WR_Byte(BMP[j++],OLED_DATA);
        }
    }
}
void OLED_Float(unsigned char Y,unsigned char X,double real,unsigned char N)
{
   unsigned char   i_Count=1;
   unsigned char   n[12]={0};
   long   j=1;
   int    real_int=0;
   double decimal=0;
   unsigned int   real_decimal=0;
   if(real<0)
     {
         real_int=(int)(-real);
     }
     else
     {
         real_int=(int)real;
   }
     decimal=real-real_int;
   real_decimal=decimal*1e4;
   while(real_int/10/j!=0)
   {
      j=j*10;i_Count++;
   }
   n[0]=(real_int/10000)%10;
   n[1]=(real_int/1000)%10;
   n[2]=(real_int/100)%10;
   n[3]=(real_int/10)%10;
   n[4]=(real_int/1)%10;
   n[5]='.';
   n[6]=(real_decimal/1000)%10;
   n[7]=(real_decimal/100)%10;
   n[8]=(real_decimal/10)%10;
   n[9]=real_decimal%10;
   n[6+N]='\0';
   for(j=0;j<10;j++) n[j]=n[j]+16+32;
     if(real<0)
     {
         i_Count+=1;
         n[5-i_Count]='-';
     }
   n[5]='.';
   n[6+N]='\0';
   OLED_ShowString(X,Y,&n[5-i_Count],12);
}



void OLED_Num_write(unsigned char x,unsigned char y,unsigned char asc)
{
    int i=0;
    OLED_Set_Pos(x*6,y);
    for(i=0;i<6;i++)
    {
         OLED_WR_Byte(F6x8[asc+16][i],OLED_DATA);
    }
}
void OLED_fuhao_write(unsigned char x,unsigned char y,unsigned char asc)
{

      int i=0;
    OLED_Set_Pos(x*6,y);
    for(i=0;i<6;i++)
    {
       OLED_WR_Byte(F6x8[asc][i],OLED_DATA);
    }

}


//初始化SSD1306
void OLED_Init(void)
{

    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;     //上拉
    GpioCtrlRegs.GPADIR.bit.GPIO14 = 1;     // 输出端口
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;    // IO口
    GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 3;   // 不同步

    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;     //上拉
    GpioCtrlRegs.GPADIR.bit.GPIO15 = 1;     // 输出端口
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;    // IO口
    GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;   // 不同步

    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;     //上拉
    GpioCtrlRegs.GPADIR.bit.GPIO16 = 1;     // 输出端口
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;    // IO口
    GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3;   // 不同步

    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;     //上拉
    GpioCtrlRegs.GPADIR.bit.GPIO17 = 1;     // 输出端口
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;    // IO口
    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3;   // 不同步
    EDIS;

    LCD_SCL_SET();
    LCD_RST_CLR();

    DELAY_US(500);//初始化之前的延时很重要！
    DELAY_US(500);//初始化之前的延时很重要！
    DELAY_US(500);//初始化之前的延时很重要！
    DELAY_US(500);//初始化之前的延时很重要！
    DELAY_US(500);//初始化之前的延时很重要！
    LCD_RST_SET();



    OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
    OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
    OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
    OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
    OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
    OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset   Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00,OLED_CMD);//-not offset
    OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
    OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
    OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
    OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x02,OLED_CMD);//
    OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
    OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
    OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7)
    OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel

    OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/
    OLED_Clear();
    OLED_Set_Pos(0,0);
}

///开机显示需要显示的，只需要显示一次即可。减少CPU的运算。/////////////////
void oled_first_show(void)
{

    /*OLED_ShowString(0,0,"ANGLE:",12);
    OLED_ShowString(0,2,"BAT :",12);
    OLED_ShowString(90,2,"V",12);
    OLED_ShowString(0,4,"Distance:",12);*/
    OLED_ShowString(0,0," Temp:    . C",12);
    OLED_ShowString(0,2,"Pitch:    . C",12);
    OLED_ShowString(0,4," Roll:    . C",12);
    OLED_ShowString(0,6," Yaw :    . C",12);

}



























