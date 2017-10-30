#ifndef _VIRTUAL_NET_H_
#define _VIRTUAL_NET_H_
#include "list.h"
struct virtual_net_que_t
{
	char *prt;
	int len;
	int max_len;
	struct list_head m_list;
};
enum
{
	VIR_NET_DUMM = 0,
	VIR_NET_DIS_CONN = 1,
	VIR_NET_CONN_ING,
	VIR_NET_CONN_OK,
};
extern int virtual_net_init();
extern int virtual_net_connect();
extern int virtual_net_send_t(struct virtual_net_que_t *p_vir_que);
extern int virtual_net_send();
extern int virtual_net_send_ok();
extern int virtual_net_get_send_buf(struct virtual_net_que_t **p_vir_que);
extern void virtual_net_release_send_buf(struct virtual_net_que_t *p_vir_que);
extern int virtual_net_recv_commit();
extern int virtual_net_recv_end();
extern void  virtual_net_set_net_status(int flg);
extern int virtual_net_get_net_status();
extern int virtual_net_is_work();
extern void virtual_net_reinit_send_buf();
extern void virtual_net_thread_start(int flg);
extern int virtual_net_close();
extern void virtual_net_heart_beat_respond();
extern void virtual_net_stop();
#endif
