#include "reg52.h"
#include "main.h"
#include "uart.h"
#include "timer.h"
#include "rc522.h"
#include "ctrl.h"
#include "lcd12864.h"
#include "beep.h"
#include "key.h"  
#include "EEPROM.h"
 
void init_all(void)	//初始化
{
	EA = 0;	      //关总中断		 
	init_timer(); //定时器初始化
	init_uart();  //串口初始化
	init_rc522(); //RC522初始化	
	lcd_init();	  //LCD初始化   
	EEPROM_READ(0,Card_SN_BUF,4);//EEPROM初始化,取出记录的卡号  
	EEPROM_READ(7,KEY_BUF,8);//EEPROM初始化,取出记录的密码 
	
	send_bytes(Card_SN_BUF,4);
	EA = 1;		  //开总中断
}

void main(void)	  //主函数
{	
	INT8U key;

    Delay_ms(50); //让硬件稳定
	init_all();	   //执行初始化函数
	relay_OFF();   //关继电器
	LED_BLINK_1(); //led test
	beep1();	   //beep test 	
    display(0,0,0);	 //显示初始化

	while(1)
	{
	   key=key_scan(); //按键操作
	   if(key==12)  if(states>0) states--;
	                else states=0;	//上一功能
	   
	   if(key==13) if(++states>3) states=3;	//下一功能
	   ctrl_process(); //进入RC522操作
	}
}	  
