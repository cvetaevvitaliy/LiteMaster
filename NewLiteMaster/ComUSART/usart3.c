#include "stm32f10x_lib.h"
#include "ring_buffer.h"

static RING_BUF usart3_recv_rgb;
static char usart3_recv_buf[128];

RING_BUF usart3_send_rgb;
static char usart3_send_buf[1024];

#define USART3_SEND_BUF_LEN 16
static const int dma5_send_buf_len=USART3_SEND_BUF_LEN;
static char dma5_send_buf[USART3_SEND_BUF_LEN];

static void init_usart3_rgb()
{
	init_ring_buf(&usart3_recv_rgb,usart3_recv_buf,128);
	init_ring_buf(&usart3_send_rgb,usart3_send_buf,1024);
}
static void NVIC_Configuration_USART3(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQChannel;	  	//外部中断6
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; 	//先占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		 	//从优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  	//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);
	
}


static void dma5_usart3_init()
{
    NVIC_InitTypeDef NVIC_InitStruct;
		DMA_InitTypeDef DMA_InitStruct;
	
		NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel2_IRQChannel;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
	
    DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,DISABLE);
    DMA_Cmd(DMA1_Channel2,DISABLE);
	
		DMA_DeInit(DMA1_Channel2);
    DMA_InitStruct.DMA_PeripheralBaseAddr = (unsigned int)(&USART3->DR);
    DMA_InitStruct.DMA_MemoryBaseAddr = (unsigned int)dma5_send_buf;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStruct.DMA_BufferSize = dma5_send_buf_len;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel2,&DMA_InitStruct);
    
        //DMA_Cmd(DMA1_Channel4,ENABLE);    
}

void uart3_init(unsigned int bound){
	
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//	NVIC_InitTypeDef NVIC_InitStructure;
	
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	USART_DeInit(USART3);
	 //USART2_TX PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//USART2_RX	PA.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);  


	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART3, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启接收中断
	//USART_ITConfig(USART3, USART_IT_TXE, ENABLE);//开启发送中断
	USART_Cmd(USART3, ENABLE);                    //使能串口 
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);	
	dma5_usart3_init();

	NVIC_Configuration_USART3();
	init_usart3_rgb();
}




static void usart3_recv()
{
	char data;
	data = USART_ReceiveData(USART3);
	push_buf(&usart3_recv_rgb,&data,1);
	USART_ClearFlag(USART3,USART_FLAG_RXNE);
	USART_ClearITPendingBit(USART3,USART_FLAG_RXNE);
}
static void USART3_SEND()
{
	int len;
	char ch;
	len = pop_buf(&usart3_send_rgb,&ch,1);
	if(len>0)
	{
		USART_SendData(USART3,ch);
	}
	else
	{
		USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
	}
	USART_ClearFlag(USART3,USART_IT_TXE);
	USART_ClearITPendingBit(USART3, USART_IT_TXE);  
}

void USART3_IRQHandler(void)                	//串口2中断服务程序
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
	{
		usart3_recv();
	}
	if(USART_GetITStatus(USART3,USART_IT_TXE))
	{ 
		USART3_SEND();
	} 
} 

static void dma5_send()
{
	int len ;
	len = pop_buf(&usart3_send_rgb,dma5_send_buf,dma5_send_buf_len);
	if(len>0)
	{
		DMA_Cmd(DMA1_Channel2,DISABLE);
		DMA_SetCurrDataCounter(DMA1_Channel2,len);
		DMA_Cmd(DMA1_Channel2,ENABLE);
		DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);
	}
	else
	{
		DMA_Cmd(DMA1_Channel2,DISABLE);
		DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,DISABLE);
	}
}
void DMA1_Channel2_IRQHandler()
{
    if(DMA_GetITStatus(DMA1_IT_TC2))
    {
        //TODO:Add code here
        DMA_ClearITPendingBit(DMA1_IT_TC2);
				dma5_send();
    }    
}

int usart3_read(char *src,int data_len)
{
	return pop_buf(&usart3_recv_rgb,src,data_len);
}

int usart3_write(char *src,int data_len)
{
#if 1
	int len;
	int flg;
	len = push_buf(&usart3_send_rgb,src,data_len);
	if(DMA_GetChanleStatu(DMA1_Channel2,DISABLE))
	{
		dma5_send();
	}
	return len;
#else
    int len;
    len = push_buf(&usart3_send_rgb,src,data_len);
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
   return len;
#endif
	
}