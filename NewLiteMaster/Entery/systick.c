#include "stm32f10x_lib.h"
#include "delay_work.h"
#include "led.h"
#include "key.h"

static unsigned long systick = 0;


unsigned long get_systick()
{
	return systick;
}
void systick_delay(unsigned long cnt)
{
	unsigned long fist = systick;
	while((systick-fist)< cnt);
}

void SysTick_Handler(void)
{				
	static int loc_sys_tick = 0;
	systick++;
	dlw_clk();
	led_clk();
	if(loc_sys_tick<10)
	{
		loc_sys_tick++;
		return ;
	}
	loc_sys_tick = 0;
	key_clk();
}

void systick_init(int ms)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//64M/8=8MHZ
	SysTick_SetReload(8000*ms);//8000 for 1ms³å¶ÏÒ»´Î
	SysTick_ITConfig(ENABLE);
	SysTick_CounterCmd(SysTick_Counter_Enable);    
}

