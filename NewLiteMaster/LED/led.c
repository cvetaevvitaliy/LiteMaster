#include "stm32f10x_lib.h"
#include "led.h"
enum
{
	FLASH_OPEN = 0,
	FLASH_CLOSE = 1,
};
typedef struct _STR_LED
{
	unsigned int start_miliseconds;
	unsigned int state;
	unsigned int old_state;
	unsigned int substate;
	unsigned int open_miliseconds;
	unsigned int close_miliseconds;
	GPIO_TypeDef *pStrGPIO;
	unsigned short GPIO_PPIN;
	int route_level;//when the led open level
}STR_LED;

struct st_led_md_t
{
	STR_LED *p_led;
	int cnt;
};
static STR_LED led[5];
static struct st_led_md_t md_led={&led[0],5};
static struct st_led_md_t *p_md_led = &md_led;

static unsigned long led_get_tick();
static void led_open(STR_LED *led)
{
	//led->pStrGPIO->BSRR = led->GPIO_PPIN;
	//HAL_GPIO_WritePin(led->pStrGPIO,led->GPIO_PPIN,GPIO_PIN_SET);
	if(led->route_level!=0)
	    led->pStrGPIO->BSRR = led->GPIO_PPIN;
	else
		led->pStrGPIO->BRR = led->GPIO_PPIN;
}

static void led_close(STR_LED *led)
{
	if(led->route_level==0)
	    led->pStrGPIO->BSRR = led->GPIO_PPIN;
	else
		led->pStrGPIO->BRR = led->GPIO_PPIN;
}

static void LED_DO(STR_LED *led)
{
	unsigned long cur_tick;
	switch(led->state)
	{
		case ALLAY_OPEN:
				led_open(led);
				led->state = LED_IDLE;
				led->substate = FLASH_OPEN;
				break;
		case ALLAY_CLOSE:
				led_close(led);
				led->state = LED_IDLE;
				led->substate = FLASH_CLOSE;
				break;
		case ALLAY_FLASH:			
			switch(led->substate)
			{
				case FLASH_OPEN:
					cur_tick = led_get_tick();
					if((cur_tick-led->start_miliseconds >led->open_miliseconds))
					{
						led->start_miliseconds = cur_tick;
						led_open(led);
						led->substate = FLASH_CLOSE;
					}
					break;
				case FLASH_CLOSE:
					cur_tick = led_get_tick();
					if((cur_tick-led->start_miliseconds >led->close_miliseconds))
					{
						led->start_miliseconds = cur_tick;
						led_close(led);
						led->substate = FLASH_OPEN;
					}
				default:
					break;
			}
			break;
		case ONE_FLASH_ON:
			switch(led->substate)
			{
				case FLASH_OPEN:
					cur_tick = led_get_tick();
					if((cur_tick-led->start_miliseconds >1500))
					{
						led->start_miliseconds = cur_tick;
						led_open(led);
						led->substate = FLASH_CLOSE;
					}
					break;
				case FLASH_CLOSE:
					cur_tick = led_get_tick();
					if((cur_tick-led->start_miliseconds >50))
					{
						led->start_miliseconds = cur_tick;
						led_close(led);
						led->substate = FLASH_OPEN;
						led->state = led->old_state;
						//sz_printk("led_new_sate:%d\n",	led->state );
					}
					break;
				default:
					break;
			}
		    break;
		default:
			break;
	}
}



void ShowLeds()
{
	int i;
	int cnt = p_md_led->cnt;
	for(i=0;i<cnt;i++)
	    LED_DO(&p_md_led->p_led[i]);
}


void SetLedState(int which,int state)
{
	STR_LED *pled;
	if(which<0 || which>5)
		return;


	if(p_md_led->p_led[which].state == ONE_FLASH_ON)
			return;
	if(state == ONE_FLASH_ON)
	{
		if(p_md_led->p_led[which].state == LED_IDLE)
	    {
		    if(p_md_led->p_led[which].substate==FLASH_OPEN)
		    {
				p_md_led->p_led[which].old_state = ALLAY_OPEN;
		    }
			else
			{
				p_md_led->p_led[which].old_state = ALLAY_CLOSE;
			}
		}
		else
		{
			p_md_led->p_led[which].old_state = p_md_led->p_led[which].state;
		}
		p_md_led->p_led[which].substate=FLASH_OPEN;
		led_close(& p_md_led->p_led[which]);
		//sz_printk("led_old_sate:%d\n",	pled->old_state);
	}
	
	 p_md_led->p_led[which].state = state;
	 p_md_led->p_led[which].start_miliseconds = led_get_tick();
}

//void SetLedFlashTime(int which,int miliseconds)
//{
//	switch(which)
//	{
//		case 0:
//			led0.open_miliseconds = miliseconds;
//			break;
//
//		default:
//			break;
//	}
//}

static unsigned long led_tick = 0;
static unsigned long max_led_tick = 0x7fffffff;
static void led_time_calculate(unsigned int time)
{
	led_tick -= time;
	//led0.start_miliseconds -=time;
}


void led_clk()
{
	led_tick++;
	if(led_tick>max_led_tick)
	{
		led_time_calculate(max_led_tick);
	}
		
}

static unsigned long led_get_tick()
{
	return led_tick;
}


void led_gpio_init()

{
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;			
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7,Bit_RESET);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_10;			
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
	 GPIO_WriteBit(GPIOC,GPIO_Pin_4|GPIO_Pin_10,Bit_RESET);

	 p_md_led->p_led[0].pStrGPIO = GPIOC;
	 p_md_led->p_led[0].GPIO_PPIN = GPIO_Pin_10;
	 p_md_led->p_led[0].state=ALLAY_FLASH;
	p_md_led->p_led[0].substate = FLASH_CLOSE;
	 p_md_led->p_led[0].route_level = 1;
	 p_md_led->p_led[0].open_miliseconds = 1500;
	 p_md_led->p_led[0].close_miliseconds=50;

	 p_md_led->p_led[1].pStrGPIO = GPIOA;
	 p_md_led->p_led[1].GPIO_PPIN = GPIO_Pin_4;
	 p_md_led->p_led[1].state=LED_IDLE;
	 p_md_led->p_led[1].substate = FLASH_CLOSE;
	 p_md_led->p_led[1].route_level = 1;
	 p_md_led->p_led[1].open_miliseconds = 1500;
	 p_md_led->p_led[1].close_miliseconds=50;

	 p_md_led->p_led[2].pStrGPIO = GPIOA;
	 p_md_led->p_led[2].GPIO_PPIN = GPIO_Pin_5;
	 p_md_led->p_led[2].state=LED_IDLE;
	 p_md_led->p_led[2].substate = FLASH_CLOSE;
	 p_md_led->p_led[2].route_level = 1;
	 p_md_led->p_led[2].open_miliseconds = 1500;
	 p_md_led->p_led[2].close_miliseconds=50;

	 p_md_led->p_led[3].pStrGPIO = GPIOA;
	 p_md_led->p_led[3].GPIO_PPIN = GPIO_Pin_7;
	 p_md_led->p_led[3].state=LED_IDLE;
	 p_md_led->p_led[3].substate = FLASH_CLOSE;
	 p_md_led->p_led[3].route_level = 1;
	 p_md_led->p_led[3].open_miliseconds = 1500;
	 p_md_led->p_led[3].close_miliseconds=50;

	 p_md_led->p_led[4].pStrGPIO = GPIOC;
	 p_md_led->p_led[4].GPIO_PPIN = GPIO_Pin_4;
	 p_md_led->p_led[4].state=LED_IDLE;
	 p_md_led->p_led[4].substate = FLASH_CLOSE;
	 p_md_led->p_led[4].route_level = 1;
	 p_md_led->p_led[4].open_miliseconds = 1500;
	 p_md_led->p_led[4].close_miliseconds=50;
	 
}
