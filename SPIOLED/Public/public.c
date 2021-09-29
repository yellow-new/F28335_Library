/*
 * public.c
 *
 *  Created on: 2019年7月10日
 *      Author: Administrator
 */


#include "public.h"

void delay(Uint16 time)                     //延时函数
{
    for(; time>0 ; time--)
    {
        asm(" nop");
    }
}

void  delay_ms(unsigned int nDelay)
{
    int ii,jj,kk=0;
    for ( ii=0;ii<nDelay;ii++ )
    {
        for ( jj=0;jj<512;jj++ )
        {
            kk++;
        }
    }
}
