#include "sim.h"
#include "terminal.h"
#include "sz_libc.h"
#include "virtual_net.h"
#include "virtual_net_handle_data.h"
#include "sz_json.h"
#include "orient_pagkage.h"
#include "sz_pakage.h"
#include "cc110x_hal.h"
#include "device_pakage.h"
#include "LiteMasterResource.h"
#include "console_cmd.h"
#include "Device_mangement.h"
#include "module_common.h"
#include "cards_store.h"
#include "sim.h"


#define data_len 28
char tx_buffer[data_len]={0x28,0x15,0x03,0x04,0x05,0x06,0x07,0x08};//tx data
//char tx_buffer[data_len]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//tx data



//static char get_adc[] = "get_adc";
//static char get_num[] = "get_self_num";
//static char call[] = "sim_call";
//static char cansel_call[] = "cansel_call";
//static char check_sim[] = "check";
//static char test_snd_msg[] = "mesaage_send_test";
//static char cmd_read_msg[]="read_msg";
//static char cmd_delete[]="delete_all_msg";
//static char cmd_get_CSQ[]="get_signal";
//static char cmd_sim_reg_net[]="sim_reg_net";
//static char cmd_sim_check_net[]="sim_check_net";
//static char send_msg_one[]="This is a message test!";
static const char sim_conn[]="conn_tst";
static const char sim_disconn[]="dis_conn";
static const char sim_shake[]="shake";
static const char sim_send_cards[]="send_cards";
static const char sim_recv_tst[]="recv_tst";
static const char send_heart_str[]="heart";
static const char str_get_cpu_id[]="get_cpu_id";
static const char str_format[]="format_flash";
static const char str_set_maddr[]="set_maddr";
static const char str_get_maddr[]="get_maddr";
static const char str_echo[]="echo";
static const char str_cat[]="cat";
static const char str_lst[]="ls";
static const char str_tst_wrt[]="inner_test_write";
static const char *str_setup="setup"; 
static const char *str_sim_reopen="sim_reopen";
static const char *str_sim_close="sim_close";
static const char *str_sim_open="sim_open";

static const char *str_clear_card="clear_card";
static const char *str_print_card="print_card";

static const char *str_sim_cmd="sim";

static int strcmp_t(char *str1,char *str2)
{
	char *end;
	int len1;
	int len2;
	len1 = sz_strlen(str1);
	len2 = sz_strlen(str2);
	if( len1 != len2)
		return -1;
	end = str1 + len1;
	while(str1 < end)
	{
		if(*str1 != *str2)
			break;
		str1++;
		str2++;
	}
	if(str1 == end)
		return 0;
	else
		return -1;
}
static int console_do_cmd(int argc,char *argv[])
{
	char ch;
	int len;
	int i;
	if(argc==0)
		return -1;
    for(i=0;i<argc;i++)
    {
		sz_printk("argc%d=\"%s\"\n",i,argv[i]);

    }
//	if(strcmp_t(get_adc,argv[0])==0)
//	{
//		sz_printk("adc:%d\n",0);
//	}
//	else if (strcmp_t(get_num,argv[0])==0)
//	{
//		get_my_phone_num();
//	}
//	else if((strcmp_t(call,argv[0])==0))
//	{
//		call_test_sim();
//	}
//	else if((strcmp_t(cansel_call,argv[0])==0))
//	{
//		cancle_sim();
//	}
//	else if((strcmp_t(check_sim,argv[0])==0))
//	{
//	}
//	else if((strcmp_t(test_snd_msg,argv[0])==0))
//	{
//		send_one_msg(send_msg_one,sz_strlen(send_msg_one));
//	}
//	else if((strcmp_t(cmd_read_msg,argv[0])==0))
//	{
//		read_msg(argv[1],argv[2]);
//	}
//	else if((strcmp_t(cmd_delete,argv[0])==0))
//	{
//		delete_msg(argv[1],argv[2]);
//	}
//	else if(strcmp_t(cmd_get_CSQ,argv[0])==0)
//	{
//		get_singal_QA();
//	}
//	else if(strcmp_t(cmd_sim_reg_net,argv[0])==0)
//	{
//		sim_reg_creg_net();
//		HAL_Delay(10);
//		sim_reg_cgreg_net();
//		
//	}
//	else if(strcmp_t(cmd_sim_check_net,argv[0])==0)
//	{
//		sim_check_creg_net();
//		HAL_Delay(10);
//		sim_check_cgreg_net();
//	}
//	else 
	if(strcmp_t((char*)sim_conn,argv[0])==0)
	{
		virtual_net_connect();
	}
	else if(strcmp_t((char*)sim_disconn,argv[0])==0)
	{
		virtual_net_close();
	}
	else if(strcmp_t((char*)sim_shake,argv[0])==0)
	{
		send_handshake();
	}
	else if(strcmp_t((char*)sim_send_cards,argv[0])==0)
	{
		send_cards();
	}
	else if(strcmp_t((char*)sim_recv_tst,argv[0])==0)
	{
		sim_recv();
	}
	else if(strcmp_t((char*)send_heart_str,argv[0])==0)
	{
		send_heart_beat();
	}
    else if(strcmp_t((char*)str_get_cpu_id,argv[0])==0)
    {
		char tmp_buf[16]={0};
        get_cpu_id(tmp_buf,16);
		sz_printk("CPUID:");
		printf_fifo(tmp_buf,12);
    }
	else if(strcmp_t((char*)str_format,argv[0])==0)
	{
		
		inner_format_flash();
	}
	else if(strcmp_t((char*)str_echo,argv[0])==0)
	{
		console_cmd_echo(argc-1,&argv[1]);
	}
	else if(strcmp_t((char*)str_cat,argv[0])==0)
	{
		console_cmd_cat(argc-1,&argv[1]);
	}
	else if(strcmp_t((char*)str_setup,argv[0])==0)
	{
		if(argc<2)
		{
		    device_setup(LITECTRL_NORMAL_WORK,0);
		}
		else if(strcmp_t("-s",argv[1])==0)
		{
			int channel = sz_strtoul(argv[2],0,10);
			device_setup(LITECTRL_NORMAL_WORK_DBG,channel);
		}
		else 
		{
			int channel = sz_strtoul(argv[1],0,10);
	        device_setup(LITECTRL_FIRST_RUNNING,channel);
		}
	}
	else if(strcmp_t((char*)str_tst_wrt,argv[0])==0)
	{
	    inner_test_write();
	}
	else if(strcmp_t((char*)str_clear_card,argv[0])==0)
	{
		clear_card();
	}

	else if(strcmp_t((char*)str_print_card,argv[0])==0)
	{
		if(argc<2)
		{
		    show_card_list_info();
		}
		else if(strcmp_t("-a",argv[1])==0)
		{
			show_card_all_info();
		}
		else if(strcmp_t("-s",argv[1])==0)
		{
			int pos;
			if(argc<3)
				return 0;
			
			pos= argv[2][0]-'0';
			if(pos<0 || pos > 4)
			{
				return 0;
			}
			else
			{
			    show_card_more_info(pos);
			}
		}
	}
	else if(strcmp_t((char*)str_sim_reopen,argv[0])==0)
	{
		sim_reopen();
	}
	else if(strcmp_t((char*)str_sim_close,argv[0])==0)
	{
		close_sim();
	}
	else if(strcmp_t((char*)str_sim_open,argv[0])==0)
	{
		open_sim();
	}
	else if(strcmp_t((char*)str_sim_cmd,argv[0])==0)
	{
		if(argc>1)
		{
			sim_send_cmd((const char*)argv[1]);
		}
	}
	
	return 0;
}

int register_cmds()
{
	//register_cmd(get_adc,strlen(get_adc));
	//register_cmd(call,strlen(call));
	//register_cmd(cansel_call,strlen(cansel_call));
	//register_cmd(check_sim,strlen(check_sim));
	//register_cmd(test_snd_msg,strlen(test_snd_msg));
	//register_cmd(cmd_read_msg,strlen(cmd_read_msg));
	//register_cmd(cmd_delete,strlen(cmd_delete));
	//register_cmd(get_num,strlen(get_num));
	//register_cmd(cmd_get_CSQ,strlen(get_num));
	//register_cmd(cmd_sim_reg_net,sz_strlen(cmd_sim_check_net));
	//register_cmd(cmd_sim_check_net,sz_strlen(cmd_sim_check_net));
	
//	register_cmd((char*)sim_conn,sz_strlen(sim_conn));
//	register_cmd((char*)sim_disconn,sz_strlen(sim_disconn));
//	register_cmd((char*)sim_shake,sz_strlen(sim_shake));
//	register_cmd((char*)send_heart_str,sz_strlen(send_heart_str));
//	register_cmd((char*)sim_send_cards,sz_strlen(sim_send_cards));
//	register_cmd(sim_recv_tst,sz_strlen(sim_recv_tst));
//    register_cmd((char*)console_tst,sz_strlen(console_tst));
	register_cmd((char*)str_get_cpu_id,sz_strlen(str_get_cpu_id));
//	register_cmd((char*)str_format,sz_strlen(str_format));
	register_cmd((char*)str_echo,sz_strlen(str_echo));
	register_cmd((char*)str_cat,sz_strlen(str_cat));
	register_cmd((char*)str_lst,sz_strlen(str_lst));
//	register_cmd((char*)str_tst_wrt,sz_strlen(str_tst_wrt));
	register_cmd((char*)str_setup,sz_strlen(str_setup));
	register_cmd((char*)str_clear_card,sz_strlen(str_clear_card));
	register_cmd((char*)str_print_card,sz_strlen(str_print_card));
	register_console_handl_data(console_do_cmd);
	register_cmd((char*)str_sim_reopen,sz_strlen(str_sim_reopen));
	register_cmd((char*)str_sim_close,sz_strlen(str_sim_close));
	register_cmd((char*)str_sim_open,sz_strlen(str_sim_open));
	register_cmd((char*)str_sim_cmd,sz_strlen(str_sim_cmd));
	
	return 0;
}
