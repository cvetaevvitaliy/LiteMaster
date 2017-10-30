#ifndef _VIRTUAL_NET_HANDLE_DATA_H_
#define _VIRTUAL_NET_HANDLE_DATA_H_

typedef struct vir_net_recv_buf_t
{
	char *buf;
	int *len;
	int max_len;
}Vir_net_recv_buf;

extern int virtul_net_handle_data_init();
extern int virtul_net_handle_data_entery();
extern void get_recv_buf_str(struct vir_net_recv_buf_t *pvir_net_recv_buf);
extern void rest_recv_buf_str();
extern void virtual_net_print_state();
extern void virtual_net_set_online(int flg);
extern void virtual_net_set_handshake(int flg);
extern int virtual_net_get_conn();
extern void virtual_net_inc_conn();
#endif
