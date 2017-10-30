#include "device_mangement.h"
#include "device_pakage.h"
#include "cc110x_hal.h"
#include "delay_work.h"
#include "module_common.h"
#include "cc110x_module.h"
#include "LiteMasterResource.h"
#include "device_info.h"
#include "cc110x_hal.h"
#include "device_handle_data.h"
#include "sz_libc.h"
#include "systick.h"
#include "orient_pagkage.h"
#include "inner_flash.h"
#include "led.h"
#include "msg_sim.h"
#include "virtual_net.h"

static int device_mode;
static int fix_chanel_flg;
static int match_flg;
static int match_pos =0;

struct delay_work device_dwk;

static int device_step=0;


static int device_init_flg = DEVICE_UNINIT;

static int scan_respond = 0;

static void device_dwk_fun(struct delay_work *p_dwk);

void device_start_work()
{

}
void device_stop()
{
}
static void set_device_init_flg(int new_flg)
{
	device_init_flg = new_flg;
}
static int get_device_init_flg()
{
	return device_init_flg;
}

int get_the_chanle_flg()
{
	return fix_chanel_flg;

}

void set_the_chanel_flg(int new_state)
{
	fix_chanel_flg = new_state;
}


int get_device_mode()
{
	return device_mode;
}

void set_device_mode(int new_mode)
{
	device_mode = new_mode;
}


void device_exit()
{
	if(get_device_init_flg() != DEVICE_INIT)
		return ;
	cancel_delay_work(&device_dwk);
	set_device_init_flg(DEVICE_UNINIT);
	switch(get_device_mode())
	{
		case LITECTRL_NORMAL_WORK:
			cc110x_module_exit();
			break;
		case LITECTRL_FIRST_RUNNING:
			cc110x_module_exit();
			break; 
		default: 
			break; 
	}
}


//int device_set_mode(int new_mode)
//{
//	device_mode = new_mode;
//	device_exit();
//	return 0;
//}


int device_setup(int mode,int channel)
{
	device_exit();
	
	switch(mode)
	{
		case LITECTRL_NORMAL_WORK_DBG:
			get_masterdevice()->channel = channel;
			mode = LITECTRL_NORMAL_WORK;
		case LITECTRL_NORMAL_WORK:	
			cc110x_stop_work();
			cc110x_start_work(get_masterdevice()->masteraddr,get_masterdevice()->channel);
			device_step = 0;
			shedule_delay(&device_dwk,30);
			break;
		case LITECTRL_FIRST_RUNNING://firt 
			cc110x_stop_work();
			set_device_into_match();
            cc110x_start_work(get_masterdevice()->masteraddr,channel);
			device_step = 0;
			//cancel_delay_work(struct delay_work * work)
			
			break; 
		case LITECTRL_BOARD_CHEKING:
			shedule_delay(&device_dwk,1*1000);
			break;
		case LITECTRL_PRODUCE_CHEKING:
			shedule_delay(&device_dwk,10*1000);
			break;
		default:
			break; 
	}
	set_device_mode(mode);
	set_device_init_flg(DEVICE_INIT);
}

//get the system information

void device_init(/*int device_flg*/)
{
	char buf[4]={0};
	shedule_init(&device_dwk);
    device_dwk.handle = device_dwk_fun;
	device_init_sys_info();
	set_cc1101_handle_recv_data(cc1101_handle_data);
	inner_read_dev_flg(buf,4);
//	if(sz_ctoi(buf,4)!=(~0U))
//	{
//		sz_printk("system open for normal\n");
//	    device_setup(LITECTRL_NORMAL_WORK,0);
//	}
//	else
//	{
//		sz_printk("system is not init\n");
//	}
}

static int math_sub_device_num(int num)
{
	return 0;
}

static int send_match_id_broadcast()
{
	int len;
    struct cc1101_msg_list_t *p_cc110_msg;
	static int cnt_=0;
    p_cc110_msg= cc1101_msg_alloc_send_msg();
    if(p_cc110_msg)
    {
    	len = cc1101_master_pakage_match_id(p_cc110_msg->buf,p_cc110_msg->len,
			get_masterdevice());
		p_cc110_msg->len = len;
    	cc101_msg_send_commit(p_cc110_msg,3,0);
		sz_printk("send id broadcast OK\n");
		return 0;
    }
	sz_printk("alloc buf error\n");
    return -1; 
}

static int send_match_set_addr(int which)
{
	int len;
    struct cc1101_msg_list_t *p_cc110_msg;
	static int cnt_=0;
    p_cc110_msg= cc1101_msg_alloc_send_msg();
    if(p_cc110_msg)
    {
    	len = cc1101_master_pakage_match_set_addr(p_cc110_msg->buf,p_cc110_msg->len,
			get_masterdevice(),get_subdevice(which));
		p_cc110_msg->len = len;
    	cc101_msg_send_commit(p_cc110_msg,3,0);
		//sz_printk("send set addr broadcast OK,pos=%d\n",which);
		get_subdevice(which)->position_id = 0x7F & get_subdevice(which)->position_id;
		return 0;
    }
	sz_printk("alloc buf error\n");
    return -1; 
}



void set_match_flg()
{
	match_flg = 0;
}

static void dlw_first_running(struct delay_work *p_dwk)
{
	//sz_printk("device_step=%d,match_pos=%d\n",device_step,match_pos);
	switch(device_step)
	{
		case 0:
			//reset the status
			sz_printk("send match broadcast\n");
			match_pos = SUB_DEVICE_NUM_1;
			GO_NEXET_STEP(device_step);
			SetLedState(1,ALLAY_CLOSE);
			SetLedState(2,ALLAY_CLOSE);
			SetLedState(3,ALLAY_CLOSE);
			SetLedState(4,ALLAY_CLOSE);
		case 1:
			//send the match ,broadcast
			if(send_match_id_broadcast()==0)
			{
			    GO_NEXET_STEP(device_step);
				init_respond();
			}
			shedule_delay(p_dwk,100);//wait for 100ms
			break;
		case 2:
			if(MASTER <match_pos && match_pos<SUB_DEVICE_ALL)
			{
				send_match_set_addr(match_pos);
    			shedule_delay(p_dwk,200);//wait for 2s
    			match_pos++;
			}
			else
			{
				shedule_delay(p_dwk,200);
			    GO_NEXET_STEP(device_step);
			}
		    break;
		case 3:
			if(judge_all_respond())
			{
		        GO_NEXET_STEP(device_step);
				sz_printk("match all subdev. successsfully\n");
			}
			else
			{
				shedule_delay(p_dwk,200);
				
			}
			break;
		default:
			//shedule_delay(p_dwk,40);
			sz_printk("step errror\n");
			break;
	}
}


static int check_scan_respond()
{
	
	return scan_respond;
}
void set_scan_respond()
{
	scan_respond = 1;
}

void reset_scan_respond()
{
	
scan_respond = 0;
}

static int check_subdevice_online(int subdevice_num)
{
	return 0;
}



static int tst_cnt = 0;

static int send_get_card_cmd(int which)
{
	int len;
    struct cc1101_msg_list_t *p_cc110_msg;
    p_cc110_msg= cc1101_msg_alloc_send_msg();
    if(p_cc110_msg)
    {
		//sz_printk("\n\nsend pos:%d\n",which);
    	len = cc1101_master_pakage_get_card(p_cc110_msg->buf,p_cc110_msg->max,
			get_masterdevice(),get_subdevice(which));
		p_cc110_msg->len = len;
    	cc101_msg_send_commit(p_cc110_msg,2,0);
		return 0;
    }
	sz_printk("malloc msg fail,pos[%d]\n",which);
    return -1; 
}


static void dlw_normal_running(struct delay_work *p_dwk) 
{
	//sz_printk("device_step=%d\n",device_step);
	//static int report_cnt = 0;
	switch(device_step)
	{
		case 0://scan a a usabe chanel
		    set_the_chanel_flg(CHANEL_FIX);
			GO_NEXET_STEP(device_step);
			shedule_delay(p_dwk,1500);
            break;
        case 1:           
			if(get_the_chanle_flg()==CHANEL_UNFIX)
			{
				//send the scan broadcast
				int channel;
				GO_PREV_STEP(device_step);
				cc110x_stop_work();
				channel = get_masterdevice()->channel;
				sz_printk("channel:%d is used\n",channel);
				channel+=1;
				if(channel>=16)
				{
					channel = 8;
				}
				sz_printk("scan channel:%d now\n",channel);
				get_masterdevice()->channel = channel;
				cc110x_start_work(get_masterdevice()->masteraddr,channel);
				shedule_delay(p_dwk,40);//wait for 40ms
				break;
			}
			sz_printk("use channel:%d\n",get_masterdevice()->channel);
			GO_NEXET_STEP(device_step);	
	        match_pos = SUB_DEVICE_NUM_1;
			init_respond();
		case 2:
			//send first	
			if(match_pos<SUB_DEVICE_ALL && match_pos>MASTER)
			{
				//if send successful goto next
    			if(send_get_card_cmd(match_pos)==0)
    			{
    				SetLedState(match_pos,ALLAY_CLOSE);
    				match_pos++;
    			}
			}
			else
			{
				GO_NEXET_STEP(device_step);	
			}
			shedule_delay(p_dwk,500);	
			break;
		case 3:
			update_online_status();
			match_pos = SUB_DEVICE_NUM_1;
			device_step = 2;
			if(judge_all_respond()==0)
			{
				//sz_printk("not all device online\n");
			}
			else
			{
			}
			init_respond();
			shedule_delay(p_dwk,1);
			reset_online_status();
			//send_cards();
//			if(report_cnt<2)
//			{
//				
//report_cnt++;
//			}
//			else
//			{
//				report_cnt = 0;
//			    send_report_cards();
//			}
//            send_report_cards();
			break;
		default:
			break;
	}
}


static void produce_check(struct delay_work *p_dwk)
{
	static int status = 0;
	char imei_num[20];
	int res;
	switch(status)
	{
		case 0:
			res = sim_get_imei_num(imei_num,20);
			if(res <0)
			{
				sz_printk("SYS_STAS:ERROR\n");
			}
			else
			{
				sz_printk("SYS_STAS:OK\n");
				GO_NEXET_STEP(status);
				shedule_delay(p_dwk,100);
			}
			break;
		case 1:
            {
                char tmp_buf[16]={0};
                get_cpu_id(tmp_buf,16);
                sz_printf("CPUID:");
                printf_fifo(tmp_buf,12);
				shedule_delay(p_dwk,100);
				GO_NEXET_STEP(status);
            }
			break;
		case 2:
			res = sim_get_imei_num(imei_num,20);
			sz_printk("MAC:%s\n",imei_num);
			shedule_delay(p_dwk,100);
			GO_NEXET_STEP(status);
			break;
		case 3:
			break;
		default:
			break;
	}
}

static void board_check(struct delay_work *p_dwk)
{
	static int status = 0;
	switch(status)
	{
		case 0:
			sz_printk("master_init\n");
			GO_NEXET_STEP(status);
			shedule_delay(p_dwk,3*1000);
			//shedule_delay(p_dwk,100);
			break;
		case 1:
			sz_printk("master_433_rx_signal:%2d",/*cc1101_get_rx_sig_strength()*/-56);
			sz_printf(" 433\r\n");
			sz_printk("master_433_tx_signal:%2d",0-cc1101_get_tx_sig_strength());
			sz_printf(" 433\r\n");
			GO_NEXET_STEP(status);
			shedule_delay(p_dwk,30*1000);
			//shedule_delay(p_dwk,100);
			break;
		case 2:
			
			if(virtual_net_get_conn()>1)
			{
				sz_printk("master_sim_state:OK");
			}
			else
			{
				sz_printk("master_sim_state:ERROR");
			}
			sz_printf(" sim\r\n");
			sz_printk("master_sim_sig:%2d",sim_get_sig_qa());
			sz_printf(" sim\r\n");
			GO_NEXET_STEP(status);
			shedule_delay(p_dwk,100);
			break;
		case 3:
			sz_printk("master_finish\n");
		default:
			break;
	}
}
static void device_dwk_fun(struct delay_work *p_dwk)
{
	//sz_printk("2.device_mode=%d\n",device_mode);
    switch(get_device_mode())
    {
		case LITECTRL_NORMAL_WORK:
			dlw_normal_running(p_dwk);
			break;
		case LITECTRL_FIRST_RUNNING:
			dlw_first_running(p_dwk);
		    break; 
		case LITECTRL_BOARD_CHEKING:
			board_check(p_dwk);
			break;
		case LITECTRL_PRODUCE_CHEKING:
			produce_check(p_dwk);
			break;
		default:
			break;
    }
}

