#ifndef _SZ_PAKAGE_H_
#define _SZ_PAKAGE_H_
typedef enum _ProtocolFlag
{
        None = 0,
        Hello = 1, //握手协议 v3
        ParameterSet = 2, //参数协议pc
        Command = 3, //控制协议pc
        Picture = 4, //图片传输协议 v3
        ServerACK = 5,//服务器应答 server
        HeartBeat = 6,//心跳
        ParameterGet = 7, //pc获取v3参数
        MotionData = 10,
        PC_REQEST_PIC = 11,//resend pic
        PC_SET_WIFI_PARA = 15,//PC set wifi param
        SERVER_SET_WIFI_PARA = 16,
        PC_CLEAR_PIC_CNT = 17,
	 	CheckLoginLanding = 23,        //检查登陆地址
	  	PLCDebugMessage = 25,  //网络Debug 信息
        SYSTEM_CMD = 31,
        ENABLE_MONTION = 32,
        DISABLE_MONTION = 33,
        GET_LED_STATE = 35,
        GET_LED_VOLTAGE = 36,
        SET_LED_VOLTAGE = 37,
        ROI_STATUS = 38,//自动定位状态
        PC_SET_PARAMETER = 39, //pc set param 
        REBOOT = 40,
        PC_SET_RIOMODE = 44,//debug
        JSON_PAKAGE = 105,
 }ProtocolFlag;


typedef struct protocol_version_t
{
	  unsigned char Product;//0x1=X1
	  unsigned char Year;
	  unsigned char Month;
	  unsigned char Date;
}Protocol_version;
#pragma pack(1)
typedef struct protecol_head_t
{
	unsigned char protocolFlag;//协议标志，cmd type
	unsigned char deviceFlag;//设备标志东西南北
	unsigned char mac[12];//物理地址
	Protocol_version protocolVersion;//协议版本
	unsigned char transmiteFlag;//传输方向
	int dataLength;//数据长度
}Protecol_head;

enum
{
	NO_HEAD	=-1,
	NO_TAIL = -2,
	BUF_NO_ENOUGH = -3,
};
typedef struct pakage_str_t
{
	char *pos_e;
	char *pos_s;
	char *pakage_e;
	char *pakage_s;
}Pakage_str;
extern int sz_pakage(char *buf,int buf_len,const char *comment,int commet_len,int flag);
extern int sz_pakage_pre(char *buf,int buf_len,Protecol_head **pp_pro);
extern int sz_pakage_aft(char *buf,int buf_len,char **pos_e);
extern int sz_un_pakage(char *buf,int len,struct protecol_head_t **protcol,struct pakage_str_t *p_pakage);
extern void sz_pakage_len(char *addr,int len);
#endif
