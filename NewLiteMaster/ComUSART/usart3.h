#ifndef __USART3_H_
#define __USART3_H_
extern void uart3_init(unsigned int bound);
extern int usart3_read(char *src,int data_len);
extern int usart3_write(char *src,int data_len);

void uart2_test(void);
#endif


