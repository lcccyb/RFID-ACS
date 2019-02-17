
#ifndef  __ctrl_h__
#define  __ctrl_h__



#define ERROR_NONE			0x00	//����
#define	ERROR_NOMONEY		0x01	//����
#define	ERROR_NOCMD			0x02	//��Ч����
#define	ERROR_CHECK			0x03	//У�����
#define ERROR_KEYA_IC 		0x04	//�������
#define	ERROR_READ_IC  		0x05	//����ʧ��
#define	ERROR_WRITE_IC  	0x06	//д��ʧ��
#define ERROR_WRITE_KEY_IC	0x07	//�޸�����ʧ��
#define ERROR_SET_CSTSN		0x08	//���ÿͻ�����ʧ��
#define ERROR_NOCARD		0x09	//IC��������
#define	ERROR_ATCLL			0x0A	//����ͻʧ��
#define ERROR_SLCT			0x0B	//ѡ��ʧ��
#define	ERROR_BLOCK_ADDR	0x0C	//�����
#define	ERROR_OP_TYPE		0x0D	//������ʽ����	

extern unsigned char states,Card_SN_BUF[4],KEY_BUF[8];//���ŵǼǻ�����	 	 
extern void uart_over( void );
extern void ctrl_process( void );

#endif