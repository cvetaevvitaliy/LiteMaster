#include "LiteMasterResource.h"
#include "sz_libc.h"
#include "inner_flash.h"

struct litemaster_devic_t
{
	char device_mac[12];
    int device;
	int sub_device_online[4];
	char server_ip[64];
	char server_port[8];
	int f433;
	char version[16];
	char password[6];
};
static struct litemaster_devic_t device_info;
const char *_VERSION = "3.1.3.20170821";
const char *_PASSWORD= "111111111111111";
//const char *_DEVICE_ID="00000003622F";
const char *_DEVICE_ID="00000000001C";
int init_device_info()
{
	sz_memset((char*)&device_info,0,sizeof(struct litemaster_devic_t));
	sz_memcpy(device_info.version,(char*)_VERSION,sz_strlen(_VERSION));
	inner_read_device_id(device_info.device_mac,12);
	sz_printk("device id : %s\n",device_info.device_mac);
	return 0;
}
int get_device_id(char *buf,int len)
{
	if(buf == 0)
		return -1;
	if(len > 12)
        len = 12;
	
	sz_memcpy(buf,(char*)device_info.device_mac,len);
	return len;
}

int get_device_version(char *buf,int len)
{
	int ver_len = sz_strlen(_VERSION);
	if(buf == 0 || len < ver_len )
		return -1;
	sz_memcpy(buf,(char*)_VERSION,ver_len);
	return ver_len;
}
int get_device_password(char *buf,int len)
{
	int pass_len=sz_strlen(_PASSWORD);
	if(buf == 0 || len < pass_len )
		return -1;
	sz_memcpy(buf,(char*)_PASSWORD,pass_len);
	return pass_len;
}


void get_cpu_id(char *buf,int len)
{
    *(int*)buf=*(int*)(0x1ffff7e8);
    buf+=4;
    *(int*)buf=*(int*)(0x1ffff7ec);
    buf+=4;
    *(int*)buf=*(int*)(0x1ffff7f0);
    buf+=4;
}

int get_cpu_type()
{
	return *(int*)(0xE0042000);
}

//Kbytes
int get_cpu_frash_size()
{
	return *(int*)(0x1FFFF7E0);
}