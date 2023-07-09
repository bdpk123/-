#include "key.h"

//������ʼ��
void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//����ɨ��
u8 key_scan(void)
{
	static u8 Flag = 1;
	if(Flag==1)
	{
		if(KEY1_PIN==0)
		{//����1����
			humiture_data.time2_count = 0;//���ö�ʱ���ĳ�ʼֵ
			ON_TIMx(TIM2);//������ʱ�����м�ʱ
			Flag = 0;
			while((KEY1_PIN==0) && (humiture_data.time2_count<120));//�ȴ������ͷ�
			if(humiture_data.time2_count>=60)//����600msΪ����
			{
				humiture_data.time2_count = 0;
				OFF_TIMx(TIM2);//�رն�ʱ��
				return KEY1_LONG_PRESS;//����
			}
			else
			{
				humiture_data.time2_count = 0;
				OFF_TIMx(TIM2);
				return KEY1_SHORT_PRESS;//�̰�
			}
		}
		if(KEY2_PIN==0)
		{
			humiture_data.time2_count = 0;//���ö�ʱ���ĳ�ʼֵ
			ON_TIMx(TIM2);
			Flag = 0;
			while((KEY2_PIN==0) && (humiture_data.time2_count<120));//�ȴ������ͷ�
			if(humiture_data.time2_count>=60)//����600msΪ����
			{
				humiture_data.time2_count = 0;
				OFF_TIMx(TIM2);
				return KEY2_LONG_PRESS;
			}
			else
			{
				humiture_data.time2_count = 0;
				OFF_TIMx(TIM2);
				return KEY2_SHORT_PRESS;
			}
		}
		if(KEY3_PIN==0)
		{
			humiture_data.time2_count = 0;
			ON_TIMx(TIM2);
			Flag = 0;
			while((KEY3_PIN==0) && (humiture_data.time2_count<120));//�ȴ������ͷ�
			if(humiture_data.time2_count>=60)//����600msΪ����
			{
				humiture_data.time2_count = 0;
				OFF_TIMx(TIM2);
				return KEY3_LONG_PRESS;
			}
			else
			{
				humiture_data.time2_count = 0;
				OFF_TIMx(TIM2);
				return KEY3_SHORT_PRESS;
			}
		}
	}
	if((KEY1_PIN==1) && (KEY2_PIN==1) && (KEY3_PIN==1))
	{
		humiture_data.time2_count = 0;
		OFF_TIMx(TIM2);
		Flag = 1;
	}
	return KEY_NULL;//�����޷�Ӧ
}
