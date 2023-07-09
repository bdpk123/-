#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "sys.h" 

#define USART1_RX_STA 256

extern u8 USART_RX_BUF[256];
extern u16 USART_RX_STA;

extern u8 USART1_RX_BUF[1];

extern u8 USART2_RX_BUF[7];//���ջ���,���USART_REC_LEN���ֽ�.

extern unsigned char reca_start;//���ܿ�ʼ�ı�־
extern unsigned char reca_num;	 //���ܵĸ���
extern unsigned char reca_ok;   //�������

void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);

void USART1_TX(u8 *data);
void USART2_TX(u8 *data);
void USART3_TX(u8 *data);

#endif


