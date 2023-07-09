#ifndef __oled_H
#define __oled_H

#include "delay.h"
#include "sys.h"

#define OLED_MODE 2          //OLEDģʽ,0:����8080ģʽ,1:4��SPIģʽ,2:I2Cģʽ

#define DATAOUT(Data_Value) {GPIO_Write(GPIOB,(GPIO_ReadOutputData(GPIOB)&0xFF00)|(Data_Value&0x00FF));}

#define OLED_8080_RST PBout(14)//8080:��λ(�͵�ƽ��Ч)
#define OLED_8080_CS  PCout(9) //8080:Ƭѡ�ź�(�͵�ƽ��Ч)
#define OLED_8080_DC  PCout(8) //8080:����/������� ����,DC=1,��������,DC=0,����ָ��
#define OLED_8080_RW  PCout(7) //8080:дʹ���ź�
#define OLED_8080_RD  PCout(6) //8080:��ʹ���ź�

#define OLED_SCLK PCout(5)     //SPI:D0(SCLK)����ʱ����
#define OLED_SDIN PCout(4)     //SPI:D1(MOSI)����������
#define OLED_RES  PCout(1)     //SPI:��λ(�͵�ƽ��Ч)
#define OLED_DC   PDout(2)     //SPI:����/������� ����,DC=1,��������,DC=0,����ָ��
#define OLED_CS   PCout(11)    //SPI:Ƭѡ�ź�(�͵�ƽ��Ч)

#define OLED_SCL PAout(5)      //I2C:SCL����ʱ����
#define OLED_SDA PAout(4)     //I2C:SDA����������

extern const unsigned char Chinese[][16];//�Զ��������ֿ�
extern const unsigned char Char6x8[][6]; //(6x8)�ֿ�
extern const unsigned char Char8x16[];   //(8x16)�ֿ�

void OLED_Init(void);                                     //OLED��ʼ��
void Start_I2C(void);                                     //����I2C
void Stop_I2C(void);                                      //ֹͣI2C
u8 I2C_Detection_ACK(void);                               //I2C���Ӧ��
void I2C_Write_Byte(u8 Data);                             //I2Cд�ֽ�
void OLED_Write_Byte(u8 Data,u8 Com);                     //��OLEDд��һ���ֽ�
void OLED_Set_Coordinate(u8 x,u8 y);                      //OLED��������
void OLED_Display_16x16(u8 x,u8 y,u8 z,const u8 (*p)[16]);//OLED��ʾ(16*16)
void OLED_Display_6x8_8x16(u8 x,u8 y,u8 z,const u8 *p);   //OLED��ʾ(6*8)/(8*16)
void OLED_Dispaly_Bmp_128x64(u8 x,u8 y,const u8 *p);      //OLED��ʾ(128*64)
void OLED_Clear(void);                                    //OLED����

#endif
