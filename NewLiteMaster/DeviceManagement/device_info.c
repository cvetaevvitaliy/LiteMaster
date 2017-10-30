#include "device_info.h"
#include "LiteMasterResource.h"
#include "sz_libc.h"
#include "LiteMasterResource.h"
#include "inner_flash.h"
#include "module_common.h"

static struct master_device_t master_device;
static struct sub_device_t  sub_device[4];
static int device_attr;
static int online[4];

int device_init_sys_info()
{
	char buf[4];
	get_cpu_id(master_device.master_local_id,12);
	master_device.cseq = 0;
	master_device.master_id = 0x001;
	inner_read_dev_addr(MASTER,buf,4);
	master_device.masteraddr =sz_ctoi(buf,4)&0xFF;
    master_device.channel = 8;
		
    inner_read_dev_addr(SUB_DEVICE_NUM_1,buf,4);
	sub_device[0].subaddr=sz_ctoi(buf,4)&0xFF;
	sub_device[0].cseq = 0x28;
    sub_device[0].position_id = SUB_DEVICE_NUM_1;
	
	inner_read_dev_addr(SUB_DEVICE_NUM_2,buf,4);
	sub_device[1].subaddr=sz_ctoi(buf,4)&0xFF;
	sub_device[1].cseq = 0x38;
    sub_device[1].position_id = SUB_DEVICE_NUM_2;
	
	inner_read_dev_addr(SUB_DEVICE_NUM_3,buf,4);
	sub_device[2].subaddr=sz_ctoi(buf,4)&0xFF;
	sub_device[2].cseq = 0x38;
	sub_device[2].position_id = SUB_DEVICE_NUM_3;

	inner_read_dev_addr(SUB_DEVICE_NUM_4,buf,4);
	sub_device[3].subaddr=sz_ctoi(buf,4)&0xFF;
	sub_device[3].cseq = 0x38;
	sub_device[3].position_id = SUB_DEVICE_NUM_4;


    sz_printk("master addr=%#x\n",master_device.masteraddr);
	sz_printk("pos1 addr=%#x\n",sub_device[0].subaddr);
	sz_printk("pos2 addr=%#x\n",sub_device[1].subaddr);
	sz_printk("pos3 addr=%#x\n",sub_device[2].subaddr);
	sz_printk("pos4 addr=%#x\n",sub_device[3].subaddr);
}

void set_device_into_match()
{
	int i;
	int res;
	get_cpu_id(master_device.master_local_id,12);
	res = Crc16Result(master_device.master_local_id,12);
	res = res%256;
	master_device.cseq = 0;
	if(res == 0 ||res > 250)
	{
		master_device.masteraddr = 0x128;
	}
	else
	{
		master_device.masteraddr = res;
		inner_write_dev_addr(MASTER,(char*)&res,4);
	}
	
	for(i = 0;i<4;i++)
	{
		res++;
		sub_device[i].cseq = 128;
		sub_device[i].subaddr = res;
        inner_write_dev_addr(i+1,(char*)&res,4);
	}

	
}

//void set_device_attr(int attr)
//{
//	device_attr = attr;
//}
//
//int get_device_attr()
//{
//	return device_attr;
//}

struct sub_device_t *get_subdevice(int which)
{
	switch(which)
	{
		case SUB_DEVICE_NUM_1:
			return &sub_device[0];
		case SUB_DEVICE_NUM_2:
			return &sub_device[1];
		case SUB_DEVICE_NUM_3:
			return &sub_device[2];
		case SUB_DEVICE_NUM_4:
			return &sub_device[3];
		default:
			return 0;
	}
}

struct master_device_t *get_masterdevice()
{
	return &master_device;
}

void reset_online_status()
{
	online[0] = SUB_DEVICE_OFF_LINE;
	online[1] = SUB_DEVICE_OFF_LINE;
	online[2] = SUB_DEVICE_OFF_LINE;
	online[3] = SUB_DEVICE_OFF_LINE;
}

int get_subdevice_online_status(int which)
{
	switch(which)
	{
		case SUB_DEVICE_NUM_1:
			return online[0];
		case SUB_DEVICE_NUM_2:
			return online[1];
		case SUB_DEVICE_NUM_3:
			return online[2];
		case SUB_DEVICE_NUM_4:
			return online[3];
		default:
			return SUB_DEVICE_LINE_END;
	}
}

void set_subdevice_online_status(int which,int new_state)
{
	//sz_printk("set pos:%d(status:%d)\n",which,new_state);
	switch(which)
	{
		case SUB_DEVICE_NUM_1:
			online[0]= new_state;
			break;
		case SUB_DEVICE_NUM_2:
			online[1]= new_state;
			break;
		case SUB_DEVICE_NUM_3:
			online[2]= new_state;
			break;
		case SUB_DEVICE_NUM_4:
			online[3]= new_state;;
			break;
		default:
			break;
	}
}

void update_online_status()
{
	int i;
	for(i=0;i<4;i++)
	{
		sub_device[i].sub_device_oline_flg = online[i];
	}
}

void get_all_subdev_online_status(int *status,int cnt)
{
	int i;
	for(i=0;i<4;i++)
	{
		status[i]=sub_device[i].sub_device_oline_flg;
	}
}



void print_config()
{   
	sz_printf("**********************************************\r\n");
	sz_printf("master addr   :%#x\r\n",master_device.masteraddr);
    sz_printf("master channel:%d\r\n",master_device.channel);
	sz_printf("**********************************************\r\n");
}