#include "sz_pakage.h"
#include "sz_libc.h"
#include "sz_json.h"
#include "inner_flash.h"
#include "orient_pagkage.h"
#include "inner_flash.h"
#include "virtual_net_handle_data.h"
#include "virtual_net.h"
#include "cards_store.h"
#include "orient_pagkage.h"


#define RECV_MAX_LEN 1024
static char virtual_recv_data[RECV_MAX_LEN];
static int virtual_recv_len;
struct virtual_net_state_t
{
	int online_flg;
	int shake_flg;
	int conn_cnt;
	int server_cnt;
};
struct virtual_net_state_t net_info;

static void init_net_info()
{
	net_info.online_flg = -1;
    net_info.shake_flg = -1;
	net_info.conn_cnt = 1;
	net_info.server_cnt = 1;
}


void virtual_net_set_online(int flg)
{
	net_info.online_flg = flg;
}

void virtual_net_set_handshake(int flg)
{
	net_info.shake_flg = flg;
}


void virtual_net_inc_conn()
{
	net_info.conn_cnt++;
}

int virtual_net_get_conn()
{
	return net_info.conn_cnt;
}

void virtual_net_print_state()
{
	char buf[64]={0};
	int value;
	sz_printf("*****************************************************\r\n");
	sz_memset(buf,0,0);
	sz_printf("online             :%s\r\n",net_info.online_flg == -1?"off":"on");
	sz_printf("handshake          :%s\r\n",net_info.shake_flg == -1?"fail":"seccuss");
	sz_printf("reconn server times:%d\r\n",net_info.conn_cnt);
    innner_read_domain(net_info.server_cnt,buf,64);
	sz_printf("server domain      :%s\r\n",buf);
	sz_memset(buf,0,0);
	innner_read_port(net_info.server_cnt,buf,4);
	value = sz_ctoi(buf,4);
	sz_printf("server port        :%d\r\n",value);
	sz_printf("*****************************************************\r\n");
}

void get_recv_buf_str(struct vir_net_recv_buf_t *pvir_net_recv_buf)
{
	if(pvir_net_recv_buf)
	{
		pvir_net_recv_buf->buf = virtual_recv_data;
		pvir_net_recv_buf->len = &virtual_recv_len;
		pvir_net_recv_buf->max_len = RECV_MAX_LEN;
	}
}

void rest_recv_buf_str()
{
	virtual_recv_len = 0;
}

//static char mp_buf[512];
//const char *tst_str = "123456789abcdef";
static virtula_net_json_hanle(char *buf,int len)
{
	char tmp_buf[128];

//	static char proc_buf[256];
//	sz_memset(proc_buf,0,256);
//	sz_memcpy(proc_buf,buf,len);
//	sz_printk("recv data:");
//	sz_printf(proc_buf);
//	sz_printf("\r\n");

	sz_get_element((const char*)buf,len,"\"message\"",tmp_buf,128);
	sz_printk("get element:%s\n",tmp_buf);
	if(sz_strcmp(tmp_buf,"heart_beat")==0)
	{
		virtual_net_heart_beat_respond();
		sz_printk("hearbeat feedback\n\n");
	}
	else if(sz_strcmp(tmp_buf,"get_card")==0)
	{
		sz_printk("game get card\n\n");
		send_cards();
//		show_card_all_info();
		clear_card();
	}
	else if(sz_strcmp(tmp_buf,"get_card_ack")==0)
	{
		sz_printk("game get card ok\n\n");
	}
	else if(sz_strcmp(tmp_buf,"set_param")==0)
	{
		
	}
	
}


void virtual_net_proc(int cmd,char *buf,int len)
{
	sz_printk("cmd type:%d,len:%d\n",cmd,len);

    switch(cmd)
    {
		case 5:
//			sz_printk("hand shake respond\n");
//			sz_memset(proc_buf,0,256);
//			sz_memcpy(proc_buf,buf,len);
//			sz_printk(proc_buf);
//			sz_printk("%s",proc_buf);
			if(sz_kmp_find_string("Connect success!",buf)==0)
			{
				virtual_net_set_handshake(0);
				sz_printk("handshake success\n\n");
			}
//			if()
//			{
//				net_info.server_cnt++;
//			}
            
			break;
        case HeartBeat:
		case JSON_PAKAGE:
			//sz_printk("HanShake success\n");
			virtula_net_json_hanle(buf,len);
			break;
		default:
			//sz_get_element(buf,len,"");
			break;
    }
}
static int virtual_net_handle_data()
{
	char *pos_s;
	char *pos_e;
	int len;
	int pakage_len;
	int ret;
	struct pakage_str_t pk_str;
	struct protecol_head_t *proto;
	ret = sz_un_pakage(virtual_recv_data,virtual_recv_len,&proto,&pk_str);
	switch(ret)
	{
		case NO_HEAD :
//			sz_printk("there no head\n");
			virtual_recv_len = 0;
			break;
		case NO_TAIL:
//			sz_printk("there no tail\n");	
			virtual_recv_len = 0;
			break;
		case BUF_NO_ENOUGH:
//			sz_printk("there no enough data\n");
			break;
		default:
			//sz_memset(mp_buf,0,512);
			len=pk_str.pos_e - pk_str.pos_s;
			pakage_len = pk_str.pakage_e - pk_str.pakage_s;
			sz_printk("buffer size:%d,pakage len:%d,data len:%d\n",virtual_recv_len,pakage_len,len);
			virtual_net_proc(proto->protocolFlag,pk_str.pos_s,len);
			if(pakage_len == virtual_recv_len)
			{

				virtual_recv_len = 0;
			    sz_printk("reset recv buf\n");
			}
			else
			{
				sz_printk("more data recv buf\n");
				virtual_recv_len = virtual_recv_len -(pk_str.pakage_e-virtual_recv_data);
				sz_memcpy(virtual_recv_data,pk_str.pakage_e,virtual_recv_len -(pk_str.pakage_e-virtual_recv_data));
			}
			//sz_memcpy(mp_buf,pk_str.pos_s,len);
			//sz_printk("ok:%s\n",mp_buf);
			break;
	}
	return 0;
}

int virtul_net_handle_data_init()
{
	rest_recv_buf_str();
	init_net_info();
	return 0;
}
int virtul_net_handle_data_entery()
{
	return virtual_net_handle_data();
}