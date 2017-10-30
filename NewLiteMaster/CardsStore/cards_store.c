#include "cards_store.h"
#include "sz_libc.h"
#include "device_info.h"
#include "inner_flash.h"

//牌的偏移量
static const char card_name[45][6] = { { "错误" }, { "一萬" }, { "二萬" }, { "三萬" }, { "四萬" }, { "五萬" }, { "六萬" }, { "七萬" }, { "八萬" }, { "九萬" },
{ "一筒" }, { "二筒" }, { "三筒" }, { "四筒" }, { "五筒" }, { "六筒" }, { "七筒" }, { "八筒" }, { "九筒" },
{ "一条" }, { "二条" }, { "三条" }, { "四条" }, { "五条" }, { "六条" }, { "七条" }, { "八条" }, { "九条" },
{ "东风" }, { "南风" }, { "西风" }, { "北风" }, { "红中" }, { "發財" }, { "门板" }, { "春天" }, { "夏天" },
{ "秋天" }, { "冬风" }, { "梅花" }, { "兰花" }, { "菊花" }, { "竹子" }, { "百搭" }, { "白板" }
};

static struct card_info current_card[4];
static struct card_info blue_cards[4];
static int card_flg = 1;


struct card_info *get_test_cards()
{
	int i;
	int j;
    struct card_info *p_card = get_cards();
	
	for(i=0;i<4;i++)
	{
		sz_memset(p_card[i].card_list,0,48);
		sz_memset(p_card[i].position,0,8);
		for(j=0;j<34;j++)
			p_card[i].card_list[j]=(char)(j+1);
		//p_card[i].card_cnt = 20-i;		
	}
	sz_snprintf(p_card[0].position,8,"East");
    sz_snprintf(p_card[1].position,8,"South");
	sz_snprintf(p_card[2].position,8,"West");
	sz_snprintf(p_card[3].position,8,"North");
	return p_card;
}

struct card_info *get_cards()
{

    return current_card;
	
}



//static void push_cards_z(struct card_info *p_card,char card)
//{
//	if(p_card->card_cnt < p_card->rng.max_size)
//	{
//		p_card->card_list[p_card->card_cnt] = card;
//		p_card->card_cnt ++;
//	}
//}

int push_card(int pos,char *card_buf,int len)
{
	int i;
	char card_;
	char *card_buf_end = card_buf+len;
	struct card_info *p_card = get_cards();
	if( pos < SUB_DEVICE_NUM_1 || pos > SUB_DEVICE_NUM_4 || (int)card_buf[0]==0xFF)

	{
		return -1;	
	}
	pos = pos -1;
	
//	while(card_buf<card_buf_end)
//	{
//		card_ = *card_buf;
//		push_cards_z(&p_card[pos],card_);
//		if((card_ & 0x80)==0)
//		{
//			push_cards_z(&blue_cards[pos],card_);
//		}
//		else
//		{
//			push_cards_z(&current_card[pos],card_);
//		}
//		card_buf++;
//	}
    return push_buf(&p_card[pos].rng,card_buf,len);
}


static void show_card_more_info_t(int pos,struct card_info *p_card)
{
	int i;
	int col;
	char tmp[64];
	RING_BUF * ring_buf = &p_card[pos].rng;
		
	sz_printf("%s list: card num.:%d,max:%d\r\n",p_card[pos].position,get_buf_data_size(&p_card[pos].rng),p_card[pos].rng.max_size);
	sz_printf("NUM: ");
    for(i=1;i<=col;i++)
	{
		sz_printf("%-2dth  ",i);
	}
	sz_printf("\r\n");
	sz_printf("上 : ");

	col = pop_buf(ring_buf,tmp,64);
	for(i=1;i<col;i+=2)
	{
		sz_printf("%s  ",card_name[tmp[col]&0x7F]);
	}
    sz_printf("\r\n");
	sz_printf("下 : ");
	for(i=0;i<col;i+=2)
	{
		sz_printf("%s  ",card_name[tmp[col]&0x7F]);
	}
	sz_printf("\r\n");
	
}


static void print_spilt_line()
{
	sz_printf("*****************************************************************************");
	sz_printf("********************************************************************\r\n");
}
void show_card_all_info()
{
	int i;
	struct card_info *p_card;
	p_card = get_cards();
	print_spilt_line();
    for(i=0;i<3;i++)
    {
		systick_delay(100);
		show_card_more_info_t(i,p_card);
		sz_printf("\r\n\r\n");
    }
	systick_delay(100);
	show_card_more_info_t(i,p_card);
	print_spilt_line();
}

void show_card_list_info()
{
	int i;
	struct card_info *p_card = get_cards();
	sz_printf("********************************************************************\r\n");
	for(i=0;i<4;i++)
	{
		sz_printf("*%s:%d(%d)\r\n",p_card[i].position,get_buf_data_size(&p_card[i].rng),p_card[i].rng.max_size);
	}
	sz_printf("********************************************************************\r\n");
}

void show_card_more_info(int pos)
{
	print_spilt_line();
	show_card_more_info_t(pos,get_cards());
    print_spilt_line();
}


void clear_card()
{
	int j;
    struct card_info *p_card;
	p_card = get_cards();
	for(j=0;j<4;j++)
	{
		reset_rng_buf(&p_card[j].rng);
	}
}

void init_cards()
{
	struct card_info *p_card;
    int i;
	p_card = get_cards();
    for(i=0;i<4;i++)
    {
		
	}
	sz_snprintf(p_card[0].position,8,"East");
    sz_snprintf(p_card[1].position,8,"South");
	sz_snprintf(p_card[2].position,8,"West");
	sz_snprintf(p_card[3].position,8,"North");
	clear_card();
	//get_test_cards();//this is for test
}






