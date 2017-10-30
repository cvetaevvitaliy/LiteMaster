#include "stm32f10x_lib.h"
//#define _USE_HSE
void SystemFirstInit(void)
{
    /* 定义枚举类型变量 HSEStartUpStatus */
    ErrorStatus HSEStartUpStatus;
    
    /* 复位系统时钟设置*/
    RCC_DeInit();
#if defined(_USE_HSE)
    /* 开启HSE*/
    RCC_HSEConfig(RCC_HSE_ON);
    /* 等待HSE起振并稳定*/
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    /* 判断HSE起是否振成功，是则进入if()内部 */
    while(HSEStartUpStatus != SUCCESS);
		/* 选择HCLK（AHB）时钟源为SYSCLK 1分频 */
#else	
		RCC_HSICmd(ENABLE); 
		while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY)== RESET);/* 等待HSI起振并稳定*/
#endif
	
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
		/* 选择PCLK2时钟源为 HCLK（AHB） 1分频 */
		RCC_PCLK2Config(RCC_HCLK_Div1); 
		/* 选择PCLK1时钟源为 HCLK（AHB） 2分频 */
		RCC_PCLK1Config(RCC_HCLK_Div2);
		/* 设置FLASH延时周期数为2 */
		FLASH_SetLatency(FLASH_Latency_2);
		/* 使能FLASH预取缓存 */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
#if defined(_USE_HSE)
		/* 选择锁相环（PLL）时钟源为HSE 1分频，倍频数为9，则PLL输出频率为 8MHz * 9 = 72MHz */
		//RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
#else
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);//8/2*16=64M
#endif
		/* 使能PLL */ 
		RCC_PLLCmd(ENABLE);
		/* 等待PLL输出稳定 */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		/* 选择SYSCLK时钟源为PLL */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		/* 等待PLL成为SYSCLK时钟源 */
	//  0x00:HSI作为时钟源
	//  0x04:HSE作为时钟源
	//  0x08:PLL作为时钟源
		while(RCC_GetSYSCLKSource() != 0x08);
}

void SystemInit(void)
{

    ErrorStatus HSEStartUpStatus;
    
   
    RCC_DeInit();
  
    RCC_HSEConfig(RCC_HSE_ON);
    
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
  
    while(HSEStartUpStatus != SUCCESS);
		
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
	
		RCC_PCLK2Config(RCC_HCLK_Div1); 
	
		RCC_PCLK1Config(RCC_HCLK_Div2);
		
		FLASH_SetLatency(FLASH_Latency_2);
		
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
 
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	
		RCC_PLLCmd(ENABLE);
		
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		while(RCC_GetSYSCLKSource() != 0x08);
}

void SystemSecondInit()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(
		RCC_APB2Periph_GPIOA |
		RCC_APB2Periph_GPIOB |
		RCC_APB2Periph_GPIOC | 
		RCC_APB2Periph_GPIOD | 
		RCC_APB2Periph_GPIOE | 
		RCC_APB2Periph_GPIOG |
		RCC_APB2Periph_AFIO    //使能IO复用功能，使用中断功能重要！！！
		, ENABLE);
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//使能USART1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
}
