#ifndef _KEY_H_
#define _KEY_H_
enum
{
	SP = 0,
	LP =1,
	LLP=2,
	REVSER = 3,
};
extern void initKey();
extern int GetKey(int whichkey);
//extern void SetKeyFlag(int whichkey,int flag);
extern void key_clk();
extern void init_key_gpio();
extern int read_pin_config();
#endif