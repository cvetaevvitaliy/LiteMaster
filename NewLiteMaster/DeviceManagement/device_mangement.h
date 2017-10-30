#ifndef _DEVICE_MANAGEMENT_H_
#define _DEVICE_MANAGEMENT_H_
#include "device_pakage.h"
extern void device_init(/*int flg*/);
extern int device_setup(int mode,int channel);
extern int get_the_chanle_flg();
extern void set_the_chanel_flg(int new_state);
extern void set_match_flg();
extern int get_device_mode();
#endif
