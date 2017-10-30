#include "sim.h"
#include "virtual_net.h"
#include "sz_libc.h"
#include "msg_sim.h"
#include "virtual_net_handle_data.h"
//static const unsigned char res_sim_read[]="+CPIN: READY";
//static const unsigned char res_no_valid_car[]= "NO CARRIER";
//static const unsigned char get_new_msg[]="+CMTI: \"SM\"";//1

//static const unsigned char selec_sv_num[] = "+CMGR: \"REC UNREAD\",\"+8613113638046\"";
//static const unsigned char filter[]="+CMGR: \"REC UNREAD\"";
//
//static const unsigned char _test_selec_sv_num[] = "+CMGR: \"REC READ\",\"+8613113638046\"";
//static const unsigned char _test_filter[]="+CMGR: \"REC READ\"";

static int is_our_msg = 0;
static unsigned char msg_index[4];

//static unsigned char is_not_my_msg[]="is not my message";
//static unsigned char is_my_msg[]="is my message";
//static int filter_flag=0-',';
static unsigned char _str_get_sgl_qa[]="+CSQ:";
//static unsigned char _str_ring[]="RING";
//static unsigned char _str_hang_down[]="NO CARRIER";
static char tmp;


struct sim_state_t
{
	int call_status;
	int sms_status;
	int sim_signal_qa;
	char imei_num[20];
	int imei_num_flg;
};
struct sim_state_t sim_info={0};

int sim_get_sig_qa()
{
	return sim_info.sim_signal_qa;
}
static void sim_set_sig_qa(int new_sig_qa)
{
	sim_info.sim_signal_qa = new_sig_qa;
}

void sim_set_call_status(int flg)
{
	sim_info.call_status = flg;
}

int sim_get_call_status()
{
	return sim_info.call_status;
}

void sim_set_sms_status(int flg)
{
	sim_info.sms_status = flg;
}

int sim_get_sms_status()
{
	return  sim_info.sms_status;
}


void sim_set_imei_num(char *buf,int len)
{
	if(len>19)
	{
		len = 19;
	}
	sz_memset(sim_info.imei_num,0,20);
	sim_info.imei_num_flg = len;
	sz_memcpy(sim_info.imei_num, buf,len);
}

int sim_get_imei_num(char *buf,int len)
{
	if(buf == 0 || len <0 || sim_info.imei_num_flg == 0) 
	{
		return -1;
	}
//	if(sim_info.imei_num_flg = 0)
//	{
//		return 0;
//	}
    sz_memset(buf,0,len);
    len = len -1;
	if(len > sim_info.imei_num_flg)
	{
		len = sim_info.imei_num_flg;
	}
	sz_memset(buf,0,len);
	sz_memcpy(buf,sim_info.imei_num,len);
	return len;
}

void sim_msg_proc(const char *buf,int len)
{
//	int lres;
    static int print_res = 0;
    static int recv_flg = 0;
	
	if(print_res == 1)
	{
		print_res = 0;
		sz_printf("%s\r\n",buf);
	}
	else if(print_res == 2)
	{
		print_res = 0;
		sim_set_imei_num((char*)buf,len);
		sz_printk("Get IMEI NUM.(%s [%d]) OK\n",buf,len);
	}
	//sz_printk("len:%d,buf:%s\n",len,buf);

	if(sz_kmp_find_string(buf,"NOT INSERTED")>0) 
	{
		virtual_net_set_net_status(VIR_NET_DUMM);
	}
	else if(sz_kmp_find_string(buf,"AT+GSN")>0) 
	{
		
		print_res = 2;
	}
	else if(sz_strcmp((char *)buf,(char *)_str_get_sgl_qa)==' ')
	{
		int sim_qa;
		sim_qa = buf[6]-'0';
		if(buf[7]!=',')
		   sim_qa = sim_qa*10+(buf[7]-'0');
		sim_set_sig_qa(sim_qa);
	}
//	else if(sz_strcmp((char *)buf,(char *)_str_ring)==0)
//	{
//		sz_printk("len:%d,buf:%s\n",len,buf);
//		respond_sim();
//	}
//	else if(sz_strcmp((char *)buf,(char *)_str_hang_down)==0)
//	{
//		sz_printk("len:%d,buf:%s\n",len,buf);
//		cancle_sim();
//	}
    else if(sz_kmp_find_string(buf,"AT+CPIN")>0)
	{
		//sz_printf("%s ",buf);
		print_res = 1;
		//sz_printk("222\n");
	}
	else if(sz_kmp_find_string(buf,"AT+CIPSEND=")>0)
	{
		virtual_net_send();
		//sz_printk("222\n");
	}
	else if(sz_kmp_find_string(buf,"SEND OK")>0)
	{
		virtual_net_send_ok();
		//sz_printk("111\n");
	}
	else if(sz_kmp_find_string(buf,"+CIPRXGET: 1")>0)
	{
		//virtual_net_send_ok();
		//sz_printk("111\n");
		virtual_net_recv_commit();
	}
	else if(sz_kmp_find_string(buf,"+CIPRXGET: 2,")>0)
	{
		//virtual_net_send_ok();
		char num_left[4]; 
		//buf+=sz_strlen(const char * s)
		const char *end = buf+len;
		const char *p_s = 0;
		const char *p_e = 0;
		const char *pos = buf;
		unsigned char ch;
		int tmp_len;

		sz_memset(num_left,0,4);
		//sz_printk("len:%d,buf:%s\n",len,buf);
		while(1)
		{
			ch = *pos;
			if(ch==0x00)
				return;
			if( ch == ',')
			{
				++pos;
				p_s = pos;
				break;
			}
			++pos;
		}

		while(1)
		{
			ch = *pos;
			if(ch==0x00)
				return;
			if( ch == ',')
			{
				p_e = pos-1;
				++pos;
				break;
			}
			++pos;
		}

		if(p_s ==0 || p_e == 0)
			return ;
		tmp_len = p_e - p_s + 1;
		if(tmp_len<4)
		{
			sz_memset(num_left,0,4);
			sz_memcpy(num_left, (char*)p_s, tmp_len);
			//sz_printk("%s\n",num_left);
			tmp_len = sz_strtoul(num_left,0,10);
			if(tmp_len==0)
			{
				sz_printk("recv null\n");
			}
			else
			{  
				recv_flg = 1;
				//_printk("recv date len:%d\n",tmp_len);
			}
		}
		p_s = p_e+2;
		p_e = 0;
		while(1)
		{
			ch = *pos;
			if(ch==0x00)
			{
				p_e = pos-1;
				break;

			}
			++pos;
		}

		tmp_len = p_e - p_s + 1;
		if(tmp_len<4)
		{
			sz_memset(num_left,0,4);
			sz_memcpy(num_left, (char*)p_s, tmp_len);
			//sz_printk("%s\n",num_left);
			tmp_len = sz_strtoul(num_left,0,10);
			if(tmp_len==0)
			{
				//_printk("recv end\n");
		        virtual_net_recv_end();
				//recv_flg = 0;
			}
			else
			{  
				//_printk("recv left:%d\n",tmp_len);
				sim_recv();
			}
		}
		
	}
	else if(sz_strcmp((char*)buf,"Call Ready")==0)
	{
		sz_printk("%s\n",buf);
		sim_set_call_status(SIM_SMS_CALL_READY);
	}
	else if(sz_strcmp((char*)buf,"SMS Ready")==0)
	{
		sz_printk("%s\n",buf);
		sim_set_sms_status(SIM_SMS_CALL_READY);
		sim_set_cipmux();
		//device can connect the server now
		virtual_net_connect();
	}
	else if(sz_strcmp((char*)buf,"CONNECT OK")==0)
	{
		sz_printk("%s\n",buf);
		virtual_net_set_net_status(VIR_NET_CONN_OK);
	}
	else if(sz_strcmp((char*)buf,"CLOSED")==0)
	{
		sz_printk("%s\n",buf);
		virtual_net_set_net_status(VIR_NET_DIS_CONN);
		recv_flg = 0;
		//device connect the server
		virtual_net_connect();
	}
	else if(sz_strcmp((char*)buf,"ALREADY CONNECT")==0)
	{
		sz_printk("%s\n",buf);
		virtual_net_set_net_status(VIR_NET_CONN_OK);
	}
	else
	{
		if(recv_flg == 1)
		{
			struct vir_net_recv_buf_t tmp_;
			//sz_printk("cpy to recvbuf:%d\n",len);
			//sz_printk("ip_recv:%s\n",buf);
			get_recv_buf_str(&tmp_);
			if((*tmp_.len+len)<tmp_.max_len)
			{
    			sz_memcpy(tmp_.buf+*tmp_.len,(char*)buf,len);
    			*tmp_.len+=len;
			}
			else
			{
				*tmp_.len = 0;
				sz_printk("recv buf is full\n");
			}
			//sz_printk(buf);
            recv_flg = 0;
		}
	}
	#if 0
	if(sz_strcmp((unsigned char *)get_new_msg,buf)==filter_flag)
	{
		read_msg("-i",(char*)(buf+12));
		my_mem_clear((char*)msg_index,2);
		my_memcpy((char*)msg_index,(char*)buf,2);
		return ;
	}
	
	if(sz_strcmp((unsigned char *)filter,buf)==filter_flag)
	{
		if(my_strcmp((unsigned char *)selec_sv_num,buf)==filter_flag)
		{
			is_our_msg = 1;
		}
		else
		{
			is_our_msg = 2;
		}
		return;
	}
	
	lres = sz_strcmp((unsigned char *)_test_filter,buf);
	if(lres==filter_flag)
	{
		if(sz_strcmp((unsigned char *)_test_selec_sv_num,buf)==filter_flag)
		{
			is_our_msg = 1;
		}
		else
		{
			is_our_msg = 2;
		}
		return;
	}
	
	if(is_our_msg == 1)
	{
		is_our_msg = 0;
	}
	else if(is_our_msg == 2)
	{
		is_our_msg = 0;
	}
	
	#endif
}
