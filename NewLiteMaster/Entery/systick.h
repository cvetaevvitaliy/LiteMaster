#ifndef _SYSTICK_H_
#define _SYSTICK_H_
extern void systick_init(int ms);
extern unsigned int get_systick();
extern void systick_delay(unsigned int cnt);	
#endif