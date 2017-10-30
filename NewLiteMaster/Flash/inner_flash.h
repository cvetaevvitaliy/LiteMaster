#ifndef _INNER_FLASH_H_
#define _INNER_FLASH_H_
extern void inner_format_flash();
extern int inner_write_dev_flg(char*buf,int len);
extern int inner_read_dev_flg(char*buf,int len);
extern int inner_write_chanel(char*buf,int len);
extern int inner_read_chanel(char*buf,int len);
extern int inner_write_dev_addr(int pos_num,char*buf,int len);
extern int inner_read_dev_addr(int pos_num,char*buf,int len);
extern int innner_write_domain(int which,char *buf,int len);
extern int innner_read_domain(int which,char *buf,int len);
extern int innner_write_port(int which,char *buf,int len);
extern int innner_read_port(int which,char *buf,int len);
extern void inner_printk_t();
extern int inner_write_card_cnt(int which,char *buf,int len);
extern int inner_read_card_cnt(int which,char *buf,int len);
extern void inner_test_write();
extern int inner_write_device_id(char *buf,int len);
extern int inner_read_device_id(char *buf,int len);

#endif
