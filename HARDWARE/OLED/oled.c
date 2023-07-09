#include "oled.h"
#include "font.h"

//OLED��ʼ��
void OLED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	                                            //ʹ��AFIOʱ��(ʹ�ܹ��ܸ���IO)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);  //ʹ��PB,PC,PD�˿�ʱ��
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);                                         //JTAG-DPʧ��+SW-DPʹ��
	

#if OLED_MODE==0
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                                            //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_Write(GPIOB,0xFFFF);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                                            //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);

	
#elif OLED_MODE==1||OLED_MODE==2
	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_11|GPIO_Pin_13;//����PC1,PC4,PC5,PC11����
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	                                            //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                                            //IO���ٶ�Ϊ50MHz
//	GPIO_Init(GPIOC,&GPIO_InitStructure); 	                                                        //��ʼ��GPIOC
//	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2; 	                                                    //����PD2����
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	                                            //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                                            //IO���ٶ�Ϊ50MHz
//	GPIO_Init(GPIOD,&GPIO_InitStructure); 	                                                        //��ʼ��GPIOD
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5;//��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	                                            //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                                            //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	                                                        //��ʼ��GPIOC
	
		
//	OLED_RES=0;
//	delay_ms(200);
//	OLED_RES=1;
//	delay_ms(200);
	
#endif
	//д��һϵ������
	OLED_Write_Byte(0xAE,0);//�ر���ʾ���
	OLED_Write_Byte(0x00,0);
	OLED_Write_Byte(0x10,0);
	OLED_Write_Byte(0x40,0);
	OLED_Write_Byte(0x81,0);//������öԱȶȡ�
	OLED_Write_Byte(0xCF,0);
	OLED_Write_Byte(0xA1,0);
	OLED_Write_Byte(0xC8,0);
	OLED_Write_Byte(0xA6,0);
	OLED_Write_Byte(0xA8,0);
	OLED_Write_Byte(0x3F,0);
	OLED_Write_Byte(0xD3,0);
	OLED_Write_Byte(0x00,0);
	OLED_Write_Byte(0xD5,0);
	OLED_Write_Byte(0x80,0);
	OLED_Write_Byte(0xD9,0);
	OLED_Write_Byte(0xF1,0);
	OLED_Write_Byte(0xDA,0);
	OLED_Write_Byte(0x12,0);
	OLED_Write_Byte(0xDB,0);
	OLED_Write_Byte(0x40,0);
	OLED_Write_Byte(0x20,0);
	OLED_Write_Byte(0x02,0);
	OLED_Write_Byte(0x8D,0);
	OLED_Write_Byte(0x14,0);
	OLED_Write_Byte(0xA4,0);
	OLED_Write_Byte(0xA6,0);
	OLED_Write_Byte(0xAF,0);//������ʾ���
	
	OLED_Clear();//OLED����
}
//����I2C
void Start_I2C(void)
{
	OLED_SDA=1;
	delay_us(5);
}
//ֹͣI2C
void Stop_I2C(void)
{
	OLED_SDA=0;
	delay_us(5);
}
//I2C���Ӧ��
u8 I2C_Detection_ACK(void)
{
	u8 ACK_bit;
	OLED_SDA=1;
	delay_us(5);
	OLED_SCL=1;
	delay_us(1);
	ACK_bit=OLED_SDA;//Ӧ���ź�,0:Ӧ��,1:��Ӧ��
	OLED_SCL=0;
	return ACK_bit;
}
//I2Cд�ֽ�
//Data:����/����
void I2C_Write_Byte(u8 Data)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(Data&0x80)
			OLED_SDA=1;
		else
			OLED_SDA=0;
	}
    I2C_Detection_ACK();//I2C���Ӧ��
}
#if OLED_MODE==0//����8080ģʽ
//��OLEDд��һ���ֽ�
//Data:����/����
//Com:1:��ʾ����
//    0:��ʾ����
void OLED_Write_Byte(u8 Data,u8 Com)
{
	DATAOUT(Data);
 	OLED_8080_DC=Com;
	OLED_8080_CS=0;
	OLED_8080_RW=0;
	OLED_8080_RW=0;
	OLED_8080_CS=0; 
	OLED_8080_DC=1;
}
#endif
#if OLED_MODE==1//4��SPIģʽ
//��OLEDд��һ���ֽ�
//Data:����/����
//Com:1:��ʾ����
//    0:��ʾ����
void OLED_Write_Byte(u8 Data,u8 Com)
{
	u8 i;
	OLED_CS=0;
	OLED_DC=Com;
	for(i=0;i<8;i++)
	{
		OLED_SCLK=0;
		if(Data&0x80)
			OLED_SDIN=1;
		else
			OLED_SDIN=0;
		OLED_SCLK=1;
		Data<<=1;
	}
	OLED_CS=1;
}
#endif
#if OLED_MODE==2//I2Cģʽ
//��OLEDд��һ���ֽ�
//Data:����/����
//Com:1:��ʾ����
//    0:��ʾ����
void OLED_Write_Byte(u8 Data,u8 Com)
{
	Start_I2C();             //����I2C
	I2C_Write_Byte(0x78);    //OLED��ַ(�ӻ���ַ)��SA0=0,RW=0 
	if(Com)//Com:1:��ʾ����	0:��ʾ����
		I2C_Write_Byte(0x40);//д������
}
#endif
//OLED��������
//x:ҳ��ַ(0~7)
//y:�е�ַ(0~127)
void OLED_Set_Coordinate(u8 x,u8 y)
{//B0~B7����ҳ��ַ��һ��8ҳ��
	OLED_Write_Byte(0xB0+x,0);            //д������ҳ��ַ�����һҳ��
	OLED_Write_Byte(((0xF0&y)>>4)|0x10,0);//д�������е�ַ(����λ)	
	OLED_Write_Byte((0x0F&y),0);          //д�������е�ַ(����λ)
}
//OLED��ʾ(16*16)
//x:ҳ��ַ(0~7)
//y:�е�ַ(0~127)
//z:(16*16)��������ѡ��
//p:(16*16)�������ݻ����׵�ַ
void OLED_Display_16x16(u8 x,u8 y,u8 z,const u8 (*p)[16])
{
	u8 i;
	OLED_Set_Coordinate(x,y);//OLED��������
	for(i=0;i<16;i++)
		OLED_Write_Byte(p[2*z][i],1);
	OLED_Set_Coordinate(x+1,y);
	for(i=0;i<16;i++)
		OLED_Write_Byte(p[2*z+1][i],1);
}
//OLED��ʾ(6*8)/(8*16)
//x:ҳ��ַ(0~7)
//y:�е�ַ(0~127)
//z:0:��ʾ(6*8)
//  1:��ʾ(8*16)
//p:�������ݻ����׵�ַ
void OLED_Display_6x8_8x16(u8 x,u8 y,u8 z,const u8 *p)
{
	u8 i=0,j,n;
	while(p[i]!='\0')
	{
		n=p[i]-' ';
		if(z==0)
		{
			OLED_Set_Coordinate(x+1,y);
			for(j=0;j<6;j++)
				OLED_Write_Byte(Char6x8[n][j],1);
		}
		i++;
		y+=8;
	}
}
//OLED��ʾ��С(128*64)
//x:ҳ��ַ(0~7)
//y:�е�ַ(0~127)
//p:�������ݻ����׵�ַ
void OLED_Dispaly_Bmp_128x64(u8 x,u8 y,const u8 *p)
{
	u8 i,j;
	u32 n=0;
	for(i=x;i<8;i++)
	{
		OLED_Set_Coordinate(i,y);
		for(j=y;j<128;j++)
			OLED_Write_Byte(p[n++],1);
	}
}
//OLED����
void OLED_Clear()
{
	u8 i,j;
	for(i=0;i<8;i++)
	{
		OLED_Write_Byte(0xB0+i,0);  //����ҳ��ַ����
		OLED_Write_Byte(0x00,0);    //�����е�ַ����λ����
		OLED_Write_Byte(0x10,0);    //�����е�ַ����λ����
		for(j=0;j<128;j++)
		OLED_Write_Byte(0x00,1);//д����0
	}
}
