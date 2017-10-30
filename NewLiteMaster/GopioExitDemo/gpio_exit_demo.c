#include "stm32f10x_lib.h"
#include "sz_libc.h"

void Init_TI_KEY(void)
{
	EXTI_InitTypeDef  EXTI_InitStructure;					//定义一个EXTI结构体变量
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);//配置端口C的13引脚为中断源	  重要！！ 板上标号INT2
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0); //配置端口E的0引脚为中断源	  重要！！ 板上标号INT1
  	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line13|EXTI_Line4;//
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//中断模式为中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//下降沿出发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//使能中断线
	EXTI_Init(&EXTI_InitStructure);							//根据参数初始化中断寄存器
}
void Init_NVIC(void)
{ 
	NVIC_InitTypeDef  NVIC_InitStructure;
	 	
	#ifdef  VECT_TAB_RAM  							//向量表基地址选择

	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);  	//将0x20000000地址作为向量表基地址(RAM)
	#else  

	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0); //将0x08000000地址作为向量表基地址(FLASH)  
	#endif

	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	  			//设置中断优先级分组2

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQChannel;		//设定中断源为PC13
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//中断占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//副优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能中断
	NVIC_Init(&NVIC_InitStructure);							   	//根据参数初始化中断寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQChannel;			//设定中断源为PE0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//中断占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//副优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能中断
	NVIC_Init(&NVIC_InitStructure);							   	//根据参数初始化中断寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQChannel;			//设定中断源为PE0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//中断占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//副优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能中断
	NVIC_Init(&NVIC_InitStructure);							   	//根据参数初始化中断寄存器

//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQChannel;			//设定中断源为PE0
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//中断占优先级为1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//副优先级为0
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能中断
//	NVIC_Init(&NVIC_InitStructure);							   	//根据参数初始化中断寄存器
}
//void EXTI15_10_IRQHandler(void)
//{
//	sz_printk("EXIT15_10\n");
//	EXTI_ClearITPendingBit(EXTI_Line13);		//清楚中断挂起位，重要！！			
//}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: EXTI0_IRQHandler
** 功能描述: 中断0入口函数			
** 参数描述：无
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void EXTI0_IRQHandler(void)
{
	sz_printk("EXIT0\n");
	EXTI_ClearITPendingBit(EXTI_Line0);		     //清楚中断挂起位，重要！！			
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