#include "stm32f10x_lib.h"
//#define _USE_HSE
void SystemFirstInit(void)
{
    /* ����ö�����ͱ��� HSEStartUpStatus */
    ErrorStatus HSEStartUpStatus;
    
    /* ��λϵͳʱ������*/
    RCC_DeInit();
#if defined(_USE_HSE)
    /* ����HSE*/
    RCC_HSEConfig(RCC_HSE_ON);
    /* �ȴ�HSE�����ȶ�*/
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    /* �ж�HSE���Ƿ���ɹ����������if()�ڲ� */
    while(HSEStartUpStatus != SUCCESS);
		/* ѡ��HCLK��AHB��ʱ��ԴΪSYSCLK 1��Ƶ */
#else	
		RCC_HSICmd(ENABLE); 
		while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY)== RESET);/* �ȴ�HSI�����ȶ�*/
#endif
	
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
		/* ѡ��PCLK2ʱ��ԴΪ HCLK��AHB�� 1��Ƶ */
		RCC_PCLK2Config(RCC_HCLK_Div1); 
		/* ѡ��PCLK1ʱ��ԴΪ HCLK��AHB�� 2��Ƶ */
		RCC_PCLK1Config(RCC_HCLK_Div2);
		/* ����FLASH��ʱ������Ϊ2 */
		FLASH_SetLatency(FLASH_Latency_2);
		/* ʹ��FLASHԤȡ���� */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
#if defined(_USE_HSE)
		/* ѡ�����໷��PLL��ʱ��ԴΪHSE 1��Ƶ����Ƶ��Ϊ9����PLL���Ƶ��Ϊ 8MHz * 9 = 72MHz */
		//RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
#else
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);//8/2*16=64M
#endif
		/* ʹ��PLL */ 
		RCC_PLLCmd(ENABLE);
		/* �ȴ�PLL����ȶ� */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		/* ѡ��SYSCLKʱ��ԴΪPLL */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		/* �ȴ�PLL��ΪSYSCLKʱ��Դ */
	//  0x00:HSI��Ϊʱ��Դ
	//  0x04:HSE��Ϊʱ��Դ
	//  0x08:PLL��Ϊʱ��Դ
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
		RCC_APB2Periph_AFIO    //ʹ��IO���ù��ܣ�ʹ���жϹ�����Ҫ������
		, ENABLE);
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//ʹ��USART1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
}
