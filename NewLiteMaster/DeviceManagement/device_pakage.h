#ifndef _DEVICE_PAKAGE_H_
#define _DEVICE_PAKAGE_H_
#include "device_info.h"
/******************************************
*��������ö��Ҫ����
******************************************/
enum
{
	CC1101_MTOS_GET_CARD = 0,
	CC1101_MTOS_MATCH,
	CC1101_MTOS_SETADDR,
	CC1101_MTOS_SET_CARD_CNT,
};
enum
{
	CC1101_COMM_REPORT_SIG = 0x40,
};
enum
{
	CC1101_STOM_CARD = 0x80,
	CC1101_STOM_SETADDR,
	CC1101_STOM_SET_CARD_CNT,
};
/******************************************/


enum
{
	DEVICE_MODE_MATCH,
	DEVICE_MODE_SCAN,
	DEVICE_MODE_NORMAL
};


extern int cc1101_master_pakage_mtom_request(char *buf,int buf_len,struct master_device_t *pmaster);
extern int cc1101_master_pakage_mtom_respond(char *buf,int buf_len,int cseq,char *dst_id,int dst_id_len);
extern int cc1101_master_pakage_mtos_respond(char *buf,int buf_len,int cseq,struct master_device_t *pmaster);
extern int cc1101_master_pakage_match_id(char *buf,int buf_len,struct master_device_t *pmaster);
extern int cc1101_master_pakage_match_set_addr(char *buf,int buf_len,struct master_device_t *pmaster,struct sub_device_t *psub);
extern int cc1101_master_pakage_get_card(char *buf,int buf_len,struct master_device_t *pmaster,struct sub_device_t *psub);	
extern int cc1101_sub_pakage_stom_request(char *buf,int buf_len,struct master_device_t *pmaster,struct sub_device_t *psub);
extern int cc1101_sub_pakage_match_id(char *buf,int buf_len,struct master_device_t *pmaster,struct sub_device_t *psub);
extern int cc1101_sub_pakage_send_card(char *buf,int buf_len,struct master_device_t *pmaster,struct sub_device_t *psub,
	char *card,int card_cnt);

#endif
