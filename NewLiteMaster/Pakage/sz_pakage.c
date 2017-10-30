#include "sz_pakage.h"
#include "sz_libc.h"
#include "LiteMasterResource.h"
const char head[] ={ 0xaa, 0xab, 0xac, 0xbb, 0xba, 0xbc, 0xcc, 0xca, 0xcb, 0xdd, 0xda, 0xdb, 0xee, 0xea, 0xff, 0xfa };
const char tail[] ={ 0xae, 0xbe, 0xce, 0xde, 0x0a, 0x0b, 0x0c, 0x0d};

static int find_flg_string(char *buf,int len,char **pos_end,const char *flg_str,int flg_str_len)
{
	char *buf_end = buf+len;
	const char *p_flg = flg_str;
	int found_len = 0;
	while(buf<buf_end)
	{
		if(*buf==*p_flg)
		{
			++buf;
			++p_flg;
			++found_len;
			if(found_len>=flg_str_len)
			{
				if(pos_end!=0)
				{
					*pos_end = buf - flg_str_len;
				}
				return 0;
			}
			else
			{
				continue;
			}
		}
		if(found_len == 0)
		{
			++buf;
		}
		else
		{
			found_len = 0;
			p_flg = flg_str;
		}
	}
	return -1;
}

static int sz_pagkage_find_head(char *buf,int len,char **pos_end)
{
	return find_flg_string(buf,len,pos_end,head,sizeof(head));
}
static int sz_pagkage_find_tail(char *buf,int len,char **pos_end)
{
	return find_flg_string(buf,len,pos_end,tail,sizeof(tail));
}

int sz_un_pakage(char *buf,int len,struct protecol_head_t **protcol,struct pakage_str_t *p_pakage)
//char **pakage_s,char **pakage_e)
{
	int ret;
	char *pos;
	char *buf_end = buf+len;
	char *plauf;
	Protecol_head *p_prt_head;
	char *p_non;
	int pakage_data_len;
	//int comment_len;
    //sz_printk("recive data:%d,str:%d\n",len,sizeof(Protecol_head));
	if(len<(sizeof(head)+sizeof(Protecol_head)+sizeof(tail)))
		return -1;
	ret = sz_pagkage_find_head(buf,len,&pos);
	if(ret == -1)
		return NO_HEAD;
	p_prt_head = (Protecol_head *)(pos+sizeof(head));
	pakage_data_len = sz_htonl(p_prt_head->dataLength);
	p_non = pos+sizeof(head)+sizeof(Protecol_head)+pakage_data_len;
	//sz_printk("Pro_len:%d\n",pakage_data_len);
	if(pakage_data_len<0)
		return NO_HEAD;
	if((p_non+sizeof(tail))>buf_end)
	{
		return BUF_NO_ENOUGH;
	}
	ret = sz_pagkage_find_tail(p_non,buf_end-p_non,&plauf);
	if(ret == -1)
	{
		return NO_TAIL;
	}
	if(p_pakage)
	{
		p_pakage->pakage_s = pos;
		p_pakage->pakage_e = plauf + sizeof(tail);
		
		p_pakage->pos_s = pos+sizeof(head)+sizeof(Protecol_head);
		p_pakage->pos_e = plauf;
	}
	if(protcol)
	{
		
*protcol = (struct protecol_head_t *)(pos+sizeof(head));
	}
	return 0;
}
//#define _memcpy(a,b,c) do{}while(0)


void init_protocol_head(Protecol_head *p_pro,int commet_len,int flag)
{
	p_pro->dataLength =sz_htonl(commet_len);
	p_pro->transmiteFlag = 10;
	p_pro->deviceFlag = 1;
	p_pro->protocolFlag = flag;
	get_device_id((char*)p_pro->mac,12);
    p_pro->protocolVersion.Product = 0x01;
	p_pro->protocolVersion.Year =16;
	p_pro->protocolVersion.Month=03;
	p_pro->protocolVersion.Date = 30;
    
}

#define _memcpy(a,b,c) sz_memcpy(a,(char*)b,c)
int sz_pakage(char *buf,int buf_len,const char *comment,int commet_len,int flag)
{
    char *pos;
    Protecol_head *p_pro;
    if((sizeof(head)+sizeof(Protecol_head)+commet_len+sizeof(tail))>buf_len)
        return -1;
    pos = buf;
    _memcpy(pos,head,sizeof(head));
    pos+=sizeof(head);
    p_pro = (Protecol_head *)pos;
	init_protocol_head(p_pro,commet_len,flag);
    pos+=sizeof(Protecol_head);
    _memcpy(pos,comment,commet_len);
    pos+=commet_len;
    _memcpy(pos,tail,sizeof(tail));
    pos+=sizeof(tail);
    return pos - buf;
}
int sz_pakage_pre(char *buf,int buf_len,Protecol_head **pp_pro)
{
    char *pos;
    ;
    if(buf_len<(sizeof(head)+sizeof(Protecol_head)))
        return -1;
   pos = buf;
   _memcpy(pos,head,sizeof(head));
   pos+=sizeof(head);
   init_protocol_head((Protecol_head *)pos,0,0);
	if(pp_pro)
   *pp_pro = (Protecol_head *)pos;
    //
   pos+=sizeof(Protecol_head);
   return pos - buf;
}

int sz_pakage_aft(char *buf,int buf_len,char **pos_e)
{
    char *pos;
    if(buf_len<sizeof(tail))
        return -1;
   pos = buf;
   _memcpy(pos,tail,sizeof(tail));
   pos+=sizeof(tail);
   if(pos_e!=0)
        *pos_e = pos;
   return pos - buf;
}

void sz_pakage_len(char *addr,int len)
{
	if(((int)addr & 0x07)==0)
    {
		*(int*)addr = len;
	}
	else
	{
		char *ps = (char*)&len;
		*addr++ = *ps++;
		*addr++ = *ps++;
		*addr++ = *ps++;
		*addr++ = *ps++;
	}
}
