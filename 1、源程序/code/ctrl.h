
#ifndef  __ctrl_h__
#define  __ctrl_h__



#define ERROR_NONE			0x00	//正常
#define	ERROR_NOMONEY		0x01	//余额不足
#define	ERROR_NOCMD			0x02	//无效命令
#define	ERROR_CHECK			0x03	//校验错误
#define ERROR_KEYA_IC 		0x04	//密码错误
#define	ERROR_READ_IC  		0x05	//读卡失败
#define	ERROR_WRITE_IC  	0x06	//写卡失败
#define ERROR_WRITE_KEY_IC	0x07	//修改密码失败
#define ERROR_SET_CSTSN		0x08	//设置客户编码失败
#define ERROR_NOCARD		0x09	//IC卡不存在
#define	ERROR_ATCLL			0x0A	//防冲突失败
#define ERROR_SLCT			0x0B	//选卡失败
#define	ERROR_BLOCK_ADDR	0x0C	//块错误
#define	ERROR_OP_TYPE		0x0D	//操作方式错误	

extern unsigned char states,Card_SN_BUF[4],KEY_BUF[8];//卡号登记缓冲区	 	 
extern void uart_over( void );
extern void ctrl_process( void );

#endif