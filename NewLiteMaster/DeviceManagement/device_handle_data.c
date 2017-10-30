#include "device_pakage.h"
#include "device_info.h"
#include "cc110x_hal.h"
#include "sz_libc.h"
#include "Module_common.h"
#include "Device_mangement.h"
#include "cards_store.h"
#include "led.h"

static int l_cseq,l_bm_crc32;

static char cc1101_respond[4];

void init_respond()
{
	*(int*)(cc1101_respond) = 0;
}

int judge_all_respond()
{
	//sz_printk("respond:%#x\n",*(int*)cc1101_respond);
	return (((*(int*)(cc1101_respond))&(~0U))==(~0U));
}

static void set_respond(int witch)
{
	
	if(witch<0 && witch>3)
		return ;
	cc1101_respond[witch] = 0xFF;
}
static void handle_cards(int cseq,int pos,char *buf,int buf_len)
{
	struct sub_device_t *p_sub_dev;
    if(pos<SUB_DEVICE_NUM_1||pos>SUB_DEVICE_NUM_4)
    {
		sz_printk("recv error position\n");
		return ;
    }
    p_sub_dev = get_subdevice(pos);
    if(cseq == p_sub_dev->cseq)
    {
		push_card(pos,buf,buf_len);
        p_sub_dev->cseq ++;
		//sz_printk("pso:%d get %d cards:%#x\n",pos,buf_len,buf[0]);
    }
	set_subdevice_online_status(pos,SUB_DEVICE_ON_LINE);	
	SetLedState(pos,ALLAY_OPEN);
	set_respond(pos-1);
}
int cc1101_handle_data(char *buf,int buf_len)
{
	char *p_buf = buf;
	int addr;
	int cseq;
	int cmd;
    int pos;
	
	addr = (int)*p_buf; //one byte for addr
	++p_buf;
	cseq = sz_ctoi(p_buf,4);//four bytes for cseq
	p_buf+=4;
    cmd = (int)*p_buf;//one byte for cmd
	++p_buf;
	pos = (int)*p_buf;//one byte for position
	++p_buf;
	buf_len-=7;
	//sz_printk("cseq=%d,cmd=%#x,pos=%d\n",cseq,cmd,pos);
	set_the_chanel_flg(CHANEL_UNFIX);
	switch(cmd)
	{
		case CC1101_STOM_CARD:
			 handle_cards(cseq,pos,p_buf,buf_len);
			break;
		case CC1101_STOM_SETADDR:
			if(pos< SUB_DEVICE_NUM_1 || pos >SUB_DEVICE_NUM_4)
				return -1; 
			if(get_subdevice(pos)->cseq == cseq)
			{
				sz_printk("get pos:%d set addr respond\n",pos);
				set_respond(pos-1);
				get_subdevice(pos)->cseq++;
				SetLedState(pos,ALLAY_FLASH);
		    }
			break;
		case CC1101_COMM_REPORT_SIG:
			cc1101_set_tx_sig_strength((int)(*p_buf));
			break;
		default:
			break;
	}

	return 0;
}



