#include "virtual_net.h"
#include "ring_buffer.h"
#include "sim.h"
#include "delay_work.h"
#include "sz_pakage.h"
#include "orient_pagkage.h"
#include "virtual_net_handle_data.h"

//static struct _RING_BUF virtual_net_rng;
static char rng_buf[4][1024];
struct virtual_net_que_t que[4];


struct virtual_net_t
{
	struct list_head m_list_head_free;
    struct list_head m_list_head_work;
	int virtual_net_send_status;
	int virtual_net_recv_status;
	int virtual_net_status;
//	const char *server_ip_port;
	struct delay_work vir_net_delay_work;
  int heart_beat_flg;
};

static struct virtual_net_t vir_sta;

enum
{
	VIR_NET_SEND_DUMMY,
	VIR_NET_SEND_HANG,	
	VIR_NET_SEND_PRE,
	VIR_NET_SEND_DATA,
};
enum
{
	VIR_NET_RECV_DUMMY,
	VIR_NET_RECV_HANG,
	VIR_NET_RECV_PRE,
};

void virual_net_delay_work(struct delay_work *p_work);
int virtual_net_init()
{
    //init_ring_buf(&virtual_net_rng,rng_buf,1024);
    int i;
	INIT_LIST_HEAD(&vir_sta.m_list_head_free);
	INIT_LIST_HEAD(&vir_sta.m_list_head_work);
	for(i=0;i<4;i++)
	{
	    que[i].prt= rng_buf[i];
		que[i].max_len = sizeof(rng_buf[i]);
		list_add(&que[i].m_list,&vir_sta.m_list_head_free);
	}
	vir_sta.virtual_net_send_status = VIR_NET_SEND_DUMMY;
	vir_sta.virtual_net_recv_status = VIR_NET_RECV_DUMMY;
	vir_sta.virtual_net_status      = VIR_NET_DUMM;
    shedule_init(&vir_sta.vir_net_delay_work);
	vir_sta.vir_net_delay_work.handle = virual_net_delay_work;
	vir_sta.vir_net_delay_work.pri_data = (void*)&vir_sta;
	vir_sta.heart_beat_flg = 0;
	return 0;
}

void virtual_net_thread_start(int flg)
{

	if(flg == 0)
	{
		struct virtual_net_t *p_vir = vir_sta.vir_net_delay_work.pri_data;
	    shedule_delay(&vir_sta.vir_net_delay_work,20*1000);
		virtual_net_connect();
		p_vir->virtual_net_status = VIR_NET_CONN_ING;
	}
	else
	{
		shedule_delay(&vir_sta.vir_net_delay_work,25000);
	}
}


void virtual_net_heart_beat_respond()
{
	vir_sta.heart_beat_flg = 0;
}

static void virtual_net_heart_beat_reset()
{

	vir_sta.heart_beat_flg = -1;
}

static int virtual_net_get_heart_beat_flg()
{
	return vir_sta.heart_beat_flg;
}

void virtual_net_stop()
{
	cancel_delay_work(&vir_sta.vir_net_delay_work);
	virtual_net_set_net_status(VIR_NET_DUMM);
}
void virual_net_delay_work(struct delay_work *p_work)
{
	struct virtual_net_t *p_vir = (struct virtual_net_t *)p_work->pri_data;
	static int flg = 0;
	static int heartbeat_num = 0;
	switch(p_vir->virtual_net_status)
	{
		case VIR_NET_DIS_CONN:
			p_vir->virtual_net_status = VIR_NET_CONN_ING;
			virtual_net_connect();
			shedule_delay(p_work,20*1000);
			break;
		case VIR_NET_CONN_ING:
			virtual_net_close();
			shedule_delay(p_work,5*1000);
			p_vir->virtual_net_status = VIR_NET_DIS_CONN;
			break;
		case VIR_NET_CONN_OK:
			{
                if(flg==0)
                {
					sz_printk("send heartbeat num:%d\n",heartbeat_num++);
					send_heart_beat();
					virtual_net_heart_beat_reset();
					shedule_delay(p_work,20*1000);
					flg = 1;
                }
                else
                {
					flg = 0;
					if(virtual_net_get_heart_beat_flg()==0)
					{
						shedule_delay(p_work,5000);
					}
					else
    				{
						sz_printk("heartbeat num(%d) no respond\n",heartbeat_num);
    					p_vir->virtual_net_status = VIR_NET_CONN_ING;
    					shedule_delay(p_work,2000);
    				}
                }				
		    }
			break;
		default:
			shedule_delay(p_work,5000);
			break;
	}
	//sz_printk("net status:%d\n",p_vir->virtual_net_status);
}
int virtul_net_entery()
{
//	switch(step)
//	{
//		case VIR_NET_SEND_DUMMY:
//			break;
//		default:
//			break;
//	}
	return 0;
}

int virtual_net_socket()
{
	return 0;
}

int virtual_net_send_t(struct virtual_net_que_t *p_vir_que)
{
	if(vir_sta.virtual_net_status !=VIR_NET_CONN_OK)
	{
		list_add_tail(&p_vir_que->m_list,&vir_sta.m_list_head_free);
		return -1;
	}
	list_add_tail(&p_vir_que->m_list,&vir_sta.m_list_head_work);
	if(vir_sta.virtual_net_recv_status!=VIR_NET_RECV_DUMMY)
	{
		vir_sta.virtual_net_send_status = VIR_NET_SEND_HANG;
	}
	else if(vir_sta.virtual_net_send_status == VIR_NET_SEND_DUMMY)
	{
		vir_sta.virtual_net_send_status = VIR_NET_SEND_PRE;
		sim_send_len(p_vir_que->len);
	}
	return 0;
}

int virtual_net_send()
{
	struct virtual_net_que_t *p_vir_que;
	if(list_empty(&vir_sta.m_list_head_work))
	{
		return -1;
	}	
	if(vir_sta.virtual_net_send_status != VIR_NET_SEND_PRE)
		return -1;
	vir_sta.virtual_net_send_status = VIR_NET_SEND_DATA;
	p_vir_que = ContainOf(vir_sta.m_list_head_work.next, struct virtual_net_que_t, m_list);
	sim_send(p_vir_que->prt,p_vir_que->len);
    return p_vir_que->len;
}

int virtual_net_send_ok()
{


    struct virtual_net_que_t *p_vir_que;
	struct list_head *p_list;
	p_list=vir_sta.m_list_head_work.next;
	list_del_init(p_list);
	list_add(p_list,&vir_sta.m_list_head_free);
	if(list_empty(&vir_sta.m_list_head_work)==1)
	{
		vir_sta.virtual_net_send_status = VIR_NET_SEND_DUMMY;
		if(vir_sta.virtual_net_recv_status == VIR_NET_RECV_HANG)
		{
			vir_sta.virtual_net_recv_status = VIR_NET_RECV_PRE;
			sim_recv();
		}
		return -1;
	}	
	p_vir_que = ContainOf(vir_sta.m_list_head_work.next, struct virtual_net_que_t, m_list);
	vir_sta.virtual_net_send_status = VIR_NET_SEND_PRE;
	sim_send_len(p_vir_que->len);
	return 0;
}

static int virtual_net_send_commit()
{
	struct virtual_net_que_t *p_vir_que;
	if(list_empty(&vir_sta.m_list_head_work))
	{
		return -1;
	}	
	
	p_vir_que = ContainOf(vir_sta.m_list_head_work.next, struct virtual_net_que_t, m_list);
	sim_send_len(p_vir_que->len);
	vir_sta.virtual_net_send_status = VIR_NET_SEND_PRE;
    return p_vir_que->len;
}

int virtual_net_recv_commit()
{
	if(vir_sta.virtual_net_send_status != VIR_NET_SEND_DUMMY)
	{
		vir_sta.virtual_net_recv_status = VIR_NET_RECV_HANG;
		return 0;
	}
	if(vir_sta.virtual_net_recv_status == VIR_NET_RECV_DUMMY)
	{
        vir_sta.virtual_net_recv_status = VIR_NET_RECV_PRE;
		sim_recv();
        return 0;
	}
}
int virtual_net_recv_end()
{
    vir_sta.virtual_net_recv_status = VIR_NET_RECV_DUMMY;	
}
int virtual_net_recv(int socket,char *src,int len,int flg)
{
	if(vir_sta.virtual_net_recv_status == VIR_NET_RECV_DUMMY)
	{
		
vir_sta.virtual_net_recv_status = VIR_NET_RECV_PRE;
	}
	return 0;
}

int virtual_net_connect()
{
	char tmp_buf[64]={0};
	int port;
	char buf[96]={0};
	int server_cnt = 1;	
	if(server_cnt>6)
	{
		return -1;
	}
	innner_read_domain(server_cnt,tmp_buf,64);
	innner_read_port(server_cnt,(char*)&port,4);
	sz_snprintf(buf,96,"AT+CIPSTART=\"TCP\",\"%s\",%d",tmp_buf,port);
	//sz_printk(buf);
	sim_connect_server((const char *)buf);
	return 0;
}

int virtual_net_close()
{
	sim_close_server();
}
int virtual_net_get_send_buf(struct virtual_net_que_t **p2_vir_que)
{
	struct virtual_net_que_t *p_vir_que;
	if(vir_sta.virtual_net_status !=VIR_NET_CONN_OK)
	{
		list_add_tail(&p_vir_que->m_list,&vir_sta.m_list_head_free);
		return -1;
	}
	if(list_empty(&vir_sta.m_list_head_free))
	{
		sz_printk("alloc send buffer fail\n");
		return -1;
	}	
	p_vir_que = ContainOf(vir_sta.m_list_head_free.next, struct virtual_net_que_t, m_list);
	list_del_init(&p_vir_que->m_list);
	*p2_vir_que = p_vir_que;
	sz_memset(p_vir_que->prt,0,p_vir_que->max_len);
	return 0;
}

void virtual_net_release_send_buf(struct virtual_net_que_t *p_vir_que)
{
	list_add(&p_vir_que->m_list,&vir_sta.m_list_head_free);
}

void virtual_net_reinit_send_buf()
{
	struct virtual_net_que_t *p_vir_que;
	struct list_head *pos,*n,*head,*free_head;
	head = &vir_sta.m_list_head_work;
	vir_sta.virtual_net_send_status = VIR_NET_SEND_DUMMY;
	vir_sta.virtual_net_recv_status = VIR_NET_RECV_DUMMY;
	vir_sta.virtual_net_status      = VIR_NET_DIS_CONN;
	
	if(list_empty(head))
	{
		return;
	}
	free_head = &vir_sta.m_list_head_free;
	list_for_each_safe(pos, n, head)
	{
		p_vir_que = ContainOf(pos, struct virtual_net_que_t, m_list);
		list_del_init(&p_vir_que->m_list);
		list_add(pos,free_head);
	}
	
}

void  virtual_net_set_net_status(int flg)
{
	virtual_net_reinit_send_buf();
	vir_sta.virtual_net_status = flg;
	sz_printk("set new virtual status:%d\n",flg);
	if(flg == VIR_NET_DIS_CONN)
	{
		rest_recv_buf_str();
		virtual_net_set_online(-1);
		virtual_net_set_handshake(-1);
	}
	else if(flg == VIR_NET_CONN_OK)
	{
		//send headshake 
		send_handshake();
		virtual_net_inc_conn();
		virtual_net_set_online(0);
	    virtual_net_heart_beat_respond();
		cancel_delay_work(&vir_sta.vir_net_delay_work);
		shedule_delay(&vir_sta.vir_net_delay_work,5*1000);
		//virtual_net_send_commit();
	}
}

int virtual_net_get_net_status()
{
	return vir_sta.virtual_net_status;
}

int virtual_net_is_work()
{
	if(vir_sta.virtual_net_status !=VIR_NET_CONN_OK)
		return -1;
	else if(vir_sta.virtual_net_recv_status == VIR_NET_RECV_DUMMY && vir_sta.virtual_net_send_status == VIR_NET_SEND_DUMMY)
	{
		return 0;
	}
	return 0;
}