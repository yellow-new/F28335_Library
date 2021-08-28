/*
 * uart.h
 *
 *  Created on: 2018-1-31
 *      Author: Administrator
 */

#ifndef UART_H_
#define UART_H_

#include "DSP2833x_Device.h"     // DSP2833x ͷ�ļ�
#include "DSP2833x_Examples.h"   // DSP2833x �������ͷ�ļ�

extern int negative_number;

void UARTa_Init(Uint32 baud);
void UARTa_SendByte(int a);
void UARTa_SendString(char * msg);
void Num2Str(char str[],int Num,char len);
unsigned long num_pow(char m,char n);
void Float_printf(char str[],float floa,char len1,char len2);
#endif /* UART_H_ */
