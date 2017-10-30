#include "stm32f10x_lib.h"
#include "cc110x_adapter.h"
#include "spi_com.h"
#include "systick.h"
#include "sz_libc.h"
static struct SPI_t cc110x_s;
static struct SPI_pin_t cc110x_pins[4];
static void cc110x_delay(int delay)
{
	//while(delay--)
		//__nop();
}

#define HAL_Delay(cnt) systick_delay(cnt)

static EXTI_InitTypeDef EXTI_InitStructure;
static void CC1101_EXIT_Config()
{
	//EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11);	
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	EXTI_ClearITPendingBit(EXTI_Line11);	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	//EXTI_Init(&EXTI_InitStructure);	 

  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
}

void CC1101_irq_disable()
{
	EXTI_ClearITPendingBit(EXTI_Line11);
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
}
void CC1101_irq_enable()
{
	EXTI_ClearITPendingBit(EXTI_Line11);
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

static void cc100x_pin_init()
{
    GPIO_InitTypeDef  GPIO_InitStruct;
	//CS MOSI CLK
	GPIO_InitStruct.GPIO_Pin = (GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct); 
	
	GPIO_WriteBit(GPIOB,( GPIO_Pin_12 ),Bit_SET);
	GPIO_WriteBit(GPIOB,( GPIO_Pin_13 | GPIO_Pin_15),Bit_RESET);

	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct); 


    //GDO0-PA11 GDO2-PA10
    GPIO_InitStruct.GPIO_Pin = (GPIO_Pin_10|GPIO_Pin_11);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz  ;
    GPIO_Init(GPIOA, &GPIO_InitStruct); 
	
	//Tx_EN-PB0 Rx_EN-PB1
    GPIO_InitStruct.GPIO_Pin = (GPIO_Pin_0|GPIO_Pin_1);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz  ;
    GPIO_Init(GPIOB, &GPIO_InitStruct); 
    GPIO_WriteBit(GPIOB,( GPIO_Pin_0 | GPIO_Pin_1),Bit_RESET);	
}

void cc110x_init()
{
	cc110x_s.data_size   = DATA_SZIE_8;
	cc110x_s.first       = MSB;
	cc110x_s.chip_select = spi_chip_select;//GPIO4   0
	cc110x_s.clk         = spi_clk;//GPIO5    1
	cc110x_s.miso        = spi_miso;//GPIO6   2
	cc110x_s.mosi        = spi_mosi;//GPIO7   3
	cc110x_s.delay       = cc110x_delay;
	cc110x_s.priv        = (void*)&cc110x_pins[0];

    cc110x_s.clk_all     = 3;
	cc110x_s.clk_half    = cc110x_s.clk_all/2;
	cc110x_s.filter_s    = cc110x_s.clk_all/10;
	cc110x_s.clk_s       = cc110x_s.clk_half - cc110x_s.filter_s;
	//spi_cs
	cc110x_pins[0].pStrGPIO = GPIOB;
	cc110x_pins[0].GPIO_PIN = GPIO_Pin_12;
	
	//spi_clk
	cc110x_pins[1].pStrGPIO = GPIOB;
	cc110x_pins[1].GPIO_PIN = GPIO_Pin_13;
	
	//MISO
	cc110x_pins[2].pStrGPIO = GPIOB;
	cc110x_pins[2].GPIO_PIN = GPIO_Pin_14;
	
	//MOSI
	cc110x_pins[3].pStrGPIO = GPIOB;
	cc110x_pins[3].GPIO_PIN = GPIO_Pin_15;
	cc100x_pin_init();
	CC1101_EXIT_Config();

	/*input your init code here */
}

int cc110x_read(int regaddr,char *src,int len)
{
	regaddr =regaddr&0x3F;
	regaddr = regaddr|0xC0;
	return ll_spi_read(&cc110x_s,regaddr,src,len,0);
}

int cc110x_write(int regaddr,const char *src,int len)
{
	regaddr =regaddr&0x3F;
	regaddr =regaddr|0x40;
	return ll_spi_write(&cc110x_s,regaddr,src,len,0);
}

int cc110x_read_t(int regaddr)
{
	regaddr = regaddr &0x3F;
	regaddr = regaddr | 0x80;
	return ll_read_t(&cc110x_s,regaddr,0);
}

int cc110x_write_t(int regaddr,int value)
{
	regaddr = regaddr &0x3F;
	return ll_write_t(&cc110x_s,regaddr,value,0);
}

int cc110x_read_status(int regaddr)
{
	regaddr = regaddr & 0x3F;
	regaddr = regaddr | 0xC0;
	return ll_read_t(&cc110x_s,regaddr,0);
}

int cc110x_write_strobe(int regaddr)
{
	
return ll_write_strobe(&cc110x_s,regaddr,0);
}


int cc110x_write_sp_cmd(int cmd)
{
	return ll_write_sp_cmd(&cc110x_s,cmd,0);
}

void cc110x_tx_enable()
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);
}

void cc110x_tx_disable()
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);
}

void cc110x_rx_enable()
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET);
}

void cc110x_rx_disable()
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_RESET);
}

int cc110x_reset(int cmd)
{
	ll_chip_select(&cc110x_s,1);//0 for enable 
	HAL_Delay(1);
	ll_chip_select(&cc110x_s,0);
	HAL_Delay(1);
	ll_chip_select(&cc110x_s,1);
	HAL_Delay(5);
	ll_write_sp_cmd(&cc110x_s,cmd,0);
	return 0;
}
