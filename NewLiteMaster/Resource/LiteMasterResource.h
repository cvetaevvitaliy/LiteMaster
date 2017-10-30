#ifndef _LITE_MASTER_RESOURCE_H_
#define _LITE_MASTER_RESOURCE_H_
extern int init_device_info();
extern int get_device_id(char *buf,int len);
extern int get_device_version(char *buf,int len);
extern int get_device_password(char *buf,int len);
extern void get_cpu_id(char *buf,int len);
#endif
