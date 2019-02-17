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
 
void init_all(void)	//��ʼ��
{
	EA = 0;	      //�����ж�		 
	init_timer(); //��ʱ����ʼ��
	init_uart();  //���ڳ�ʼ��
	init_rc522(); //RC522��ʼ��	
	lcd_init();	  //LCD��ʼ��   
	EEPROM_READ(0,Card_SN_BUF,4);//EEPROM��ʼ��,ȡ����¼�Ŀ���  
	EEPROM_READ(7,KEY_BUF,8);//EEPROM��ʼ��,ȡ����¼������ 
	
	send_bytes(Card_SN_BUF,4);
	EA = 1;		  //�����ж�
}

void main(void)	  //������
{	
	INT8U key;

    Delay_ms(50); //��Ӳ���ȶ�
	init_all();	   //ִ�г�ʼ������
	relay_OFF();   //�ؼ̵���
	LED_BLINK_1(); //led test
	beep1();	   //beep test 	
    display(0,0,0);	 //��ʾ��ʼ��

	while(1)
	{
	   key=key_scan(); //��������
	   if(key==12)  if(states>0) states--;
	                else states=0;	//��һ����
	   
	   if(key==13) if(++states>3) states=3;	//��һ����
	   ctrl_process(); //����RC522����
	}
}	  
