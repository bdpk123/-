#ifndef __timer_H
#define __timer_H

#include "buzzer.h"
#include "humiture_data.h"
#include "sys.h"

#define TIM1_UP_IRQ 0//ʹ��(1)��ʧ��(0)��ʱ��1�ж��������������ж�ʱ���ȴ����жϡ�
#define TIM2_IRQ    1//ʹ��(1)��ʧ��(0)��ʱ��2�ж�������
#define TIM3_IRQ    1//ʹ��(1)��ʧ��(0)��ʱ��3�ж�������
#define TIM4_IRQ    0//ʹ��(1)��ʧ��(0)��ʱ��4�ж�������

extern u32 TIMx_Count;//TIMx����ֵ

void TIMx_Init(TIM_TypeDef* TIMx,u32 Period,uint8_t Seize_Priority,uint8_t Son_Priority);//TIMx��ʼ��
void ON_TIMx(TIM_TypeDef* TIMx);                                                         //��TIMx
void OFF_TIMx(TIM_TypeDef* TIMx);                                                        //�ر�TIMx

#endif
