/**
***************************************************************************
* @file: 
* @author: alw  
* @date:  5,26,2017
* @brief:  
* @attention:
**************************************************************************
**/
#include "stm32f10x_lib.h"
#include "sim.h"
#include "delay_work.h"
#include "sz_libc.h"
#include "systick.h"
#include "virtual_net.h"

void asy_sim();

//todo::ddddd
enum SIM_STATE
{
	OPEN_SIM_PRE,
	OPEN_SIM_AFR,
	CHECK_STA,
	SET_MAX_VOLUME,
	CHECK_SIM_IS_READY,
    OPENED,
	CLOSED,
	CLOSE_PRE,
	CLOSE_AFT,
	UNKNOW,
};
enum SIM_POWER
{
	SET_NR,
	SET_PWRKEY
};
struct str_sim_state
{
	int sim_state;
	int call_status;
	int sms_status;
	int sig_cnt_time;
};
struct str_sim_state sim_status;
static struct delay_work sim_delay_work;
static struct delay_work sim_send_msg_delay_work;
static int(*sim_write_cb)(char *src,int len);
static int(*sim_read_cb)(char *src,int len);

int check_sim_state()
{
	int ret;
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	ret = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST,ENABLE);
	return ret;
}

static const char end_s[]= "\r\n";

static int sim_write(char *src,int len)
{
	if(sim_write_cb)
	    return sim_write_cb(src,len);
	else
		return -1;
}

void set_sim_write_cb(int(*cb_)(char *src,int len))
{
	sim_write_cb = cb_;
}

static int sim_read(char *src,int len)
{
	if(sim_read_cb)
	    return sim_read_cb(src,len);
	else
		return -1;
}

void set_sim_read_cb(int(*cb_)(char *src,int len))
{
	sim_read_cb = cb_;
}

/*******************************************************************************
*@ Description    :  
*@ Input          :
*@ Output         :
*@ Return         :
*******************************************************************************/

int sim_send_cmd(const char *src)
{
	int len;
	len	= sz_strlen((char*)src);
	if(len <=0)
		return len;
	sim_write((char*)src,len);
	sim_write((char*)end_s,2);
	return len;
}


static void sim_state_handle(struct delay_work *pwork)
{
	struct str_sim_state *priv_data = (struct str_sim_state *)pwork->pri_data;
	if(priv_data==0)
		return;
 	switch(priv_data->sim_state)
	{
		case OPEN_SIM_PRE:
			GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_RESET);//reset
			shedule_delay(&sim_delay_work,1500);
			priv_data->sim_state = OPEN_SIM_AFR;
			break;
		case OPEN_SIM_AFR:
			GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_SET);//set
			shedule_delay(&sim_delay_work,500);
			priv_data->sim_state = CHECK_STA;
			break;
		case CHECK_STA:
			shedule_delay(&sim_delay_work,5*1000);
			#if 0
			if(check_sim_state()==1)//open
			{
				asy_sim();//
				priv_data->sim_state = SET_MAX_VOLUME;
				sz_printk("send asy to sim\m");
			}
			else
			{

				sz_printk("sim open fail\n");
				priv_data->sim_state = CLOSED;
			}
			#else
			    asy_sim();//
				priv_data->sim_state = SET_MAX_VOLUME;
				sz_printk("send asy to sim\n");
			#endif
			break;
		case SET_MAX_VOLUME:
			shedule_delay(&sim_delay_work,500);
			//set_sim_volum_sim(100);
			//get_cmic_statue();
			sim_get_gloable_object_id();
			priv_data->sim_state = CHECK_SIM_IS_READY;
			sz_printk("get the IMEI\n");
			break;
	    case CHECK_SIM_IS_READY:
			check_sim_card_sim();
			shedule_delay(&sim_delay_work,500);
			priv_data->sim_state = OPENED;
			sz_printk("check sim is ready or not now\n");
			break;
		case OPENED:
			if(virtual_net_is_work()==0)
			{
				if(priv_data->sig_cnt_time <15)
				{
				    priv_data->sig_cnt_time +=1;
				}
		        get_singal_QA();
			}
			shedule_delay(&sim_delay_work,priv_data->sig_cnt_time);
			break;
		case CLOSED:
			break;
		case CLOSE_PRE:
			priv_data->sim_state = CLOSE_AFT;
			shedule_delay(&sim_delay_work,2000);
			GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_RESET);//reset
			break;
	    case CLOSE_AFT:
			shedule_delay(&sim_delay_work,1000);
			priv_data->sim_state = OPEN_SIM_PRE;
			GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_SET);//reset
			break;
		default:
//			shedule_delay(&sim_delay_work,10);
//			if(check_sim_state()==1)
//			{
//				priv_data->sim_state = OPENED;
//			}
//			else
//			{
//				priv_data->sim_state = CLOSED;
//			}
			break;
	}
}

enum
{
	IDLE,
	SET_EDIT_MODE,
	SEND_PHONE_NUM,
	SEND_MSG,
	
};

struct snd_statue
{
	int snd_state;
	char *src;
	int len;
};
struct snd_statue snd_st;

static const char send_msg[]="AT+CMGF=1";
static const char send_mode[] = "AT+CSCS=\"GSM\"";
static const char serverNum[] = "AT+CMGS=\"+8613557037162\"";


static void snd_msg_pro(struct delay_work *pwork)
{
	char end_ch = 0x1A;
	struct snd_statue *pStatue = (struct snd_statue *)pwork->pri_data;
	switch(pStatue->snd_state)
	{
		case SET_EDIT_MODE:
			snd_st.snd_state = SEND_PHONE_NUM;
			sim_send_cmd(send_msg);
			shedule_delay(pwork,10);//10ms
			break;
		case SEND_PHONE_NUM:
			snd_st.snd_state = SEND_MSG;
			sim_send_cmd(serverNum);
			shedule_delay(pwork,10);//10ms
			break;
		case SEND_MSG:
			sim_write((char*)pStatue->src,pStatue->len);
			sim_write(&end_ch,1);
			pStatue->snd_state = IDLE;
			break;
		default:
			break;
	}
}
void send_one_msg(const char *msg,int len)
{
	if(snd_st.snd_state !=IDLE)
		return ;
	sim_send_cmd(send_msg);
	snd_st.snd_state = SET_EDIT_MODE;
	snd_st.src =(char*)msg;
	snd_st.len = len;
	shedule_delay(&sim_send_msg_delay_work,100);//100ms
}

static char read_msg_ch[16] = "AT+CMGR=1";
static const char test[]=" AT+CPMS=?";
static const char set_new_msg_mde[]="AT+CNMI=2,1";
static const char read_all_msg[]="AT+CMGL=\"ALL\"";
void read_msg(char *mode,char *src)
{
	sim_send_cmd(send_msg);
	//HALDelay10ms(1);
	if(sz_strcmp("-a",mode)==0 ||sz_strcmp("-A",mode)==0)
	{
		sim_send_cmd(read_all_msg);
	}		
	else if(sz_strcmp("-i",mode)==0 ||sz_strcmp("-I",mode)==0)
	{
		 sz_memset(&read_msg_ch[8],0,8);
		 sz_memcpy(&read_msg_ch[8],src,sz_strlen(src));
		 sim_send_cmd((const char*)read_msg_ch);
	}

}


void delete_msg(char *mode,char *src)
{
	static const char sim_delete_all_msg[]="AT+CMGD=\"ALL\"";
	static char delete_msg_ch[16] = "AT+CMGD=1";
	if(sz_strcmp("-a",mode)==0 ||sz_strcmp("-A",mode)==0)
	{
		sim_send_cmd(sim_delete_all_msg);
	}		
	if(sz_strcmp("-i",mode)==0 ||sz_strcmp("-I",mode)==0)
	{
		 sz_memset(&delete_msg_ch[8],0,8);
		 sz_memcpy(&delete_msg_ch[8],src,sz_strlen(src));
		 sim_send_cmd((const char*)delete_msg_ch);
	}
	
}

void sim_check_creg_net()
{
	static const char *ptr = "AT+CREG?";
	sim_send_cmd(ptr);
}

void sim_check_cgreg_net()
{
	static const char *ptr = "AT+CGREG?";
	sim_send_cmd(ptr);
}


void sim_reg_creg_net()
{
	static const char *prt="AT+CREG=1";
	sim_send_cmd(prt);
}
void sim_reg_cgreg_net()
{
	static const char *ptr = "AT+CGREG=1";
	sim_send_cmd(ptr);
}

void sim_set_cipmux()
{
	sim_send_cmd("AT+CIPMUX=0");
	systick_delay(1);
	sim_send_cmd("AT+CIPRXGET=1");
}
void sim_connect_server(const char *conn_cmd)
{
	sim_send_cmd(conn_cmd); 
}

void sim_close_server()
{
	sim_send_cmd("AT+CIPCLOSE");
}
void sim_send_len(int len)
{
	static char tmp_buf[16]={0};
	sz_memset(tmp_buf,0,16);
	sz_snprintf(tmp_buf,16,"AT+CIPSEND=%d",len);
	sim_send_cmd((const char*)tmp_buf);
}
void sim_send(const char *src,int len)
{
//	sim_send_len(len);
//	HAL_Delay(10);
	sim_write((char*)src,len);
	sim_write((char*)end_s,2);
}
void sim_recv()
{
	sim_send_cmd("AT+CIPRXGET=2,64");	
}

void sim_get_gloable_object_id()
{
	static const char *goi = "AT+GSN";
	sim_send_cmd(goi);
}
static void sim_init_pin()
{
	//sim status
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST,ENABLE);
		
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz  ;
    GPIO_Init(GPIOB, &GPIO_InitStruct); 

	//sim pwd key
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct); 
	GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_SET);//set
}
void init_sim()
{
	sim_init_pin();
	shedule_init(&sim_delay_work);
	shedule_init(&sim_send_msg_delay_work);
	sim_status.sim_state = CLOSED;
	sim_delay_work.pri_data = (void*)&sim_status;
	sim_delay_work.handle = sim_state_handle;
	sim_send_msg_delay_work.pri_data = (void*)&snd_st;
	sim_send_msg_delay_work.handle = snd_msg_pro;
	sim_status.sig_cnt_time = 3;
}

void sim_timer_run()
{
	sim_status.sim_state = CHECK_STA;
	shedule_delay(&sim_delay_work,1000);//1000ms
}

void sim_timer_stop()
{
	cancel_delay_work(&sim_delay_work);
}

void open_sim()
{
	if(sim_status.sim_state == CLOSED)
	{
		sim_timer_stop();
		//GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_SET);//reset
		//HALDelay10ms(1500);
		//GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_SET);//set
		//sim_timer_run();//1500ms
		sim_status.sim_state = OPEN_SIM_PRE;
		cancel_delay_work(&sim_delay_work);
		shedule_delay(&sim_delay_work,50);
	}	
}


void close_sim()
{
	static char power_off[]="AT+CPOWD=0\r\n";
//	if(check_sim_state()==1)
//	{
		cancel_delay_work(&sim_delay_work);
		sim_status.sim_state = CLOSED;
		virtual_net_stop();
		#if 1
		sim_write(power_off,sz_strlen(power_off));
		#else
		GPIOB->BRR = GPIO_PIN_4 ;
		HALDelay10ms(120);
		GPIOB->BSRR = GPIO_PIN_4 ;
		HALDelay10ms(20);//200ms
		#endif
//	}
}

void sim_reopen()
{
	struct str_sim_state *priv_data = (struct str_sim_state *)sim_delay_work.pri_data;	
#if 1
	priv_data->sim_state = CLOSE_PRE;
    virtual_net_stop();
#else
    close_sim();
	priv_data->sim_state = OPEN_SIM_PRE;	
#endif
    cancel_delay_work(&sim_delay_work);
	shedule_delay(&sim_delay_work,1000);
}

void asy_sim() //__attribult__()
{
	static const char SIM_A[] = "A";
    static const char SIM_AT[] = "AT";
	sim_send_cmd(SIM_A);
	sim_send_cmd(SIM_AT);
}


void call_test_sim()
{
    //static const char SIM_CALL[] = "ATD13760378012;";
    //static const char SIM_CALL[] = "ATD13509953122;";
    //static const char SIM_CALL[] = "ATD13113638046;";
    static const char SIM_CALL[] = "ATD13557037162;";
	sim_send_cmd(SIM_CALL);
}


void cancle_sim()
{
	static const char SIM_ATH[]= "ATH";
	sim_send_cmd(SIM_ATH);
}


void respond_sim()
{
	static const char  SIM_ATA[]= "ATA";
	sim_send_cmd(SIM_ATA);
}


void check_sim_card_sim()
{
	static const char SIM_CHECK[] = "AT+CPIN?";
	sim_send_cmd(SIM_CHECK);
}


void check_sim_volum_sim()
{
	static const char  GET_SIM_VOLUM[] = "AT+CLVL?";
	sim_send_cmd(GET_SIM_VOLUM);
}


int set_sim_volum_sim(int volum)
{
	int len;
	static char SET_SIM_VOLUM[16];
	sz_memset(SET_SIM_VOLUM,0,16);
	len = sz_snprintf(SET_SIM_VOLUM,16,"AT+CLVL=%d",volum);
	if(len<0)
		return len;
	sim_send_cmd((const char*)SET_SIM_VOLUM);
	return 0;
}


void	get_my_phone_num()
{
	static const char cmd_get_my_phone_num[] = "AT+CNUM";
	sim_send_cmd(cmd_get_my_phone_num);
}


static const char	test_audio_2[]="AT+CLDTMF=100,\"1\"";//\r\n
struct delay_work tst_dk;
static int init_flg = 0;
void tst_handle(struct delay_work *timer)
{
	shedule_delay(timer,1000);
	sim_send_cmd(test_audio_2);
}
void test_audio()
{
	if(init_flg==0)
	{
		sim_send_cmd(test_audio_2);
		init_flg = 1;
		tst_dk.handle = tst_handle;
		shedule_delay(&tst_dk,1000);
	}
	else if(init_flg == 2)
	{
		sim_send_cmd(test_audio_2);
		init_flg = 1;
		shedule_delay(&tst_dk,1000);
	}
}


void get_mute_statue()
{
	static const char check_mute[] = "AT+CMUT?";
	sim_send_cmd(check_mute);
}


void get_cmic_statue()
{
	static const char check_cmic[] = "AT+CMIC=?";
	sim_send_cmd(check_cmic);
}

void stop_test_audio()
{
	if(init_flg==1)
	{
		cancel_delay_work(&tst_dk);
		init_flg = 2;
		sz_printk("stop\n",6);
	}
}


void get_singal_QA()
{
	static const char str_get_singal_QA[]="AT+CSQ";
	sim_send_cmd(str_get_singal_QA);
}


/*********************************************************************************************/

static	char *buf_end;
static	char *pos;
static  char sim_buffer[256]={0};
static int hand_status=0;

static void (*recall_handl)(int device,char *buf,int len);
void set_sim_recall_fun(void (*func)(int device,char *buf,int len))
{
	if(func!=0)
		recall_handl = func;
	
	buf_end = sim_buffer + sizeof(sim_buffer);
	pos = sim_buffer;
	sz_memset((char*)pos,0,sizeof(sim_buffer));
	hand_status = 0;
}

static void handle_data(char *buf,int len)
{

	if(recall_handl)
		recall_handl(0,buf,len);
}

enum
{
	SIM_RX_HAND_REST = 0,
	SIM_RX_HAND_FOUND_A,
	SIM_RX_HAND_FOUND_D,
};

static void handle_data_sim(char hand_ch)
{
    switch(hand_status)
    {
		case 1:
			if(hand_ch=='\r')
			    hand_status = 2;
			break;
	    case 2:
			if(hand_ch=='\n')
			{
                pos = sim_buffer;
                sz_memset((char*)pos,0,sizeof(sim_buffer));
                hand_status = 0;
			}
			else
			{
				hand_status = 1;
			}
			break;
		default:
    	{
            if(hand_ch!='\n')
            {
    			if(pos<buf_end)
    			{
    				*pos = hand_ch;
    				++pos;
    				return;
    			}
    			else
    			{
					if(hand_ch=='\r')
						hand_status = 2;
					else
						hand_status = 1;
    			}
            }
    		if(pos>sim_buffer)
    		{
        		if(*(pos-1)=='\r')//find a \r\n
        		{
					--pos;
					*(pos)= 0x00;
        		    handle_data(sim_buffer,pos-sim_buffer);
					pos = sim_buffer;
                    sz_memset((char*)pos,0,sizeof(sim_buffer));
                    hand_status = 0;
        		}

        		else//is a sigal \n
        		{
                    if(pos<buf_end)
        			{
        				*pos = hand_ch;
        				++pos;
        				return;
        			}
        			else
        			{
    					if(hand_ch=='\r')
    						hand_status = 2;
    					else
    						hand_status = 1;
        			}
        		}
    		}
    		else
    		{
				*pos = hand_ch;
				++pos;
				return;
    		}
    	}
		break;
    }
        
}
/*********************************************************************************************/

static char read_uart2_buf[12]={0};
void sim_entry()
{
	int len;
	int i;
	sz_memset(read_uart2_buf,0,12);
	len = sim_read(read_uart2_buf,8);
	for(i=0;i<len;i++)
	{
		handle_data_sim(read_uart2_buf[i]);
	}
}
