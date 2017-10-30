#include "sz_libc.h"


int Crc16Result(char *data, int length)
{
	int i;
	unsigned int reg_crc=0xFFFF;
	while(length--)
	{
		reg_crc ^= *data++;
		for(i=0; i<8; i++)
		{
			if(reg_crc & 0x01)
				reg_crc=(reg_crc>>1) ^ 0xA001;
			else
				reg_crc=reg_crc >>1;
		}
	}
	return reg_crc;
}
void printf_fifo(char *buf,int len)
{
	int cnt=0;
	int pri_len=0;
	int ch;
	char pri_buf[64] = {0};
	sz_memset(pri_buf,0,64);
	for(cnt=0;cnt<len;cnt++)
	{
		pri_len +=sz_snprintf(&pri_buf[pri_len],64 - pri_len,\
			"%02X",(char)buf[cnt]);
	}
	sz_printf("%s\r\n",pri_buf);
}