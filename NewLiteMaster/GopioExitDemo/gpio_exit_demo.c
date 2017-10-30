#include "stm32f10x_lib.h"
#include "sz_libc.h"

void Init_TI_KEY(void)
{
	EXTI_InitTypeDef  EXTI_InitStructure;					//����һ��EXTI�ṹ�����
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);//���ö˿�C��13����Ϊ�ж�Դ	  ��Ҫ���� ���ϱ��INT2
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0); //���ö˿�E��0����Ϊ�ж�Դ	  ��Ҫ���� ���ϱ��INT1
  	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line13|EXTI_Line4;//
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//�ж�ģʽΪ�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//�½��س���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//ʹ���ж���
	EXTI_Init(&EXTI_InitStructure);							//���ݲ�����ʼ���жϼĴ���
}
void Init_NVIC(void)
{ 
	NVIC_InitTypeDef  NVIC_InitStructure;
	 	
	#ifdef  VECT_TAB_RAM  							//���������ַѡ��

	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);  	//��0x20000000��ַ��Ϊ���������ַ(RAM)
	#else  

	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0); //��0x08000000��ַ��Ϊ���������ַ(FLASH)  
	#endif

	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	  			//�����ж����ȼ�����2

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQChannel;		//�趨�ж�ԴΪPC13
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//�ж�ռ���ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//�����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);							   	//���ݲ�����ʼ���жϼĴ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQChannel;			//�趨�ж�ԴΪPE0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//�ж�ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//�����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);							   	//���ݲ�����ʼ���жϼĴ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQChannel;			//�趨�ж�ԴΪPE0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//�ж�ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//�����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);							   	//���ݲ�����ʼ���жϼĴ���

//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQChannel;			//�趨�ж�ԴΪPE0
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//�ж�ռ���ȼ�Ϊ1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//�����ȼ�Ϊ0
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ж�
//	NVIC_Init(&NVIC_InitStructure);							   	//���ݲ�����ʼ���жϼĴ���
}
//void EXTI15_10_IRQHandler(void)
//{
//	sz_printk("EXIT15_10\n");
//	EXTI_ClearITPendingBit(EXTI_Line13);		//����жϹ���λ����Ҫ����			
//}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: EXTI0_IRQHandler
** ��������: �ж�0��ں���			
** ������������
** ��  ����: Dream
** �ա�  ��: 2011��6��20��
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void EXTI0_IRQHandler(void)
{
	sz_printk("EXIT0\n");
	EXTI_ClearITPendingBit(EXTI_Line0);		     //����жϹ���λ����Ҫ����			
}

void EXTI4_IRQHandler(void)
{
	sz_printk("EXIT4\n");
	EXTI_ClearITPendingBit(EXTI_Line4);	
}
void gpio_exit_test()
{
	Init_NVIC();
	Init_TI_KEY();
}