#ifndef _DEVICE_INFO_H_
#define _DEVICE_INFO_H_

enum
{
	SUB_DEVICE_OFF_LINE = 0,
	SUB_DEVICE_ON_LINE ,
	SUB_DEVICE_LINE_END
};

enum
{
	MASTER = 0,
    SUB_DEVICE_NUM_1,
    SUB_DEVICE_NUM_2,
    SUB_DEVICE_NUM_3,
    SUB_DEVICE_NUM_4,
    SUB_DEVICE_ALL,
};

struct sub_device_card_t
{
	char card[63];
	short cur;
	short max;
};

struct sub_device_t
{
	int cseq;
	short subaddr;
	char master_local_id[12];
	short master_crc16_id;
	struct sub_device_card_t st_card;
	int sub_device_oline_flg;
	int position_id;
	int channel;
	int card_cnt;
};

struct master_device_t
{
	int cseq;
	int master_id;
	char master_local_id[12];//chip_id
	int cc1101_freq;
	int masteraddr;
	int channel;
};

extern int device_init_sys_info();
extern void set_device_into_match();
extern void set_device_attr(int attr);
extern int get_device_attr();
extern struct sub_device_t *get_subdevice(int which);
extern struct master_device_t *get_masterdevice();

extern void reset_online_status();
extern int get_subdevice_online_status(int which);
extern void set_subdevice_online_status(int which,int new_state);
extern void update_online_status();
extern void get_all_subdev_online_status(int *status,int cnt);
extern void print_config();
#endif
