#include "stm32f10x_lib.h"
#include "terminal.h"
#include "delay_work.h"
#include "sz_libc.h"
#include "usart2.h"
#include "usart3.h"
#include "sz_libc.h"
#include "systick.h"
#include "stm_console.h"
#include "cc110x_module.h"
#include "device_mangement.h"
#include "module_common.h"
#include "virtual_net.h"
#include "virtual_net_handle_data.h"
#include "sim.h"
#include "msg_sim.h"
#include "gpio_exit_demo.h"
#include "led.h"
#include "key.h"
#include "device_info.h"
#include "cc110x_hal.h"
#include "LiteMasterResource.h"
#include "cards_store.h"

#define PRNT_MAX 4
#define PRIN_BUF 1024
static char print_buf[PRIN_BUF];
struct prinf_bft
{

	char print_buf[PRIN_BUF];
    int flg;
};
struct prinf_bft prnt[PRNT_MAX];
static int prnt_i=0;
static void get_format_buf(char **p_buf,int *len)
{
	if(get_login_status()!=0)
	{
		*p_buf=0;
		*len=0;
		return ;
	}
	*p_buf = prnt[prnt_i++%PRNT_MAX].print_buf;
	*len = PRIN_BUF;
}


static struct delay_work m_delay_work;

static void send_thread(struct delay_work *pdlw)
{
	sz_printk("1.\n");
	shedule_delay(pdlw,1000);
}
static void init_send_thread()
{

	shedule_init(&m_delay_work);
    m_delay_work.handle = send_thread;
	m_delay_work.pri_data =0;
	shedule_delay(&m_delay_work,1200);//
}

//这个代码正式量产的时候去掉
static void console_pmt()
{
	init_console();
	register_console_write((int (*)(char *,int))usart3_write);
	register_cmds();
	//set_login_info("dog","123456");
}

static void sz_test()
{
	int aa = 0x12345678;
	short bb = 0x1234;
	char buf[4]={0x0D,0x13,0x12,0x56};
	sz_printk("sz_htonl test:aa=%#X,sz_htonl(aa)=%#X\n",aa,sz_htonl(aa));
	sz_printk("sz_htons test:bb=%#X,sz_htons(bb)=%#X\n",bb,sz_htons(bb));
	sz_printk("sz_ctoi=%#x\n",sz_ctoi(buf,4));
	sz_printk("*(int*)buf=%#x\n",*(int*)buf);
}
static int test_endain()
{
	int aa = 1;
	
	if(*(char*)&aa == 1)
	{

		sz_printk("Little endain\n");
	    return 0;
	}
	else
	{
		sz_printk("Big endain\n");
	    return 1;
	}
}

char test_buf[64];

static void console_entery()
{
	int ret = 0;
	char *temp;
	char *start;
	ret = usart3_read(test_buf,64);
	if(ret>0)
	{
		start = test_buf;
		temp = start+ret;
		while(start<temp)
		{
			console_proc(*start++);
		}
	}
}
static void sim_windows(int device,char *buf,int len)
{
	switch(device)
	{
		case 0:
			sim_msg_proc((const char *)buf,len);
			break;
		case 1:
			break;
		default:
			break;
	}
	
}

static void key_proc()
{
	int key_value;
	key_value = GetKey(0);
	switch(key_value)
	{
		case 0:
		case 1:
		case 2:
		case 3:

		    {
				int pos=read_pin_config();
			    sz_printk("Key value:%d\n",key_value);
			    sz_printk("pin config:%#x\n",pos);
			    get_subdevice(pos+1)->position_id = ((pos+1)|0x80);
				device_setup(LITECTRL_FIRST_RUNNING,0);
		    }
		default:
			break;
	}
}
int main()
{
	systick_init(1);
	init_delay_work();
	uart2_init(115200);
	uart3_init(115200);
	led_gpio_init();
	init_console();
	sz_set_system_tick((int(*)(void))get_systick);
	sz_set_libc_callback((int (*)(char *buf,int len))usart3_write,get_format_buf);//设置并启用C库
	//init_send_thread();
	console_pmt();//console init
	//sz_test();
	test_endain();
	cc110x_module_init();
	virtual_net_init();
	virtul_net_handle_data_init();
	device_init();
	init_cards();
	init_key_gpio();
	init_gloabel_info();
    device_setup(LITECTRL_NORMAL_WORK,0);
	//device_setup(LITECTRL_PRODUCE_CHEKING,0);
	//device_setup(LITECTRL_BOARD_CHEKING,0);
	init_device_info();
#if 1	//sim module
    init_sim();
	systick_delay(10);
	set_sim_write_cb(usart2_write);
	set_sim_read_cb(usart2_read);
	set_sim_recall_fun(sim_windows);
	if(check_sim_state()==0)//if sim is close
	{
		sz_printk("sim will open.\n");
		open_sim();
		virtual_net_thread_start(-1);
	}
	else
	{
		sz_printk("sim has been opened!\n");
		sim_timer_run();//run the thread
		virtual_net_thread_start(0);
	}
#endif	
	while(1)
	{
		console_entery();
		cc110x_module_entery();
		delay_work_entry();
		sim_entry();
		ShowLeds();
		virtul_net_handle_data_entery();
		key_proc();
	}
	return 0;
}
