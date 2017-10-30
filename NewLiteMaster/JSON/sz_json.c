#include "sz_libc.h"
#include "sz_json.h"
static char *json_head = "\
{\n\
\"from\":\"device\",\n\
\"to\":\"client\",\n\
";

#define CHECK(res,left)do{if(res<0){return -1;}else{left=left-res;}}while(0)
const char *sub_online="Yes";
const char *sub_unonline="No";
#define JUDGE_ON_LINE(a) ((a)==0)?sub_unonline:sub_online
#define ONLINE ",\n\"online\":{\n\"East\":\"%s\",\n\"West\":\"%s\",\n\"South\":\"%s\",\n\"North\":\"%s\"\n}"
int json_pakge_heart_beat(char *buf,int len,int sig,int *onlist)
{
	char* pos;
	int cnt=0;
	pos = buf;
	cnt = sz_snprintf(pos,len,"%s",json_head);
	CHECK(cnt,len);
	pos+=cnt;
	cnt = sz_snprintf(pos,len,"\"message\":\"heart_beat\",\n");
	CHECK(cnt,len);
	pos+=cnt;

	cnt = sz_snprintf(pos,len,"\"data\":{\n");
	CHECK(cnt,len);
	pos+=cnt;
	
	cnt = sz_snprintf(pos,len,"\"sigal\":%d",sig);
	CHECK(cnt,len);
	pos+=cnt;
	if(onlist)
	{
    	cnt = sz_snprintf(pos,len,ONLINE,JUDGE_ON_LINE(onlist[0]),
    	JUDGE_ON_LINE(onlist[1]),JUDGE_ON_LINE(onlist[2]),JUDGE_ON_LINE(onlist[3]));
    	CHECK(cnt,len);
    	pos+=cnt;
	}
    cnt = sz_snprintf(pos,len,"\n}");
	CHECK(cnt,len);
	pos+=cnt;
	
	cnt = sz_snprintf(pos,len,"\n}");
	CHECK(cnt,len);
	pos+=cnt;
//	sz_printk(buf);
//	sz_printf("\r\n");
	return pos-buf;
}

int json_pakge_card(char *buf,int len,struct scard_t *card_list[],int card_list_cnt)
{
	char* pos;
	int cnt=0;
	int i;
	int c_cnt;
	int j;
	int card_cnt;
	char *pos_list;
	int ret_len=0;
	pos = buf;

	//sz_printk("json len:%d\n",len);
	ret_len = sz_snprintf(pos,len,"%s",json_head);
	CHECK(cnt,len);
	pos+=ret_len;
	ret_len = sz_snprintf(pos,len,"\"message\":\"card\",\n");
	CHECK(cnt,len);
	pos+=ret_len;

	cnt = sz_snprintf(pos,len,"\"data\":{\n");
	CHECK(cnt,len);
	pos+=cnt;
	
	if(card_list_cnt > 1)
	{
    	c_cnt = card_list_cnt-1;
    	for(i=0;i<c_cnt;i++)
    	{
				
				switch(card_list[i]->pos)
				{
					case 1:
						cnt = sz_snprintf(pos,len,"\"%s\":[\n","East");
						break;
					case 2:
						cnt = sz_snprintf(pos,len,"\"%s\":[\n","South");
						break;
					case 3:
						cnt = sz_snprintf(pos,len,"\"%s\":[\n","West");
						break;
					case 4:
						cnt = sz_snprintf(pos,len,"\"%s\":[\n","North");
						break;
				}
   
        	CHECK(cnt,len);
        	pos+=cnt;
		
    		

			//sz_printk("card_cnt:%d\n",card_cnt);
			if(card_cnt<0)
			{
				cnt = sz_snprintf(pos,len,"%d\n],\n",-1);
        	    CHECK(cnt,len);
        	    pos+=cnt;
			}
			else
			{
        		for(j=0;j<card_cnt;j++)
        		{
            		cnt = sz_snprintf(pos,len,"%d,",pos_list[j]);
            		CHECK(cnt,len);
            		pos+=cnt;
        		}
        		cnt = sz_snprintf(pos,len,"%d\n],\n",pos_list[j]);
        	    CHECK(cnt,len);
        	    pos+=cnt;
			}
	    }   

		
	switch(card_list[i]->pos)
				{
					case 1:
						cnt = sz_snprintf(pos,len,"\"%s\":[\n","East");
						break;
					case 2:
						cnt = sz_snprintf(pos,len,"\"%s\":[\n","South");
						break;
					case 3:
						cnt = sz_snprintf(pos,len,"\"%s\":[\n","West");
						break;
					case 4:
						cnt = sz_snprintf(pos,len,"\"%s\":[\n","North");
						break;
				}
   
        	CHECK(cnt,len);
        	pos+=cnt;
    	CHECK(cnt,len);
    	pos+=cnt;
		pos_list = card_list[i]->src;
		card_cnt = card_list[i]->cnt;
		//sz_printk("card_cnt:%d\n",card_cnt);
		if(card_cnt<0)
		{
    		cnt = sz_snprintf(pos,len,"%d\n]\n",-1);
    	    CHECK(cnt,len);
    	    pos+=cnt;
		}
		else
		{
			for(j=0;j<card_cnt;j++)
    		{
        		cnt = sz_snprintf(pos,len,"%d,",pos_list[j]);
        		CHECK(cnt,len);
        		pos+=cnt;
    		}
    		cnt = sz_snprintf(pos,len,"%d\n]\n",pos_list[j]);
    	    CHECK(cnt,len);
    	    pos+=cnt;
		}
	}
	else if(card_list_cnt==1)
	{
		switch(card_list[i]->pos)
				{
					case 1:
						cnt = sz_snprintf(pos,len,"\"%s\":[\n","East");
						break;
					case 2:
						cnt = sz_snprintf(pos,len,"\"%s\":[\n","South");
						break;
					case 3:
						cnt = sz_snprintf(pos,len,"\"%s\":[\n","West");
						break;
					case 4:
						cnt = sz_snprintf(pos,len,"\"%s\":[\n","North");
						break;
				}
    	CHECK(cnt,len);
    	pos+=cnt;
		pos_list = card_list[i]->src;
		card_cnt = card_list[i]->cnt-1;
		for(j=0;j<card_cnt;j++)
		{
    		cnt = sz_snprintf(pos,len,"%d,",pos_list[j]);
    		CHECK(cnt,len);
    		pos+=cnt;
		}
		cnt = sz_snprintf(pos,len,"%d\n]}\n",pos_list[j]);
	    CHECK(cnt,len);
	    pos+=cnt;
	}
    
	cnt = sz_snprintf(pos,len,"\n}");
	CHECK(cnt,len);
	pos+=cnt;
	
	cnt = sz_snprintf(pos,len,"\n}");
	CHECK(cnt,len);
	pos+=cnt;

//	sz_printk(buf);
//	sz_printf("\r\n");
	return pos-buf;
}

//int json_pakge_report_card(char *buf,int len,struct card_info *card_list,int card_list_cnt)
//{
//	char* pos;
//	int cnt=0;
//	int i;
//	int c_cnt;
//	int j;
//	int card_cnt;
//	char *pos_lis;
//	int ret_len=0;
//	pos = buf;
//	ret_len = sz_snprintf(pos,len,"%s",json_head);
//	CHECK(cnt,len);
//	pos+=ret_len;
//	ret_len = sz_snprintf(pos,len,"\"message\":\"report_card_num\",\n");
//	CHECK(cnt,len);
//	pos+=ret_len;
//
//	cnt = sz_snprintf(pos,len,"\"data\":{\n");
//	CHECK(cnt,len);
//	pos+=cnt;
//
//	c_cnt = card_list_cnt-1;
//	for(i=0;i<c_cnt;i++)
//	{
//		cnt = sz_snprintf(pos,len,"\"%s\":%d,\n",card_list[i].position,card_list[i].card_cnt);
//    	CHECK(cnt,len);
//    	pos+=cnt;
//    }   
//	cnt = sz_snprintf(pos,len,"\"%s\":%d",card_list[i].position,card_list[i].card_cnt);
//	CHECK(cnt,len);
//	pos+=cnt;
//	    
//	cnt = sz_snprintf(pos,len,"\n}");
//	CHECK(cnt,len);
//	pos+=cnt;
//	
//	cnt = sz_snprintf(pos,len,"\n}");
//	CHECK(cnt,len);
//	pos+=cnt;
//
//    sz_printf("\r\n");
//	sz_printk(buf);
//	sz_printf("\r\n");
//	return pos-buf;
//}

int sz_get_element(const char *src,int len,const char *dst,char *msg_buf,int msg_len)
{
    const char *src_end = src+len;
    const char *tmp_ptr = dst;
    int cmd_len =sz_strlen(dst);
    int pos=0;
	  const char *b_;
    while(src<src_end)
    {
        if(*src == *tmp_ptr)
        {
            ++src;
            ++tmp_ptr;
            ++pos;
            if(pos == cmd_len)
            {
              goto have_found;
            }
            continue;
        }
        if(pos==0)
        {
            ++src;
        }
        else
        {

            tmp_ptr = dst;
            pos =0;
        }
    }
    return -1;
have_found:
	{
		char ch;
		char *dst_start = msg_buf;
		char *dst_end = msg_buf + msg_len;
        sz_memset(msg_buf,0,msg_len);
        while(src<src_end && msg_buf < dst_end)
        {
    		ch = *src;
			if(ch == ','||ch=='}')
				break;
			if(ch == ':' || ch =='"' || ch == ' ')
			{
				++src;
				continue;
			}
            *msg_buf = ch;
			++src;
			++msg_buf;
        }
		pos =  dst_start - msg_buf;
    }
    return pos;
}

