#ifndef _MSG_SIM_H_
#define _MSG_SIM_H_
enum
{
	SIM_SMS_CALL_UNREADY = 0,
	SIM_SMS_CALL_READY,
};

extern void sim_msg_proc(const char *buf,int len);
extern int sim_get_sig_qa();
extern void sim_set_imei_num(char *buf,int len);
extern int sim_get_imei_num(char *buf,int len);
#endif
