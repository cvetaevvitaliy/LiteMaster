#include "stm32f10x_lib.h"
#include "stm32f10x_flash.h" 
#include "inner_flash.h"     
#include "sz_libc.h"
#include "module_common.h"
#include "device_info.h"
#define  STARTADDR  0x08010000                   	 
volatile FLASH_Status flash_status = FLASH_COMPLETE;    

//buf 地址要对4字节
static int flash_read(int addr_base, char *buf, int len) 
{
		short *p_addr = (short*)(STARTADDR + addr_base); 
		short *p_dst = (short*)buf;
		short *p_dst_end = p_dst+(len/2);
        while(p_dst < p_dst_end) 
		{
           *p_dst++=*p_addr++;
        }
        return len;
}



static void write_data(int addr_base,char *buf, int len)
{
	int p_addr_base = (int)(STARTADDR + addr_base);
	short *p_dst = (short*)buf;
	short *p_dst_end = p_dst + len/2;
	
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); 
    flash_status = FLASH_ErasePage(STARTADDR);
	
	for(;p_dst<p_dst_end && flash_status == FLASH_COMPLETE;)
	{
	    flash_status = FLASH_ProgramHalfWord(p_addr_base, *p_dst); 
			p_dst++;
			p_addr_base+=2;
	}
	
	FLASH_Lock();
} 

#define INNER_BASE_OFFSET(base,offset) ((base)+(offset))
const int FLASH_ADDR_BASE            =       0;
const int DEV_FLG_BASE          =       INNER_BASE_OFFSET(FLASH_ADDR_BASE,0);
const int MASTER_ADDR_BASE      =       INNER_BASE_OFFSET(DEV_FLG_BASE,4);
const int MASTER_CHANEL_BASE    =       INNER_BASE_OFFSET(MASTER_ADDR_BASE,4);
const int POSI_ADDR1            =       INNER_BASE_OFFSET(MASTER_CHANEL_BASE,4);
const int POSI_ADDR2            =       INNER_BASE_OFFSET(POSI_ADDR1,4);
const int POSI_ADDR3            =       INNER_BASE_OFFSET(POSI_ADDR2,4);
const int POSI_ADDR4            =       INNER_BASE_OFFSET(POSI_ADDR3,4);

const int DOMAIN_ADDR1          =       INNER_BASE_OFFSET(POSI_ADDR4,4);
const int DOMAIN_ADDR2          =       INNER_BASE_OFFSET(DOMAIN_ADDR1,64);
const int DOMAIN_ADDR3          =       INNER_BASE_OFFSET(DOMAIN_ADDR2,64);
const int DOMAIN_ADDR4          =       INNER_BASE_OFFSET(DOMAIN_ADDR3,64);
const int DOMAIN_ADDR5          =       INNER_BASE_OFFSET(DOMAIN_ADDR4,64);
const int DOMAIN_ADDR6          =       INNER_BASE_OFFSET(DOMAIN_ADDR5,64);

const int DOMAIN_PORT1          =       INNER_BASE_OFFSET(DOMAIN_ADDR6,64);
const int DOMAIN_PORT2          =       INNER_BASE_OFFSET(DOMAIN_PORT1,4);
const int DOMAIN_PORT3          =       INNER_BASE_OFFSET(DOMAIN_PORT2,4);
const int DOMAIN_PORT4          =       INNER_BASE_OFFSET(DOMAIN_PORT3,4);
const int DOMAIN_PORT5          =       INNER_BASE_OFFSET(DOMAIN_PORT4,4);
const int DOMAIN_PORT6          =       INNER_BASE_OFFSET(DOMAIN_PORT5,4);

//const int CARD_CNT              =       INNER_BASE_OFFSET(DOMAIN_PORT6,6);
const int CARD_CNT_POS1              =       INNER_BASE_OFFSET(DOMAIN_PORT6,4);
const int CARD_CNT_POS2              =       INNER_BASE_OFFSET(CARD_CNT_POS1,4);
const int CARD_CNT_POS3              =       INNER_BASE_OFFSET(CARD_CNT_POS2,4);
const int CARD_CNT_POS4             =       INNER_BASE_OFFSET(CARD_CNT_POS3,4);
const int DEV_ID_BASE                =  INNER_BASE_OFFSET(CARD_CNT_POS4,4);

const int MAX_LEN = 512;
static char tmp_buf[MAX_LEN];

int inner_write_dev_flg(char*buf,int len)
{
	flash_read(FLASH_ADDR_BASE,tmp_buf,MAX_LEN);
	sz_memcpy(tmp_buf+DEV_FLG_BASE, buf, 4);
	write_data(FLASH_ADDR_BASE,tmp_buf,MAX_LEN);
	return 4;
}
int inner_read_dev_flg(char*buf,int len)
{
	flash_read(DEV_FLG_BASE,buf,4);
	return 4;
}

int inner_write_chanel(char*buf,int len)
{
	flash_read(FLASH_ADDR_BASE,tmp_buf,MAX_LEN);
	sz_memcpy(tmp_buf+MASTER_CHANEL_BASE, buf, 4);
	write_data(FLASH_ADDR_BASE,tmp_buf,MAX_LEN);
	return 4;
}
int inner_read_chanel(char*buf,int len)
{
	flash_read(MASTER_CHANEL_BASE,buf,4);
	return 4;
}

int inner_write_dev_addr(int pos_num,char*buf,int len)
{
	flash_read(FLASH_ADDR_BASE,tmp_buf,MAX_LEN);
	if(pos_num<MASTER||pos_num>SUB_DEVICE_ALL)
		return -1;
	switch(pos_num)
	{
		case MASTER:
			sz_memcpy((char *)(tmp_buf+MASTER_ADDR_BASE),buf,4);
			break;
		case SUB_DEVICE_NUM_1:
			sz_memcpy((char *)(tmp_buf+POSI_ADDR1),buf,4);
			break;
		case SUB_DEVICE_NUM_2:
			sz_memcpy((char *)(tmp_buf+POSI_ADDR2),buf,4);
			break;
		case SUB_DEVICE_NUM_3:
			sz_memcpy((char *)(tmp_buf+POSI_ADDR3),buf,4);
			break;
		case SUB_DEVICE_NUM_4:
			sz_memcpy((char *)(tmp_buf+POSI_ADDR4),buf,4);
			break;
		default:
			break;
	}
	write_data(FLASH_ADDR_BASE,tmp_buf,MAX_LEN);
	return 2;
}

int inner_read_dev_addr(int pos_num,char*buf,int len)
{
	sz_memset(buf,0,len);
	switch(pos_num)
	{
		case MASTER:
			flash_read(MASTER_ADDR_BASE,buf,4);
			break;
		case SUB_DEVICE_NUM_1:
			flash_read(POSI_ADDR1,buf,4);
			break;
		case SUB_DEVICE_NUM_2:
			flash_read(POSI_ADDR2,buf,4);
			break;
		case SUB_DEVICE_NUM_3:
			flash_read(POSI_ADDR3,buf,4);
			break;
		case SUB_DEVICE_NUM_4:
			flash_read(POSI_ADDR4,buf,4);
			break;
		default:
			break;
	}
	return 2;
}
int innner_write_domain(int which,char *buf,int len)
{
	flash_read(FLASH_ADDR_BASE,tmp_buf,MAX_LEN);
	switch(which)
	{
		case 1:
			sz_memset((char *)(tmp_buf+DOMAIN_ADDR1),0,64);
			sz_memcpy((char *)(tmp_buf+DOMAIN_ADDR1),buf,len);
			break;
		case 2:
			sz_memset((char *)(tmp_buf+DOMAIN_ADDR2),0,64);
			sz_memcpy((char *)(tmp_buf+DOMAIN_ADDR2),buf,len);
			break;
		case 3:
			sz_memset((char *)(tmp_buf+DOMAIN_ADDR3),0,64);
			sz_memcpy((char *)(tmp_buf+DOMAIN_ADDR3),buf,len);
			break;
		case 4:
			sz_memset((char *)(tmp_buf+DOMAIN_ADDR4),0,64);
			sz_memcpy((char *)(tmp_buf+DOMAIN_ADDR4),buf,len);
			break;
		case 5:
			sz_memset((char *)(tmp_buf+DOMAIN_ADDR5),0,64);
			sz_memcpy((char *)(tmp_buf+DOMAIN_ADDR5),buf,len);
			break;
		case 6:
			sz_memset((char *)(tmp_buf+DOMAIN_ADDR6),0,64);
			sz_memcpy((char *)(tmp_buf+DOMAIN_ADDR6),buf,len);
			break;
		default:
			break;
	}
	write_data(FLASH_ADDR_BASE,tmp_buf,MAX_LEN);
	return len;
}

int innner_read_domain(int which,char *buf,int len)
{
	switch(which)
	{
		case 1:
			flash_read(DOMAIN_ADDR1,buf,len);
			break;
		case 2:
			flash_read(DOMAIN_ADDR2,buf,len);
			break;
		case 3:
			flash_read(DOMAIN_ADDR3,buf,len);
			break;
		case 4:
			flash_read(DOMAIN_ADDR4,buf,len);
			break;
		case 5:
			flash_read(DOMAIN_ADDR5,buf,len);
			break;
		case 6:
			flash_read(DOMAIN_ADDR6,buf,len);
			break;
		default:
			break;
	}
	return len;
}


int innner_write_port(int which,char *buf,int len)
{
	flash_read(FLASH_ADDR_BASE,tmp_buf,MAX_LEN);
	switch(which)
	{
		case 1:
			sz_memset((char *)(tmp_buf+DOMAIN_PORT1),0,4);
			sz_memcpy((char *)(tmp_buf+DOMAIN_PORT1),buf,len);
			break;
		case 2:
			sz_memset((char *)(tmp_buf+DOMAIN_PORT2),0,4);
			sz_memcpy((char *)(tmp_buf+DOMAIN_PORT2),buf,len);
			break;
		case 3:
			sz_memset((char *)(tmp_buf+DOMAIN_PORT3),0,4);
			sz_memcpy((char *)(tmp_buf+DOMAIN_PORT3),buf,len);
			break;
		case 4:
			sz_memset((char *)(tmp_buf+DOMAIN_PORT4),0,4);
			sz_memcpy((char *)(tmp_buf+DOMAIN_PORT4),buf,len);
			break;
		case 5:
			sz_memset((char *)(tmp_buf+DOMAIN_PORT5),0,4);
			sz_memcpy((char *)(tmp_buf+DOMAIN_PORT5),buf,len);
			break;
		case 6:
			sz_memset((char *)(tmp_buf+DOMAIN_PORT6),0,4);
			sz_memcpy((char *)(tmp_buf+DOMAIN_PORT6),buf,len);
			break;
		default:
			break;
	}
	write_data(FLASH_ADDR_BASE,tmp_buf,MAX_LEN);
	return len;
}

int innner_read_port(int which,char *buf,int len)
{
	switch(which)
	{
		case 1:
			flash_read(DOMAIN_PORT1,buf,4);
			break;
		case 2:
			flash_read(DOMAIN_PORT2,buf,4);
			break;
		case 3:
			flash_read(DOMAIN_PORT3,buf,4);
			break;
		case 4:
			flash_read(DOMAIN_PORT4,buf,4);
			break;
		case 5:
			flash_read(DOMAIN_PORT5,buf,4);
			break;
		case 6:
			flash_read(DOMAIN_PORT6,buf,4);
			break;
		default:
			break;
	}
	return len;
}

int inner_write_card_cnt(int which,char *buf,int len)
{
	flash_read(FLASH_ADDR_BASE,tmp_buf,MAX_LEN);
	switch(which)
	{
		case 1:
			sz_memset((char *)(tmp_buf+CARD_CNT_POS1),0,4);
			sz_memcpy((char *)(tmp_buf+CARD_CNT_POS1),buf,4);
			break;
		case 2:
			sz_memset((char *)(tmp_buf+CARD_CNT_POS2),0,4);
			sz_memcpy((char *)(tmp_buf+CARD_CNT_POS2),buf,4);
			break;
		case 3:
			sz_memset((char *)(tmp_buf+CARD_CNT_POS3),0,4);
			sz_memcpy((char *)(tmp_buf+CARD_CNT_POS3),buf,4);
			break;
		case 4:
			sz_memset((char *)(tmp_buf+CARD_CNT_POS4),0,4);
			sz_memcpy((char *)(tmp_buf+CARD_CNT_POS4),buf,4);
			break;
	    default:
			break;
	}
	write_data(FLASH_ADDR_BASE,tmp_buf,MAX_LEN);
	return 4;
}
int inner_read_card_cnt(int which,char *buf,int len)
{
	switch(which)
	{
		case 1:
			flash_read(CARD_CNT_POS1,buf,4);
			break;
		case 2:
			flash_read(CARD_CNT_POS2,buf,4);
			break;
		case 3:
			flash_read(CARD_CNT_POS3,buf,4);
			break;
		case 4:
			flash_read(CARD_CNT_POS4,buf,4);
			break;
		default:
			break;
	}
	return 4;
}

int inner_write_device_id(char *buf,int len)
{
	if(len >12)
		len = 12;
	flash_read(FLASH_ADDR_BASE,tmp_buf,MAX_LEN);
	sz_memset((char *)(tmp_buf+DEV_ID_BASE),0,12);
	sz_memcpy((char *)(tmp_buf+DEV_ID_BASE),buf,len);
	write_data(FLASH_ADDR_BASE,tmp_buf,MAX_LEN);
	return len;
}

int inner_read_device_id(char *buf,int len)
{
	flash_read(DEV_ID_BASE,buf,12);
	return 12;
}


void inner_printk_t()
{
	char buf[64];
	int i=1;
	int value;
	sz_printf("**************************************************************\r\n");
	sz_memset(buf,0,64);
	inner_read_dev_flg(buf,64);
	sz_printf("* system        :");
	value = sz_ctoi(buf,4);
	if(value==(~0U))
	{
	    sz_printf("(not init)\r\n",value);
	}
	else
	{

		sz_printf("%#x(init)\r\n",value);
	}
	
	//device addr.
	sz_memset(buf,0,64);
	inner_read_dev_addr(0,buf,64);
	value = sz_ctoi(buf,4);
	sz_printf("* master addr   :%#x\r\n",value);	

	sz_memset(buf,0,64);
	inner_read_dev_addr(1,buf,64);
	value = sz_ctoi(buf,4);
	sz_printf("* dong addr     :%#x\r\n",value);

	sz_memset(buf,0,64);
	inner_read_dev_addr(2,buf,64);
	value = sz_ctoi(buf,4);
	sz_printf("* xi addr       :%#x\r\n",value);

	sz_memset(buf,0,64);
	inner_read_dev_addr(3,buf,64);
	value = sz_ctoi(buf,4);
	sz_printf("* nan addr      :%#x\r\n",value);

	sz_memset(buf,0,64);
	inner_read_dev_addr(4,buf,64);
	value = sz_ctoi(buf,4);
	sz_printf("* bei addr      :%#x\r\n",value);

	//server domain
	i = 1;
	sz_memset(buf,0,64);
	innner_read_domain(i,buf,64);
	sz_printf("* domain%d       :%s\r\n",i++,buf);

	sz_memset(buf,0,64);
	innner_read_domain(i,buf,64);
	sz_printf("* domain%d       :%s\r\n",i++,buf);

	sz_memset(buf,0,64);
	innner_read_domain(i,buf,64);
	sz_printf("* domain%d       :%s\r\n",i++,buf);

    sz_memset(buf,0,64);
	innner_read_domain(i,buf,64);
	sz_printf("* domain%d       :%s\r\n",i++,buf);

	sz_memset(buf,0,64);
	innner_read_domain(i,buf,64);
	sz_printf("* domain%d       :%s\r\n",i++,buf);

	sz_memset(buf,0,64);
	innner_read_domain(i,buf,64);
	sz_printf("* domain%d       :%s\r\n",i++,buf);

	//server port
    i = 1;
	sz_memset(buf,0,64);
	innner_read_port(i,buf,4);
	value = sz_ctoi(buf,4);
	sz_printf("* port%d         :%d\r\n",i++,value);

	sz_memset(buf,0,64);
	innner_read_port(i,buf,4);
	value = sz_ctoi(buf,4);
	sz_printf("* port%d         :%d\r\n",i++,value);

	sz_memset(buf,0,64);
	innner_read_port(i,buf,4);
    value = sz_ctoi(buf,4);
	sz_printf("* port%d         :%d\r\n",i++,value);

	sz_memset(buf,0,64);
	innner_read_port(i,buf,4);
	value = sz_ctoi(buf,4);
	sz_printf("* port%d         :%d\r\n",i++,value);

	sz_memset(buf,0,64);
	innner_read_port(i,buf,4);
	value = sz_ctoi(buf,4);
	sz_printf("* port%d         :%d\r\n",i++,value);

	sz_memset(buf,0,64);
	innner_read_port(i,buf,4);
	value = sz_ctoi(buf,4);
	sz_printf("* port%d         :%d\r\n",i++,value);

	sz_memset(buf,0,64);
	inner_read_chanel(buf,4);
	value = sz_ctoi(buf,4);
	sz_printf("* channel       :%d\r\n",value);

	sz_memset(buf,0,64);
	inner_read_device_id(buf,12);
	sz_printf("* device_id     :%s\r\n",buf);
	
	//card count
//    i = 1;
//    sz_memset(buf,0,64);
//	inner_read_card_cnt(i,buf,4);
//	value = sz_ctoi(buf,4);
//	sz_printf("* pos%d cards    :%d\r\n",i++,value);
//	
//    sz_memset(buf,0,64);
//	inner_read_card_cnt(i,buf,4);
//	value = sz_ctoi(buf,4);
//	sz_printf("* pos%d cards    :%d\r\n",i++,value);
//
//	sz_memset(buf,0,64);
//	inner_read_card_cnt(i,buf,4);
//	value = sz_ctoi(buf,4);
//	sz_printf("* pos%d cards    :%d\r\n",i++,value);
//
//	sz_memset(buf,0,64);
//	inner_read_card_cnt(i,buf,4);
//	value = sz_ctoi(buf,4);
//	sz_printf("* pos%d cards    :%d\r\n",i++,value);
	
	sz_printf("**************************************************************\r\n");
}



void inner_format_flash()
{
	int len = ~0U;
	write_data(DEV_FLG_BASE,(char*)&len,4);
	
}

void inner_test_write()
{

	short len = 0x00;
	int i;
    char buf[64];
	write_data(DEV_FLG_BASE,(char*)&len,4);
	for(i = 0;i<5;i++)
	{
		len = i+0x95;
        inner_write_dev_addr(i,(char*)&len,4);
	}
	for(i = 1;i<7;i++)
	{
		sz_memset(buf,0,64);
		len = sz_snprintf(buf,64,"www.colorful%d.com",i);
        innner_write_domain(i,buf,64);
	}

	for(i = 1;i<7;i++)
	{
		sz_memset(buf,0,64);
		len = 8809+10*i;
        innner_write_port(i,(char*)&len,6);
	}
	
	len = 178;
	inner_write_chanel((char*)&len,4);
    sz_printk("all sizeof:%d\n",CARD_CNT_POS4+4);
    len = 44;
    for(i = 1;i<5;i++)
	{
        inner_write_card_cnt(i,(char*)&len,4);
	}
	inner_write_device_id("00000000001C",12);
}
