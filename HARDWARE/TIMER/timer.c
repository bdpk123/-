#include "timer.h"

u32 TIMx_Count=0;//TIMx����ֵ

//TIMx��ʼ��
//TIMx:TIM1~TIM4
//Period(����):1us~6553000us,Tout(us)=((Arr+1)*(Psc+1))/Tclk(MHz)
//Seize_Priority:��ռ���ȼ�
//Son_Priority:�����ȼ�
void TIMx_Init(TIM_TypeDef* TIMx,u32 Period,uint8_t Seize_Priority,uint8_t Son_Priority)
{
	uint16_t Arr,Psc;
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	if(Period==1)                                                         //1us
	{
		Psc=35;                                                           //TIMxʱ��Ƶ�ʳ���Ԥ��Ƶֵ
		Arr=36;                                                           //�Զ���װ�ؼĴ�������ֵ
	}
	else if(Period>=2 && Period<=65535)                                   //2us~65535us
	{
		Psc=71;                                                           //TIMxʱ��Ƶ�ʳ���Ԥ��Ƶֵ
		Arr=(72/(Psc+1))*Period;                                          //�Զ���װ�ؼĴ�������ֵ
	}
	else if(Period>65535 && Period<=6553000)                              //65535us~6553000us
	{
		Psc=7199;                                                         //TIMxʱ��Ƶ�ʳ���Ԥ��Ƶֵ
		Period/=1000;                                                     //ת��Ϊms
		Arr=(72000/(Psc+1))*Period;                                       //�Զ���װ�ؼĴ�������ֵ
	}
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);                   //ʹ��AFIOʱ��(ʹ�ܹ��ܸ���IO)
	
	if(TIMx==TIM1)                                                        //ͨ�ö�ʱ��1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);               //ʹ��TIM1ʱ��
	else if(TIMx==TIM2)                                                   //ͨ�ö�ʱ��2
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);               //ʹ��TIM2ʱ��
	else if(TIMx==TIM3)                                                   //ͨ�ö�ʱ��3
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);               //ʹ��TIM3ʱ��
	else if(TIMx==TIM4)                                                   //ͨ�ö�ʱ��4
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);               //ʹ��TIM4ʱ��
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);               //JTAG-DPʧ��+SW-DPʹ��
	
	TIM_TimeBaseStructure.TIM_Period = (uint16_t)Arr-1;	                  //�Զ���װ�ؼĴ�������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)Psc;                  //TIMxʱ��Ƶ�ʳ���Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;               //����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;           //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);
	
	if(TIMx==TIM1)
		NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;                //TIM1�ж�
	else if(TIMx==TIM2)
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;                   //TIM2�ж�
	else if(TIMx==TIM3)
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;                   //TIM3�ж�
	else if(TIMx==TIM4)
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;                   //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Seize_Priority;//��ռ���ȼ�Seize_Priority
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = Son_Priority;         //�����ȼ�Son_Priority
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                       //NVICʹ��
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearFlag(TIMx,TIM_FLAG_Update);                                  //��������ж�
	TIM_ITConfig(TIMx,TIM_IT_Update,DISABLE);                             //ʧ��TIMx�����ж�
	TIM_Cmd(TIMx,DISABLE);                                                //ʧ��TIMx����
}
//��TIMx
//TIMx:TIM1~TIM8
void ON_TIMx(TIM_TypeDef* TIMx)
{
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);//ʹ��TIMx�����ж�
	TIM_Cmd(TIMx,ENABLE);                   //ʹ��TIMx����
}
//�ر�TIMx
//TIMx:TIM1~TIM8
void OFF_TIMx(TIM_TypeDef* TIMx)
{
	TIM_ITConfig(TIMx,TIM_IT_Update,DISABLE);//ʧ��TIMx�����ж�
	TIM_Cmd(TIMx,DISABLE);                   //ʧ��TIMx����
}
#if TIM1_UP_IRQ==1
//TIM1�ж�������
void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);//���TIM1�жϱ�־
	}
}
#endif
#if TIM2_IRQ==1
//TIM2�ж�������
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//���TIM2�жϱ�־
		
		humiture_data.time2_count++;
	}
}
#endif
#if TIM3_IRQ==1
//TIM3�ж�������
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//���TIM3�жϱ�־
		
		humiture_data.time3_count1++;
		
		if (humiture_data.alarm_flag == 1)
		{
			humiture_data.time3_count++;
			if(humiture_data.time3_count >= 450)
			{
				humiture_data.time3_count = 0;
				
				humiture_data.alarm_count++;
				if(humiture_data.alarm_count % 2)
					buzzer_set(0);
				else
					buzzer_set(1);
			}
			if(humiture_data.alarm_count >= 2)
			{
				humiture_data.time3_count = 0;
				humiture_data.alarm_count = 0;
				humiture_data.alarm_flag = 0;
				
				/* �رշ����� */
				buzzer_set(1);
				
//				/* ֹͣ��ʱ��3 */
//				OFF_TIMx(TIM3);
			}
		}
//		else
//			buzzer_set(1);
	}
}
#endif
#if TIM4_IRQ==1
//TIM4�ж�������
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//���TIM4�жϱ�־
		
	}
}
#endif
