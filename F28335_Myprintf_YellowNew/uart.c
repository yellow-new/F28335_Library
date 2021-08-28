/*
 * uart.c
 *
 *  Created on: 2021.3.27
 *      Author: huang'xin
 */

#include "uart.h"
#include "stdio.h"
int negative_number=0;

//将printf重定向到串口打印
int fputc(int _c, register FILE *_fp)
{
        while (ScibRegs.SCICTL2.bit.TXEMPTY == 0);
                ScibRegs.SCITXBUF = _c;
        return _c;
}

//串口初始化
void UARTa_Init(Uint32 baud)
{
	unsigned char scihbaud=0;
	unsigned char scilbaud=0;
	Uint16 scibaud=0;

	scibaud=37500000/(8*baud)-1;
	scihbaud=scibaud>>8; //取得高八位
	scilbaud=scibaud&0xff;//取得低八位


	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 1;   // SCI-B
	EDIS;

	InitScibGpio();

	//Initalize the SCI FIFO
	ScibRegs.SCIFFTX.all=0xE040;
	ScibRegs.SCIFFRX.all=0x204f;
	ScibRegs.SCIFFCT.all=0x0;

	// Note: Clocks were turned on to the Scib peripheral
	// in the InitSysCtrl() function
	ScibRegs.SCICCR.all =0x0007;    // 1 stop bit,  No loopback
								    // No parity,8 char bits,
								    // async mode, idle-line protocol
	ScibRegs.SCICTL1.all =0x0003;   // enable TX, RX, internal SCICLK,
								    // Disable RX ERR, SLEEP, TXWAKE
	ScibRegs.SCICTL2.all =0x0003;
	ScibRegs.SCICTL2.bit.TXINTENA =1;
	ScibRegs.SCICTL2.bit.RXBKINTENA =1;
	ScibRegs.SCIHBAUD    =scihbaud;  // 9600 baud @LSPCLK = 37.5MHz.
	ScibRegs.SCILBAUD    =scilbaud;
//	ScibRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back
	ScibRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset

}


// Transmit a character from the SCI'
void UARTa_SendByte(int a)
{
	while (ScibRegs.SCIFFTX.bit.TXFFST != 0);
	ScibRegs.SCITXBUF=a;
}

void UARTa_SendString(char * msg)
{
	int i=0;

	while(msg[i] != '\0')
	{
		UARTa_SendByte(msg[i]);
		i++;
	}
}

//打印整数,参数：生成的数组str,待转换的整数Num,整数长度为len
void Int_Printf(char str[],int Num,char len)//显示数字,长度为len
{
    unsigned char i=0;
    while(len--)
    {
        str[len] = Num%10+0x30;
        Num = Num/10;
        i++;
    }
    str[i++] = '\0';
    UARTa_SendString(str);
}


//m^n函数
unsigned long num_pow(char m,char n)
{
    unsigned long result=1;
    while(n--)result*=m;
    return result;
}

//打印浮点数,参数：生成的数组str,待转换的浮点数floa,整数长度为len1,小数长度为len2
void Float_Printf(char str[],float floa,char len1,char len2)
{
    if(floa<0)
    {
        floa = -floa;
        negative_number = 1;
    }
    else negative_number = 0;
    unsigned char i=0,j=0;
    unsigned int Integer = floa;
    unsigned int decimal = (floa-Integer)*num_pow(10,len2);
    while(len1--)
    {
        str[len1] = Integer%10+0x30;
        Integer = Integer/10;
        i++;
    }
    str[i] = '.';
    while(len2--)
    {
        str[i+len2+1] = decimal%10+0x30;
        decimal = decimal/10;
        j++;
    }
    str[i+j+1] = '\0';
 //负数->字符串数组右移位
    if( negative_number == 1)
    {
        int t;

        for(t=i+j+1;t>=1;t--)
        {
            str[t]=str[t-1];
        }
        str[0]=0x2D;//0x2D 为"-"的ascll码值
        str[i+j+2]='\0';
    }
    UARTa_SendString(str);
}

