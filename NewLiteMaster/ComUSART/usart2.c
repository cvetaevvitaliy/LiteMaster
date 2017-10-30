#include "stm32f10x_lib.h"
#include "usart2.h"	
#include "ring_buffer.h"

static RING_BUF usart2_recv_rgb;
static char usart2_recv_buf[128];

RING_BUF usart2_send_rgb;
static char usart2_send_buf[1024];

#define USART1_SEND_BUF_LEN 64
static const int dma1_chanel7_send_buf_len=USART1_SEND_BUF_LEN;
static char  dma1_chanel7_send_buf[USART1_SEND_BUF_LEN];

static void init_rgb()
{
	init_ring_buf(&usart2_recv_rgb,usart2_recv_buf,128);
	init_ring_buf(&usart2_send_rgb,usart2_send_buf,1024);
}
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;	  	//外部中断6
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 	//先占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		 	//从优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  	//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);
	
}


static void dma1_chanle7_usart2_init()
{
    NVIC_InitTypeDef NVIC_InitStruct;
		DMA_InitTypeDef DMA_InitStruct;
	
		NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel7_IRQChannel;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
	
    DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,DISABLE);
    DMA_Cmd(DMA1_Channel7,DISABLE);
	
		DMA_DeInit(DMA1_Channel7);
    DMA_InitStruct.DMA_PeripheralBaseAddr = (unsigned int)(&USART2->DR);
    DMA_InitStruct.DMA_MemoryBaseAddr = (unsigned int)dma1_chanel7_send_buf;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStruct.DMA_BufferSize = dma1_chanel7_send_buf_len;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel7,&DMA_InitStruct);
    
    //DMA_Cmd(DMA1_Channel4,ENABLE);    
}

void uart2_init(unsigned int bound){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_Configuration();
	init_rgb();
	 //USART1_TX   PA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART1_RX	  PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  


	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART2, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启接收中断
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//开启发送中断
	USART_Cmd(USART2, ENABLE);                    //使能串口 
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
	dma1_chanle7_usart2_init();
}


static void dma1_chanel7_send()
{
	int len ;
	len = pop_buf(&usart2_send_rgb,dma1_chanel7_send_buf,dma1_chanel7_send_buf_len);
	if(len>0)
	{
		DMA_Cmd(DMA1_Channel7,DISABLE);
		DMA_SetCurrDataCounter(DMA1_Channel7,len);
		DMA_Cmd(DMA1_Channel7,ENABLE);
		DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE);
	}
	else
	{
		DMA_Cmd(DMA1_Channel7,DISABLE);
		DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,DISABLE);
	}
}

static void USART2_RECV()
{
	char data;
	data = USART_ReceiveData(USART2);
	push_buf(&usart2_recv_rgb,&data,1);
	USART_ClearFlag(USART2,USART_FLAG_RXNE);
	USART_ClearITPendingBit(USART2,USART_FLAG_RXNE);
}

static void USART2_SEND()
{
	int len;
	char ch;
	len = pop_buf(&usart2_send_rgb,&ch,1);
	if(len>0)
	{
		USART_SendData(USART2,ch);
	}
	else
	{
		USART_ITConfig(USART2, USART_IT_TXE, DISABLE);//关闭启发送中断
	}
	USART_ClearFlag(USART2,USART_IT_TXE);
	USART_ClearITPendingBit(USART2, USART_IT_TXE);  
}
void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
	{
		USART2_RECV();
	}
	if(USART_GetITStatus(USART2,USART_IT_TXE))
	{ 
		USART2_SEND();
	}  
	
} 

void DMA1_Channel7_IRQHandler()
{
    if(DMA_GetITStatus(DMA1_IT_TC7))
    {
        //TODO:Add code here
        DMA_ClearITPendingBit(DMA1_IT_TC7);
				dma1_chanel7_send();
    }    
}

int usart2_read(char *src,int data_len)
{
	return pop_buf(&usart2_recv_rgb,src,data_len);
}

int usart2_write(char *src,int data_len)
{
	int len;
	int flg;
	flg = is_buf_empty(&usart2_send_rgb);
	len = push_buf(&usart2_send_rgb,src,data_len);
	//sz_printf(src);
	if(DMA_GetChanleStatu(DMA1_Channel7,DISABLE))
	{
		dma1_chanel7_send();
	}
//    if(flg)
//			USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//开启发送中断
	return len;
}