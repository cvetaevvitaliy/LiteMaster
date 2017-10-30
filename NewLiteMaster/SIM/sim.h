#ifndef _SIM900A_H_
#define _SIM900A_H_
#include "ring_buffer.h"
extern void init_sim();
extern int check_sim_state();
extern void open_sim();
extern void close_sim();
extern void asy_sim();
extern void call_test_sim();
extern void respond_sim();
extern void cancle_sim();
extern void sim_entry();
extern void set_sim_recall_fun(void (*func)(int device,char *buf,int len));
extern void check_sim_card_sim();
extern void check_sim_volum_sim();
extern int set_sim_volum_sim(int volum);
extern void	get_my_phone_num();
extern void send_one_msg(const char *msg,int len);
extern void read_msg(char *mode,char *src);
extern void delete_msg(char *mode,char *src);
extern void get_mute_statue();
extern void test_audio();
extern void stop_test_audio();
extern void get_cmic_statue();
extern void get_singal_QA();
extern void sim_timer_run();
extern void sim_timer_stop();
extern void sim_check_creg_net();
extern void sim_check_cgreg_net();
extern void sim_reg_creg_net();
extern void sim_reg_cgreg_net();
extern void sim_connect_server(const char *conn_cmd);
extern void sim_close_server();
extern void sim_set_cipmux();
extern void sim_send_len(int len);
extern void sim_send(const char *src,int len);
extern void sim_recv();
extern void set_sim_write_cb(int(*cb_)(char *src,int len));
extern void set_sim_read_cb(int(*cb_)(char *src,int len));
extern void sim_get_gloable_object_id();
extern void sim_reopen();
extern int sim_send_cmd(const char *src);
#endif