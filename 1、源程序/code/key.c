#include "reg52.h"
#include "timer.h"

#define uchar unsigned char 
#define uint unsigned int

uchar num,temp;		

uchar key_scan(void) //键盘扫描
{
	num=16;//16表示没按键输入
	P1=0xfe; //扫描第一行
	temp=P1;
	temp&=0xf0;
	if(temp!=0xf0)	//检测有按键按下
	{
		Delay_ms(5);
		temp=P1;
		temp&=0xf0;
		if(temp!=0xf0)
		{
			temp=P1;
			switch(temp)	  //判断按下是哪一个按键
			{
				case 0xee:num=1;
					break;
				case 0xde:num=2;
					break;
				case 0xbe:num=3;
					break;
				case 0x7e:num=12;
					break;
			}
			while(temp!=0xf0)
			{
				temp=P1;
				temp&=0xf0;	
			}
		}
	}

	P1=0xfd;	//扫描第二行
	temp=P1;
	temp&=0xf0;
	if(temp!=0xf0)
	{
		Delay_ms(5);
		temp=P1;
		temp&=0xf0;
		if(temp!=0xf0)
		{
			temp=P1;
			switch(temp)
			{
				case 0xed:num=4;
					break;
				case 0xdd:num=5;
					break;
				case 0xbd:num=6;
					break;
				case 0x7d:num=13;
					break;
			}
		 	while(temp!=0xf0)
			{
				temp=P1;
				temp&=0xf0;	
			}	
		}
	}

	P1=0xfb;         //扫描第三行
	temp=P1;
	temp&=0xf0;
	if(temp!=0xf0)
	{
		Delay_ms(5);
		temp=P1;
		temp&=0xf0;
		if(temp!=0xf0)
		{
			temp=P1;
			switch(temp)
			{
				case 0xeb:num=7;
					break;
				case 0xdb:num=8;
					break;
				case 0xbb:num=9;
					break;	 
				case 0x7b:num=14;
					break;
			}
		 	while(temp!=0xf0)
			{
				temp=P1;
				temp&=0xf0;	
			}	
		}
	}

	P1=0xf7;	  //扫描第四行
	temp=P1;
	temp&=0xf0;
	if(temp!=0xf0)
	{
		Delay_ms(5);
		temp=P1;
		temp&=0xf0;
		if(temp!=0xf0)
		{
			temp=P1;
			switch(temp)
			{
				case 0xe7:num=0;
					break;
				case 0xd7:num=10;
					break;
				case 0xb7:num=11;
					break;
				case 0x77:num=15;
					break;
			}
		 	while(temp!=0xf0)
			{
				temp=P1;
				temp&=0xf0;	
			}	
		}
	}
	return num;	   //返回按键对应编号
}	  



