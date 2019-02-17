#ifndef  __uart_h__
#define  __uart_h__

#define	UART_MAX_LEN	50

extern unsigned char data UartBuf[UART_MAX_LEN];

extern unsigned char UartCount, UartLen, UartStart, UartComp;

extern void init_uart(void);
extern void send_byte(unsigned char  SendData);
extern void send_bytes( unsigned char *Buf, unsigned char Len );

#endif
