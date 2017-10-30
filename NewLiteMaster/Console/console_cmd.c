#include "console_cmd.h"
#include "inner_flash.h"
#include "sz_libc.h"
#include "cards_store.h"
#include "sz_libc.h"
#include "led.h"
#include "inner_flash.h"
#include "device_info.h"
#include "sim.h"
#include "virtual_net_handle_data.h"
#include "sz_json.h"
#include "cards_store.h"
#include "orient_pagkage.h"
#include "cc110x_hal.h"
#include "msg_sim.h"
#include "cc110x_hal.h"

static int sz_strcmp_t(char *str1,char *str2)
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

int console_cmd_echo(char argc,char *argv[])
{
	int i;
	int addr;
	if(argc < 2)
		return -1;

	if(sz_strcmp_t("sys_flg",argv[1])==0)
	{
		int value = sz_strtoul(argv[0],0,10);
		inner_write_dev_flg((char*)&value,4);
	}
	else if(sz_strcmp_t("maddr",argv[1])==0)
	{
		if(argv[0][0]=='0' &&(argv[0][1]=='x'||argv[0][1]=='X'))
			addr = sz_strtoul(argv[0],0,16);
		else
			addr = sz_strtoul(argv[0],0,10);
		
		inner_write_dev_addr(0,(char*)&addr,4);
	}
	else if(sz_strcmp_t("pos1",argv[1])==0)
	{
		if(argv[0][0]=='0' &&(argv[0][1]=='x'||argv[0][1]=='X'))
			addr = sz_strtoul(argv[0],0,16);
		else
			addr = sz_strtoul(argv[0],0,10);
		inner_write_dev_addr(1,(char*)&addr,4);
	}
	else if(sz_strcmp_t("pos2",argv[1])==0)
	{
		if(argv[0][0]=='0' &&(argv[0][1]=='x'||argv[0][1]=='X'))
			addr = sz_strtoul(argv[0],0,16);
		else
			addr = sz_strtoul(argv[0],0,10);
		inner_write_dev_addr(2,(char*)&addr,4);
	}
	else if(sz_strcmp_t("pos3",argv[1])==0)
	{
		if(argv[0][0]=='0' &&(argv[0][1]=='x'||argv[0][1]=='X'))
			addr = sz_strtoul(argv[0],0,16);
		else
			addr = sz_strtoul(argv[0],0,10);
		inner_write_dev_addr(3,(char*)&addr,4);
	}
	else if(sz_strcmp_t("pos4",argv[1])==0)
	{
		if(argv[0][0]=='0' &&(argv[0][1]=='x'||argv[0][1]=='X'))
			addr = sz_strtoul(argv[0],0,16);
		else
			addr = sz_strtoul(argv[0],0,10);
		inner_write_dev_addr(4,(char*)&addr,4);
	}
	else if(sz_strcmp_t("domain1",argv[1])==0)
	{
		innner_write_domain(1,argv[0],64);
	}
	else if(sz_strcmp_t("domain2",argv[1])==0)
	{
		innner_write_domain(2,argv[0],64);
	}
	else if(sz_strcmp_t("domain3",argv[1])==0)
	{
		innner_write_domain(3,argv[0],64);
	}
	else if(sz_strcmp_t("domain4",argv[1])==0)
	{
		innner_write_domain(4,argv[0],64);
	}
	else if(sz_strcmp_t("domain5",argv[1])==0)
	{
		innner_write_domain(5,argv[0],64);
	}
	else if(sz_strcmp_t("domain6",argv[1])==0)
	{
		innner_write_domain(6,argv[0],64);
	}
	else if(sz_strcmp_t("port1",argv[1])==0)
	{
		if(argv[0][0]=='0' &&(argv[0][1]=='x'||argv[0][1]=='X'))
			addr = sz_strtoul(argv[0],0,16);
		else
			addr = sz_strtoul(argv[0],0,10);
		innner_write_port(1,(char*)&addr,6);
	}
	else if(sz_strcmp_t("port2",argv[1])==0)
	{
		if(argv[0][0]=='0' &&(argv[0][1]=='x'||argv[0][1]=='X'))
			addr = sz_strtoul(argv[0],0,16);
		else
			addr = sz_strtoul(argv[0],0,10);
		innner_write_port(2,(char*)&addr,6);
	}
	else if(sz_strcmp_t("port3",argv[1])==0)
	{
		if(argv[0][0]=='0' &&(argv[0][1]=='x'||argv[0][1]=='X'))
			addr = sz_strtoul(argv[0],0,16);
		else
			addr = sz_strtoul(argv[0],0,10);
		innner_write_port(3,(char*)&addr,6);
	}
	else if(sz_strcmp_t("port4",argv[1])==0)
	{
		if(argv[0][0]=='0' &&(argv[0][1]=='x'||argv[0][1]=='X'))
			addr = sz_strtoul(argv[0],0,16);
		else
			addr = sz_strtoul(argv[0],0,10);
		innner_write_port(4,(char*)&addr,6);
	}
	else if(sz_strcmp_t("port5",argv[1])==0)
	{
		if(argv[0][0]=='0' &&(argv[0][1]=='x'||argv[0][1]=='X'))
			addr = sz_strtoul(argv[0],0,16);
		else
			addr = sz_strtoul(argv[0],0,10);
		innner_write_port(5,(char*)&addr,6);
	}
	else if(sz_strcmp_t("port6",argv[1])==0)
	{
		if(argv[0][0]=='0' &&(argv[0][1]=='x'||argv[0][1]=='X'))
			addr = sz_strtoul(argv[0],0,16);
		else
			addr = sz_strtoul(argv[0],0,10);
		innner_write_port(6,(char*)&addr,6);
	}
	else if(sz_strcmp_t("channel",argv[1])==0)
	{
		if(argv[0][0]=='0' &&(argv[0][1]=='x'||argv[0][1]=='X'))
			addr = sz_strtoul(argv[0],0,16);
		else
			addr = sz_strtoul(argv[0],0,10);
		inner_write_chanel((char*)&addr,2);
	}
	else if(sz_strcmp_t("led1",argv[1])==0)
	{
		int cmd = argv[0][0]=argv[0][0]-'0';
		switch(cmd)
		{
			case 0:
				SetLedState(1,ALLAY_OPEN);
				break;
			case 1:
				SetLedState(1,ALLAY_CLOSE);
				break;
			case 2:
				SetLedState(1,ALLAY_FLASH);
				break;
			case 3:
				SetLedState(1,ONE_FLASH_ON);
				break;
			default:
				break;
		}
	}
	else if(sz_strcmp_t("led2",argv[1])==0)
	{
		int cmd = argv[0][0]=argv[0][0]-'0';
		switch(cmd)
		{
			case 0:
				SetLedState(2,ALLAY_OPEN);
				break;
			case 1:
				SetLedState(2,ALLAY_CLOSE);
				break;
			case 2:
				SetLedState(2,ALLAY_FLASH);
				break;
			case 3:
				SetLedState(2,ONE_FLASH_ON);
				break;
			default:
				break;
		}
	}
	else if(sz_strcmp_t("device_id",argv[1])==0)
	{
		inner_write_device_id(argv[0],sz_strlen(argv[0]));
		sz_printk("SETTIING_ACK:SET_DEVIC_ID_OK\n");
		//sz_printk("SETTIING_ACK:SET_DEVIC_ID_OK\n");
	}
	else
	{
		
	}
	return 0;
}

int console_cmd_cat(char argc,char *argv[])
{
	if(sz_strcmp_t("system",argv[0])==0)
	{


		inner_printk_t();
	}
	else if(sz_strcmp_t("maddr",argv[0])==0)
	{
		char buf[4]={0};
		inner_read_dev_addr(0,buf,2);
        sz_printk("master addr = %s\n",buf);
	}
	else if(sz_strcmp_t("msg",argv[0])==0)
    {
		
        msg_printf();
	}
	else if(sz_strcmp_t("ifconfig",argv[0])==0)
	{
		print_config();
	}
	else if(sz_strcmp_t("sim_goi",argv[0])==0)
	{
        sim_get_gloable_object_id();
	}
    else if(sz_strcmp_t("net_state",argv[0])==0)
	{
		
        virtual_net_print_state();
	}
	else if(sz_strcmp_t("report",argv[0])==0)
	{
//		send_report_cards();
	}
	else if(sz_strcmp_t("sim_sig",argv[0])==0)
	{
		sz_printk("sig. strlen:%d\n",sim_get_sig_qa());
	}
	else if(sz_strcmp_t("cc1101",argv[0])==0)
	{
		printf_cc1101_info();		
	}
	return 0;
}

