#ifndef __USART2_H_
#define __USART2_H_
extern void uart2_init(unsigned int bound);
extern int usart2_read(char *src,int data_len);
extern int usart2_write(char *src,int data_len);
#endif


