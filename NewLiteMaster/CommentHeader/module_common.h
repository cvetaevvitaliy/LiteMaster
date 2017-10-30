#ifndef _MODULE_COMMON_H_
#define _MODULE_COMMON_H_
#include "sz_libc.h"

#define GO_PREV_STEP(curr_step)((curr_step)--)
#define GO_NEXET_STEP(curr_step)((curr_step)++)
#define GOTO_STEP(curr_step,new_curr) ((curr_step)=(new_curr))
#define DRUMP_STEP(curr_step,tedal) ((curr_step)+=(tedal))

enum
{
	MODULE_STA_STOP = 0,
	MODULE_STA_WORK,	
};
enum
{
	LITECTRL_NORMAL_WORK = 0,
	LITECTRL_FIRST_RUNNING,
	LITECTRL_NORMAL_WORK_DBG,
	LITECTRL_BOARD_CHEKING,
	LITECTRL_PRODUCE_CHEKING,
	
//	LITECTRL_MAN,
//	LITECTRL_SCAN_MODE,
//	LITECTRL_SCAN_MODE_MASTER,
//	LITECTRL_SCAN_MODE_SUBDEVICE,
};

enum
{
	DEVICE_UNINIT=0,
	DEVICE_INIT,
};

enum
{
	CHANEL_UNFIX = 0,
	CHANEL_FIX ,
};
 
extern int Crc16Result(char *data, int length);
extern void printf_fifo(char *buf,int len);
#endif
