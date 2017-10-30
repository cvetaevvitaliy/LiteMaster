#include "sz_pakage.h"
#include "LiteMasterResource.h"
#include "sz_libc.h"
#include "virtual_net.h"
#include "sz_json.h"
#include "cards_store.h"
#include "msg_sim.h"

#define CHECK_RES(res,offset) do{ if ((res)<0) return -1;offset+=res;}while(0)

int orient_pakage_hanshake(char *buf,int b_len)
{
	Protecol_head *p_prot;
	int ret = 0;
	int offset = 0;
	int data_len = 0;
	char *buf_end = buf + b_len;
	if(buf==0)
		return -1;
	
	ret = sz_pakage_pre(buf+offset,b_len-offset,&p_prot);
	CHECK_RES(ret,offset);
	//sz_printk("ret=%d,offset=%d\n",ret,offset);
	
	p_prot->protocolFlag = Hello;//握手包
	ret = get_device_version(buf+offset+4,b_len-offset-4);
	sz_pakage_len(buf+offset,ret);
	CHECK_RES(ret,offset);
	offset+=4;
	//sz_printk("ret=%d,offset=%d\n",ret,offset);
	data_len+=ret+4;

	ret = get_device_password(buf+offset+4,b_len-offset-4);
	sz_pakage_len(buf+offset,ret);
	CHECK_RES(ret,offset);
	offset+=4;
	//sz_printk("ret=%d,offset=%d\n",ret,offset);
	data_len+=ret+4;
    p_prot->dataLength = sz_htonl(data_len);
	
    ret = sz_pakage_aft(buf+offset,b_len-offset,0);
	CHECK_RES(ret,offset);
	//sz_printk("ret=%d,pakage_len=%d\n",ret,offset);
	return offset;
}


void send_handshake()
{
	struct virtual_net_que_t *p_que;
	int ret_len=0;
	int all_len=0;
	Protecol_head *p_pro;
	ret_len = virtual_net_get_send_buf(&p_que);
	if(ret_len == 0)
	{
		sz_memset(p_que->prt,0,p_que->max_len);
		ret_len = orient_pakage_hanshake(p_que->prt,p_que->max_len);
		p_que->len=ret_len;
    	virtual_net_send_t(p_que);
	}
}


static int orient_pakage_heartbeat(char *buf,int b_len)
{
	Protecol_head *p_prot;
	int ret = 0;
	int offset = 0;
	int data_len = 0;
	char *buf_end = buf + b_len;
	int subdev_online_status[4]={0};
	if(buf==0)
		return -1;
	
	ret = sz_pakage_pre(buf+offset,b_len-offset,&p_prot);
	CHECK_RES(ret,offset);
	//sz_printk("ret=%d,offset=%d\n",ret,offset);
	p_prot->protocolFlag = JSON_PAKAGE;//牌包
    get_all_subdev_online_status(subdev_online_status,4);
	ret = json_pakge_heart_beat(buf+offset,b_len-offset,sim_get_sig_qa(),subdev_online_status);
	CHECK_RES(ret,offset);
	//sz_printk("ret=%d,offset=%d\n",ret,offset);
	p_prot->dataLength = sz_htonl(ret);
		
    ret = sz_pakage_aft(buf+offset,b_len-offset,0);
	CHECK_RES(ret,offset);
	//sz_printk("ret=%d,pakage_len=%d\n",ret,offset);
	return offset;
}
//send_heatbeat;
void send_heart_beat()
{
	struct virtual_net_que_t *p_que;
	int ret_len=0;
	int all_len=0;
	Protecol_head *p_pro;
	ret_len = virtual_net_get_send_buf(&p_que);
	if(ret_len == 0)
	{
    	all_len = orient_pakage_heartbeat(p_que->prt,p_que->max_len);
    	p_que->len = all_len;
    	virtual_net_send_t(p_que);
	}
}


static int orient_pakage_cards(char *buf,int b_len)
{
	Protecol_head *p_prot;
	int ret = 0;
	int offset = 0;
	int data_len = 0;
	char *buf_end = buf + b_len;
	if(buf==0)
		return -1;
	
	ret = sz_pakage_pre(buf+offset,b_len-offset,&p_prot);
	CHECK_RES(ret,offset);
	//sz_printk("ret=%d,offset=%d\n",ret,offset);
	p_prot->protocolFlag = JSON_PAKAGE;//牌包

	//ret = json_pakge_card(buf+offset,b_len-offset,get_test_cards(),4);
	//ret = json_pakge_card(buf+offset,b_len-offset,get_cards(),4);
	CHECK_RES(ret,offset);
	//sz_printk("ret=%d,offset=%d\n",ret,offset);
	p_prot->dataLength = sz_htonl(ret);
		
    ret = sz_pakage_aft(buf+offset,b_len-offset,0);
	CHECK_RES(ret,offset);
	return offset;
}

void send_cards()
{
	struct virtual_net_que_t *p_que;
	int ret_len=0;
	int all_len=0;
	Protecol_head *p_pro;
	ret_len = virtual_net_get_send_buf(&p_que);
	if(ret_len == 0)
	{
		ret_len = orient_pakage_cards(p_que->prt,p_que->max_len);
		p_que->len=ret_len;
    	virtual_net_send_t(p_que);
	}
}




//static int orient_pakage_report_cards(char *buf,int b_len)
//{
//	Protecol_head *p_prot;
//	int ret = 0;
//	int offset = 0;
//	int data_len = 0;
//	char *buf_end = buf + b_len;
//	if(buf==0)
//		return -1;
//	
//	ret = sz_pakage_pre(buf+offset,b_len-offset,&p_prot);
//	CHECK_RES(ret,offset);
//	//sz_printk("ret=%d,offset=%d\n",ret,offset);
//	p_prot->protocolFlag = JSON_PAKAGE;//牌包
//
//	ret = json_pakge_report_card(buf+offset,b_len-offset,get_cards(),4);
//	CHECK_RES(ret,offset);
//	//sz_printk("ret=%d,offset=%d\n",ret,offset);
//	p_prot->dataLength = sz_htonl(ret);
//		
//    ret = sz_pakage_aft(buf+offset,b_len-offset,0);
//	CHECK_RES(ret,offset);
//	return offset;
//}


//static int is_have_new_card(struct card_info *p_card)
//{
//	static int old_cat_num[4]={0};
//	int ret;
//	ret =   (p_card[0].card_cnt & old_cat_num[0])|
//		    (p_card[1].card_cnt & old_cat_num[1])|
//		    (p_card[2].card_cnt & old_cat_num[2])|
//		    (p_card[3].card_cnt & old_cat_num[3]);
//	
//	old_cat_num[0] = ~p_card[0].card_cnt;
//	old_cat_num[1] = ~p_card[1].card_cnt;
//	old_cat_num[2] = ~p_card[2].card_cnt;
//	old_cat_num[3] = ~p_card[3].card_cnt;
//	//sz_printk("check card ret:%#x\n",ret);
//	return ret;
//}
//void send_report_cards()
//{
//	struct virtual_net_que_t *p_que;
//	int ret_len=0;
//	int all_len=0;
//	Protecol_head *p_pro;
//	if(is_have_new_card(get_cards())==0)
//	{
//		return ;
//	}
//	ret_len = virtual_net_get_send_buf(&p_que);
//	if(ret_len == 0)
//	{
//		ret_len = 0;//orient_pakage_report_cards(p_que->prt,p_que->max_len);
//		p_que->len=ret_len;
//    	virtual_net_send_t(p_que);
//	}
//}







