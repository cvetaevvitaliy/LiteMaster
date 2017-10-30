#include "device_pakage.h"
#include "sz_libc.h"
int cc1101_master_pakage_match_id(char *buf,int buf_len,struct master_device_t *pmaster)
{
	char *p_buf = buf;
	
	*p_buf = 0x00;//or 0xff
	++p_buf;
	
	sz_itoc(p_buf,pmaster->cseq); 
	pmaster->cseq++;
	p_buf+=4;
		
	*p_buf = CC1101_MTOS_MATCH;
	++p_buf;

	*p_buf = 0;
	++p_buf;
	
	sz_memcpy(p_buf,pmaster->master_local_id,12);
	p_buf+=12;
	
	return p_buf-buf;
}

int cc1101_master_pakage_match_set_addr(char *buf,int buf_len,struct master_device_t *pmaster,struct sub_device_t *psub)
{
	char *p_buf = buf;
	
	*p_buf = 0x00;//or 0xff
	++p_buf;
	
	sz_itoc(p_buf,psub->cseq);
	p_buf+=4;
	
	*p_buf = CC1101_MTOS_SETADDR;
	++p_buf;

    *p_buf = (char)psub->position_id;
	++p_buf;
	//sz_printk("pos:%d\n",psub->position_id);
	sz_memcpy(p_buf,pmaster->master_local_id,12);
	p_buf+=12;
	
	*p_buf =(char) pmaster->masteraddr;
	++p_buf;
	
	*p_buf =(char) psub->subaddr;
	++p_buf;
	
	return p_buf-buf;
}


int cc1101_master_pakage_get_card(char *buf,int buf_len,struct master_device_t *pmaster,struct sub_device_t *psub)
{
	char *p_buf = buf;
	//sz_printk("get card masteraddr:%#x\n",pmaster->masteraddr);
	//sub addr
	//sz_printk("get card subaddr:%#x\n",psub->subaddr);
	*p_buf = (char)psub->subaddr;//
	++p_buf;
	//cseq
	sz_itoc(p_buf,psub->cseq);
	//sz_printk("sub %d cseq:%d\n",psub->position_id,psub->cseq);
	p_buf+=4;
	//cmd
	*p_buf = CC1101_MTOS_GET_CARD;
	++p_buf;
	//position
	*p_buf = (char)psub->position_id;
	++p_buf;
	//master id
	sz_memcpy(p_buf,pmaster->master_local_id,12);
	p_buf+=12;
	return p_buf-buf;
}



