#ifndef __KEY_H
#define __KEY_H

#include "sys.h"
#include "timer.h"
#include "humiture_data.h"

#define KEY1_PIN PBin(3)
#define KEY2_PIN PBin(4)
#define KEY3_PIN PCin(13)

#define KEY_NULL         0//�ް�������
#define KEY1_SHORT_PRESS 1//����1�̰�
#define KEY1_LONG_PRESS  2//����1����
#define KEY2_SHORT_PRESS 3//����2�̰�
#define KEY2_LONG_PRESS  4//����2����
#define KEY3_SHORT_PRESS 5//����3�̰�
#define KEY3_LONG_PRESS  6//����3����

void key_init(void);//������ʼ��
u8 key_scan(void);  //����ɨ��

#endif
