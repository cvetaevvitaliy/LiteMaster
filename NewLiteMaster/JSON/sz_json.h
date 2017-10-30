#ifndef _SZ_JSON_H_
#define _SZ_JSON_H_
#include "cards_store.h"

extern int json_pakge_card(char *buf,int len,struct scard_t *card_list[],int cnt);
extern int json_pakge_heart_beat(char *buf,int len,int sig,int *onlist);
extern int sz_get_element(const char *src,int len,const char *dst,char *msg_buf,int msg_len);
//extern int json_pakge_report_card(char *buf,int len,struct card_info *card_list,int card_list_cnt);
#endif
