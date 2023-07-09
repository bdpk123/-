#ifndef __humiture_data_H
#define __humiture_data_H

#include "delay.h"
#include "sys.h"

/* ��ʪ�����ݽṹ�� */
typedef struct
{
	int temp_alarm_value;    //�¶ȱ���ֵ
	int humidity_alarm_value;//ʪ�ȱ���ֵ
	u8 page;                 //ҳ�棬�������棬�������ʪ�ȿ��ƽ���
	u8 show_flag;            //��ʾ��־
	u8 alarm_flag;           //������־
	u8 temp_flag;            //�¶ȱ�־
	u8 humidity_flag;        //ʪ�ȱ�־
	u8 set_flag;             //���ñ�־ 
	u32 time2_count;         //��ʱ��2����
	u32 time3_count;         //��ʱ��3����
	u32 time3_count1;        //��ʱ��3����1
	u32 alarm_count;         //��������
}humitureDataTypeDef;

extern humitureDataTypeDef humiture_data;

#endif
