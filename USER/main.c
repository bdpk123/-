#include "buzzer.h"
#include "dht11.h"
#include "key.h"
#include "oled.h"
#include "timer.h"
#include "humiture_data.h"

#include "delay.h"
#include "sys.h"
#include "usart.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char usart1_app_buff[256] = {0};

uint8_t relay_flag = 0;
uint8_t mode_flag = 0;

void relay_init(void);                //��ʪ���̵�����ʼ��
void relay_set(uint8_t flag);         //��ʪ���̵�������
void water_stage_init(void);          //ˮλ��������ʼ��
uint16_t water_stage_get(void);       //ˮλ������ֵ��ȡ
void humiture_alarm_set_dispose(void);//��ʪ�ȱ������ô���
void main_interface(void);            //������
void humidity_interface(void);        //ʪ�Ƚ���

int main(void)
{
	uint32_t time_count = 0;
	uint8_t temp = 0;
	uint8_t humidity = 0;
	uint16_t water_stage_value = 0;
	char str1[8] = {0};
	char str2[8] = {0};
	
	memset(&humiture_data, 0, sizeof(humitureDataTypeDef));//�����ڴ�ռ�

	humiture_data.humidity_alarm_value = 80;//��ʼʪ�ȱ���ֵΪ80
	
	delay_init();	  //��ʱ��ʼ��,72MHz
	
	DHT11_Init();      //��ʪ�ȳ�ʼ��(PB11)
	relay_init();      //��ʪ���̵�����ʼ��(PA8)
	key_init();        //������ʼ��(PB3,PB4,PC13)
	water_stage_init();//ˮλ��������ʼ��(PA2)
	OLED_Init();       //OLED��ʼ��(PA4,PA5)
	buzzer_init();     //��������ʼ��(PB12)
	uart1_init(9600);//����1��ʼ��(PA9,PA10)
	
	delay_ms(1500);
	
	OLED_Clear();		//OLED����
	
	delay_ms(150);
	
	main_interface();	//������
	
	humiture_data.time3_count = 0;
	humiture_data.time3_count1 = 0;
	ON_TIMx(TIM3);
	
	while(1)
	{
		/* ��ʪ�ȱ������ô��� */
		humiture_alarm_set_dispose();//������ƣ��л���
		
		/* ��ʾ��־Ϊ0������ʾ��ʪ�Ⱥͼ�ر��� */
		if (humiture_data.show_flag == 0)
		{
			/* ��ȡˮλADCֵ */
			water_stage_value = water_stage_get();
			//����0��ȡ������1��ȡʧ��
			if (DHT11_Read_Data(&temp, &humidity) == 0)//��ȡ������ʪ��ֵ
			{
				sprintf(str2, "%02d", humidity);//��ǰʪ��ת�ַ���
				OLED_Display_6x8_8x16(0, 40, 1, (const u8 *)str2);
				
				sprintf(str1, "%02d", temp);//��ǰ�¶�ת�ַ���
				OLED_Display_6x8_8x16(0, 104, 1, (const u8 *)str1);
			}
			
			/* ������־Ϊ0���Լ�ر��� */
			if (humiture_data.alarm_flag == 0)
			{//ˮ�������ҷ�����û�б���
				if (water_stage_value < 3500)
				{
					humiture_data.time3_count = 0;
					
					OLED_Display_16x16(6, 80, 16, Chinese);//��ʾ"��"
					OLED_Display_16x16(6, 96, 17, Chinese);//��ʾ"��"
					
					if (mode_flag == 0)
					{
						/* ��ǰʪ�ȴ��ڱ���ʪ��ֵ */
						if (humidity < humiture_data.humidity_alarm_value)
						{
							OLED_Display_6x8_8x16(2, 96, 1, (const u8 *)"ON ");
							
							relay_flag = 1;
							relay_set(1);
						}
						else
						{
							OLED_Display_6x8_8x16(2, 96, 1, (const u8 *)"OFF");
							
							relay_flag = 0;
							relay_set(0);
						}
					}
				}
				else
				{
					OLED_Display_6x8_8x16(2, 96, 1, (const u8 *)"OFF");
					
					OLED_Display_16x16(6, 80, 14, Chinese);//��ʾ"ȱ"
					OLED_Display_16x16(6, 96, 15, Chinese);//��ʾ"ˮ"
					
					relay_flag = 0;
					relay_set(0);
					
					humiture_data.alarm_flag = 1;//��������

				}
			}


		time_count++;
        if (time_count >= 155)
        {
			__set_PRIMASK(1);
			 //PRIMASK ֻ�е�һ���صļĴ�������Ϊ1��,�͹ص����п������쳣,ֻʣ��NMI��ӲFault������Ӧ
			//��������ͨѶ��������Ϊ9600
            sprintf(usart1_app_buff, " \r\n", 0);
            USART1_TX((uint8_t *)usart1_app_buff);
			delay_ms(20);
			
            sprintf(usart1_app_buff, "����ʪ�ȣ�%d\r\n", humidity);
            USART1_TX((uint8_t *)usart1_app_buff);
			delay_ms(20);

            sprintf(usart1_app_buff, "�����¶ȣ�%d\r\n", temp);
            USART1_TX((uint8_t *)usart1_app_buff);
			delay_ms(20);

            sprintf(usart1_app_buff, "ʪ����ֵ��%d\r\n", humiture_data.humidity_alarm_value);
            USART1_TX((uint8_t *)usart1_app_buff);
			delay_ms(20);
				//ˮλ�������ĵ����뺬ˮ���ɷ��ȵĹ�ϵ��
            sprintf(usart1_app_buff, "��ǰˮ����%s\r\n\r\n", water_stage_value < 3500 ? "����" : "ȱˮ");
            USART1_TX((uint8_t *)usart1_app_buff);
			delay_ms(20);

            sprintf(usart1_app_buff, "��ʪ��ģʽ��%s\r\n", mode_flag == 0 ? "�Զ�" : "�ֶ�");
            USART1_TX((uint8_t *)usart1_app_buff);
			delay_ms(20);

            sprintf(usart1_app_buff, "��ʪ��״̬��%s\r\n", relay_flag == 0 ? "OFF" : "ON");
            USART1_TX((uint8_t *)usart1_app_buff);
			delay_ms(20);
			
			__set_PRIMASK(0);
			//Ĭ��ֵ��0,��ʾû�йر��жϡ�
		    time_count = 0;
        }	
			
		}
	}
}

//��ʪ���̵�����ʼ��
void relay_init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
 	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
}

//��ʪ���̵�������
void relay_set(uint8_t flag)
{
	if (flag)
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
//		GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	else
//		GPIO_SetBits(GPIOA, GPIO_Pin_8);
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);
}

//ˮλ��������ʼ��
void water_stage_init(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//PA1 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//PA2

	ADC_DeInit(ADC1);  //��λADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}

//���ADCֵ
//ch:ͨ��ֵ 0~3
uint16_t Get_Adc(uint8_t ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//Returns the last ADCx conversion result data 
}

uint16_t Get_Adc_Average(uint8_t ch, uint8_t times)
{//ͨ��2	
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);//����ת����ʪ��ֵ//����ǻ�ȡ��ADC�Ĵ��������ͨ��ֵ
		delay_ms(5);
	}
	return temp_val/times;//����ADC��ֵ�������timesʱ������Ϊת����Ҫһ����ʱ�䡣
}

//ˮλ������ֵ��ȡ
uint16_t water_stage_get(void)
{
	return Get_Adc_Average(2, 2);//����ADת����ֵ0~4095
}

//��ʪ�ȱ������ô���
void humiture_alarm_set_dispose(void)
{
	u8 key = KEY_NULL;//��ʼ�ް�������
	char str1[8] = {0};
	char humiture_str[16] = {0};
	//����ҳ�棬������page��ʪ�Ƚ���page
	if (humiture_data.page == 0)
	{
		key = key_scan();//����ɨ��
		switch(key)
		{//�������̰�������
			case KEY1_LONG_PRESS:
				
				humiture_data.page++;
				
				humiture_data.set_flag = 1;
				
				humiture_data.alarm_flag = 0;
				
				humidity_interface();//�������2
				
				break;
			//��ʪ��ģʽ��/���л�
			case KEY3_SHORT_PRESS:
				
				if (mode_flag == 1)//mode_flagģʽ��־λ���Զ�ģʽΪ1���ֶ�ģʽΪ0
				{
					relay_flag = !relay_flag;
					relay_set(relay_flag);
					if (relay_flag == 0)
					{
						OLED_Display_6x8_8x16(2, 96, 1, (const u8 *)"OFF");
					}
					else
					{
						OLED_Display_6x8_8x16(2, 96, 1, (const u8 *)"ON ");
					}
				}
				
				break;
			//�ֶ�ģʽ/�Զ�ģʽ�л�
			case KEY3_LONG_PRESS:
				//��Ҫ���ֶ�ģʽ�²��ܿ�����ʪ��
				mode_flag = !mode_flag;
				if (mode_flag == 0)
				{
					OLED_Display_16x16(2, 40, 5, Chinese); //��ʾ"��"
					OLED_Display_16x16(2, 56, 7, Chinese); //��ʾ"��"
				}
				else
				{
					OLED_Display_16x16(2, 40, 6, Chinese); //��ʾ"��"
					OLED_Display_16x16(2, 56, 7, Chinese); //��ʾ"��"
				}
				
				break;
			
			default:break;
		}
		
		/* ����APP���ã��ֻ�APP���� ��������������*/
		if (USART1_RX_BUF[0] != 0x00)
		{
			if (USART1_RX_BUF[0] == 'J')//ʪ����ֵ��
			{
				humiture_data.humidity_alarm_value++;
				if(humiture_data.humidity_alarm_value > 99)
					humiture_data.humidity_alarm_value = 99;
				//��ʪ����ֵ���ֻ�app+OLED��ʾ������ʾ
				sprintf(str1, "%02d", humiture_data.humidity_alarm_value);
				OLED_Display_6x8_8x16(4, 80, 1, (const uint8_t *)str1);
			}
			else if (USART1_RX_BUF[0] == 'K')//ʪ����ֵ��
			{
				humiture_data.humidity_alarm_value--;
				if(humiture_data.humidity_alarm_value < 1)
					humiture_data.humidity_alarm_value = 1;
				sprintf(str1, "%02d", humiture_data.humidity_alarm_value);
				OLED_Display_6x8_8x16(4, 80, 1, (const uint8_t *)str1);
			}
			else if (USART1_RX_BUF[0] == 'M')//�����л�
			{
				mode_flag = !mode_flag;
				if (mode_flag == 0)
				{
					OLED_Display_16x16(2, 40, 5, Chinese); //��ʾ"��"
					OLED_Display_16x16(2, 56, 7, Chinese); //��ʾ"��"
				}
				else
				{
					OLED_Display_16x16(2, 40, 6, Chinese); //��ʾ"��"
					OLED_Display_16x16(2, 56, 7, Chinese); //��ʾ"��"
				}
			}
			else if (USART1_RX_BUF[0] == 'S')//�ֶ�ģʽ�¿�����ʪ
			{
				if (mode_flag == 1)
				{
					relay_flag = !relay_flag;
					relay_set(relay_flag);
					if (relay_flag == 0)
					{
						OLED_Display_6x8_8x16(2, 96, 1, (const u8 *)"OFF");
					}
					else
					{
						OLED_Display_6x8_8x16(2, 96, 1, (const u8 *)"ON ");
					}
				}
			}
			
			USART1_RX_BUF[0] = 0x00;
		}
	}
	//�������page2
	else if(humiture_data.page == 1)
	{
		key = key_scan();//����ɨ��
		switch(key)
		{
			case KEY1_SHORT_PRESS:
				
				if(humiture_data.set_flag == 1)
				{
					humiture_data.humidity_alarm_value--;
					if(humiture_data.humidity_alarm_value < 1)
						humiture_data.humidity_alarm_value = 1;
					
					sprintf(humiture_str, "%02d", humiture_data.humidity_alarm_value);
					OLED_Display_6x8_8x16(2, 80, 1, (const u8 *)humiture_str);
				}
				
				break;
			
			case KEY1_LONG_PRESS:
				//������������������
				humiture_data.page--;
				
				humiture_data.show_flag = 0;
				
				humiture_data.alarm_flag = 0;
				
				OLED_Clear();
				
				delay_ms(100);
				
				main_interface();
				
				break;
			
			case KEY2_SHORT_PRESS:
				
				if(humiture_data.set_flag == 1)
				{
					humiture_data.humidity_alarm_value++;
					if(humiture_data.humidity_alarm_value > 99)
						humiture_data.humidity_alarm_value = 99;
					
					sprintf(humiture_str, "%02d", humiture_data.humidity_alarm_value);
					OLED_Display_6x8_8x16(2, 80, 1, (const u8 *)humiture_str);
				}
				
				break;
			
			default:break;
		}
	}
}

//������
void main_interface(void)
{
	char str1[8] = {0};
	//�ڶ����������壬������֮�䣬һ���ַ����16�����ص㡣
	OLED_Display_16x16(0, 0, 1, Chinese);  //��ʾ"ʪ"
	OLED_Display_16x16(0, 16, 2, Chinese); //��ʾ"��"
	OLED_Display_16x16(0, 32, 18, Chinese);//��ʾ":"
	
	OLED_Display_16x16(0, 64, 0, Chinese); //��ʾ"��"
	OLED_Display_16x16(0, 80, 2, Chinese); //��ʾ"��"
	OLED_Display_16x16(0, 96, 18, Chinese);//��ʾ":"
	
	
	
	OLED_Display_16x16(2, 0, 3, Chinese);  //��ʾ"ģ"
	OLED_Display_16x16(2, 16, 4, Chinese); //��ʾ"ʽ"
	OLED_Display_16x16(2, 32, 18, Chinese);//��ʾ":"
	OLED_Display_16x16(2, 40, 5, Chinese); //��ʾ"��"
	OLED_Display_16x16(2, 56, 7, Chinese); //��ʾ"��"
	
	OLED_Display_6x8_8x16(2, 80, 1, (const uint8_t *)"S");
	OLED_Display_16x16(2, 88, 18, Chinese);//��ʾ":"
	OLED_Display_6x8_8x16(2, 96, 1, (const u8 *)"OFF");
	
	
	OLED_Display_16x16(4, 0, 1, Chinese);  //��ʾ"ʪ"
	OLED_Display_16x16(4, 16, 2, Chinese); //��ʾ"��"
	OLED_Display_16x16(4, 32, 8, Chinese); //��ʾ"��"
	OLED_Display_16x16(4, 48, 9, Chinese); //��ʾ"ֵ"
	OLED_Display_16x16(4, 64, 18, Chinese);//��ʾ":"
	sprintf(str1, "%02d", humiture_data.humidity_alarm_value);
	OLED_Display_6x8_8x16(4, 80, 1, (const uint8_t *)str1);
	
	
	
	OLED_Display_16x16(6, 0, 10, Chinese); //��ʾ"��"
	OLED_Display_16x16(6, 16, 11, Chinese);//��ʾ"ǰ"
	OLED_Display_16x16(6, 32, 12, Chinese);//��ʾ"ˮ"
	OLED_Display_16x16(6, 48, 13, Chinese);//��ʾ"��"
	OLED_Display_16x16(6, 64, 18, Chinese);//��ʾ":"
}

//ʪ�Ƚ���
void humidity_interface(void)
{
	char humidity_str[16] = {0};
	
	humiture_data.show_flag = 1;
	
	OLED_Clear();                                                   //OLED����
	
	delay_ms(50);
	//x y 
	OLED_Display_16x16(0, 16, 1, Chinese);                          //��ʾ"ʪ"
	OLED_Display_16x16(0, 32, 2, Chinese);                          //��ʾ"��"
	OLED_Display_16x16(0, 48, 8, Chinese);                          //��ʾ"��"
	OLED_Display_16x16(0, 64, 9, Chinese);                          //��ʾ"ֵ"
	OLED_Display_16x16(0, 80, 20, Chinese);                         //��ʾ"��"
	OLED_Display_16x16(0, 96, 21, Chinese);                         //��ʾ"��"
	
	OLED_Display_16x16(2, 0, 6, Chinese);                           //��ʾ"��"
	OLED_Display_16x16(2, 16, 7, Chinese);                          //��ʾ"��"
	OLED_Display_16x16(2, 32, 4, Chinese);                          //��ʾ"ʪ"
	OLED_Display_16x16(2, 48, 5, Chinese);                          //��ʾ"��"
	OLED_Display_6x8_8x16(2, 64, 1, (const u8 *)":");               //��ʾ":"
	sprintf(humidity_str, "%02d", humiture_data.humidity_alarm_value);//����ʪ��ֵת�ַ���
	OLED_Display_6x8_8x16(2, 80, 1, (const u8 *)humidity_str);      //��ʾ����ʪ��ֵ
	
	OLED_Display_16x16(4, 0, 22, Chinese);                          //��ʾ"��"
	OLED_Display_16x16(4, 16, 23, Chinese);                         //��ʾ"��"
	OLED_Display_6x8_8x16(4, 32, 1, (const u8 *)"K1");              //��ʾ"K1"
	OLED_Display_16x16(4, 48, 24, Chinese);                         //��ʾ"��"
	OLED_Display_16x16(4, 64, 25, Chinese);                         //��ʾ"��"
	OLED_Display_16x16(4, 80, 26, Chinese);                         //��ʾ"��"
	OLED_Display_16x16(4, 96, 20, Chinese);                         //��ʾ"��"
	OLED_Display_16x16(4, 112, 21, Chinese);                        //��ʾ"��"
	
	OLED_Display_6x8_8x16(6, 0, 1, (const u8 *)"K1");               //��ʾ"K1"
	OLED_Display_16x16(6, 16, 24, Chinese);                         //��ʾ"��"
	OLED_Display_6x8_8x16(6, 32, 1, (const u8 *)":");               //��ʾ":"
	OLED_Display_6x8_8x16(6, 48, 1, (const u8 *)"-");               //��ʾ"-"
	OLED_Display_6x8_8x16(6, 64, 1, (const u8 *)"K2");              //��ʾ"K2"
	OLED_Display_16x16(6, 80, 24, Chinese);                         //��ʾ"��"
	OLED_Display_6x8_8x16(6, 96, 1, (const u8 *)":");               //��ʾ":"
	OLED_Display_6x8_8x16(6, 112, 1, (const u8 *)"+");              //��ʾ"+"
}
