#ifndef _CC110X_ADAPTER_H_
#define _CC110X_ADAPTER_H_
#include "stm32f10x_lib.h"
extern void cc110x_init();
extern int  cc110x_read(int regaddr,char *src,int len);
extern int  cc110x_write(int regaddr,const char *src,int len);
extern int cc110x_read_t(int regaddr);
extern int cc110x_write_t(int regaddr,int value);
extern int cc110x_read_status(int regaddr);
extern int cc110x_write_strobe(int regaddr);
#define cc110x_read_GDO0 (GPIOA->IDR & (GPIO_Pin_3))
//#define cc110x_read_GDO0 (GPIOA->IDR & (GPIO_PIN_11))
extern int cc110x_write_sp_cmd(int cmd);
extern void cc110x_tx_enable();
extern void cc110x_tx_disable();
extern void cc110x_rx_enable();
extern void cc110x_rx_disable();
extern int cc110x_reset(int cmd);
extern void CC1101_irq_enable();
extern void CC1101_irq_disable();
#endif
