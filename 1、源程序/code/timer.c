#include "reg52.h"
#include "main.h"
#include "uart.h"
#include "ctrl.h"

#define	TIME10MS	4
#define	TIME80MS	6
#define	TIME200MS	20

INT16U UartWaitTime = 0, RequestCardTime = 0;	
INT8U TimerTh0, TimerTl0;

//TIMER1 initialize - prescale:8
// desired value: 20mSec
// actual value: 19.999mSec (0.0%)
void init_timer(void)  //20ms
{
	TMOD = 0x21;	 //定时器1自动重载  定时器0 普通16位计时

	TH0 = (65535-20000)/256;//0x69;
	TL0 = (65535-20000)%256; //0xFA;
	ET0 = 1;
	TR0 = 1;

	TH1 = 256 - FOSC / 12 / 32 / BAUD;
	TL1 = 256 - FOSC / 12 / 32 / BAUD;
	TR1 = 1;
}

void  time0_routine(void) interrupt 1
{
  //TIMER1 has overflowed
  //TMOD = 0x01;

	TH0 = (65535-20000)/256;//0x69;
	TL0 = (65535-20000)%256; //0xFA;

	RequestCardTime ++;

	if( ( ( UartWaitTime ++ ) >= TIME10MS ) && ( UartStart == TRUE ) )
	{
		uart_over();
	} 			   	
}

void Delay_ms(int ms)	 //延时xms
{
    unsigned int i,j;
	for(i=ms;i>0;i--)
	   for(j=220;j>0;j--);
}