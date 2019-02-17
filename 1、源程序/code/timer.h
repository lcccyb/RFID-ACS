#ifndef  __timer_h__
#define  __timer_h__

extern unsigned int  UartWaitTime, RequestCardTime;	
extern void init_timer(void);  //20ms
extern void timer_fast( void );
extern void timer_slow( void );
extern void Delay_ms(int ms);

#endif