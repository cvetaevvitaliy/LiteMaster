#include "stm32f10x_lib.h"
#include "key.h"

typedef struct _KEY
{
	unsigned int start_milisecond;
	unsigned int wave_milisecond;
	unsigned int short_press_milisecond;
	int flag;
	int (*read_key)(void);
	unsigned int new_pos;
}KEY;

static void key_reset_tick(unsigned long);
static unsigned long key_tick = 0;
static unsigned long key_tick_max = 0x7fffffff;
void key_clk()
{
	key_tick++;
	if(key_tick >= key_tick_max)
	{
		key_reset_tick(key_tick_max);
	}
}

unsigned long get_key_clk()
{
	return key_tick;
}

static int K1() 
{
	return (int)GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5);
}
//static int K2() 
//{
//	return (int)HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11);
//}
//static int K3() 
//{
//	return (int)HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12);
//}
//static int K4() 
//{
//	return (int)HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15);
//}

//滤波时间30ms，按下30-1500ms期间放手的是短按，其他为长按，抬起后自动清理
static KEY key0={0,3,300,0,0,0};
//static KEY key2={0,3,150,0,0};
//static KEY key3={0,3,150,0,0};
//static KEY key4={0,3,300,0,0};


void initKey()
{
	key0.read_key = K1;
//	key2.read_key = K2;
//	key3.read_key = K3;
//	key4.read_key = K4;
}


static int ReadKeyValue(KEY *key)
{
	unsigned long cur_pos = get_key_clk();
	if(cur_pos < key->new_pos)//当前的时间小于下一个要读取的时间，10ms间隔
	{
		return -1;
	}
	else
	{
		key->new_pos = cur_pos + 1;
	}
	switch(key->flag)
	{
		case 0:
			if(key->read_key() ==0)//press down
			{
				key->flag = 1;
				key->start_milisecond = cur_pos;
			}
			break;
		case 1:
			if(key->read_key()==1)
			{
    			 key->flag = 0;
    			 break;
			}
			if((cur_pos - key->start_milisecond)>key->wave_milisecond)
			{
				key->flag = 2;
				//key->start_milisecond = HAL_GetTick();
			}
			break;
		case 2:
			if(key->read_key()==0)//is still press downs
			{
				if((cur_pos - key->start_milisecond) > key->short_press_milisecond)
				{
    				key->flag = 3;
    				return LP;
				}
			}
			else//if press up
			{
				key->flag = 0;
				return SP;
			}
			
		case 3:
			if(key->read_key()==1)//wait unil press up
			{
				key->flag = 0;
			}
			break;
		default:
			break;
	}
	return -1;
}

static int ReadKeyValue_t(KEY *key)
{
	unsigned long cur_pos = get_key_clk();
	if(cur_pos < key->new_pos)//当前的时间小于下一个要读取的时间，10ms间隔
	{
		return -1;
	}
	else
	{
		key->new_pos = cur_pos + 1;
	}
	switch(key->flag)
	{
		case 0:
			if(key->read_key() ==0)//press down
			{
				key->flag = 1;
				key->start_milisecond = cur_pos;
			}
			break;
		case 1:
			if(key->read_key()==1)
			{
    			 key->flag = 0;
    			 break;
			}
			if((cur_pos - key->start_milisecond)>key->wave_milisecond)
			{
				key->flag = 2;
				//key->start_milisecond = HAL_GetTick();
			}
			break;
		case 2:
			if(key->read_key()==0)//is still press downs
			{
				if((cur_pos - key->start_milisecond) > key->short_press_milisecond)
				{
    				key->flag = 3;
					key->start_milisecond = cur_pos;
    				return LP;
				}
			}
			else//if press up
			{
				key->flag = 0;
				return SP;
			}
			
		case 3:
			if(key->read_key()== 0)//wait unil press up
			{
				if((cur_pos - key->start_milisecond) > 80)
				{
					key->start_milisecond = cur_pos;
					return LLP;
			    }
			}
			else
			{
				key->flag = 0;
			}
			break;
		default:
			break;
	}
	return -1;
}
int GetKey(int whichkey)
{
	
	switch(whichkey)
	{
		case 0:
			return ReadKeyValue(&key0);
//		case 2:
//			return ReadKeyValue_t(&key2);
//		case 3:
//			return ReadKeyValue_t(&key3);
//		case 4:
//			return ReadKeyValue(&key4);
		default:
			return -1;
	}
}
static void _ClearKeyFlag(KEY *key)
{
	key->flag = 0;
}

static void key_reset_tick(unsigned long tick)
{
	key_tick -= tick;
	key0.start_milisecond -= tick;
	key0.start_milisecond -= tick;
//	key2.new_pos -= tick;
//	key2.start_milisecond -= tick;
//	key3.new_pos -= tick;
//	key3.start_milisecond -= tick;
//	key4.new_pos -= tick;
//	key4.start_milisecond -= tick;
}
int SetKeyPressTime()
{
	return 0;
}


void init_key_gpio()
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = (GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz  ;
    GPIO_Init(GPIOC, &GPIO_InitStruct); 
	initKey();
}


int read_pin_config()
{
	int ret;
	ret = (int)GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8);
	ret = ret <<1;
	ret |= (int)GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7);
	return ret;
}