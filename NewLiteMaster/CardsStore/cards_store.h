#ifndef _CARDS_STERE_H_
#define _CARDS_STERE_H_
#include "ring_buffer.h"
#include ""

struct scard_t{
    char src[8];
	char pos;
    char cnt;
    char max;
};


struct card_info
{
	char position[8];
	char card_list[64];
	RING_BUF rng;
};

struct frame_cards_t
{
	int resever;
};
extern void init_cards();
extern struct card_info *get_test_cards();
extern struct card_info *get_cards();

extern int push_card(int pos,char *card_buf,int len);
extern void show_card_all_info();
extern void clear_card();
extern void show_card_more_info(int pos);
extern void show_card_list_info();
#endif
