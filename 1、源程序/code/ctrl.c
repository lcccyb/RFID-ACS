#include "reg52.h"
#include "string.h"
#include "main.h"
#include "uart.h"  
#include "rc522.h"
#include "ctrl.h"
#include "beep.h"
#include "lcd12864.h"
#include "timer.h"
#include "EEPROM.h"
#include "key.h"

INT8U states=0;//状态机

INT8U Card_type[2],	//卡片类型
      Card_SN[4]={0},//IC卡号
	  Card_SN_BUF[4]={0xff,0xff,0xff,0xff},//卡号登记缓冲区	   
	  KEY_BUF[8]={'0','0','0','0','0','0','0','0'};

INT8U bPass = 0; bWarn = 0;	   

void uart_over( void )	//串口数据还原
{
	UartCount = 0;
	UartStart = FALSE;
	UartComp = FALSE;
}

void pass( void )
{
   beep1();
   Delay_ms(2000);//让卡片离开
   LED_BLINK_1();
}

void warn(void)
{
  LED_BLINK_2();
}


INT8U IC_READ( void )
{
    INT8U ID_ASC[8],i;
	if( PcdRequest( PICC_REQIDL, Card_type ) != MI_OK )//寻天线区内未进入休眠状态的卡，返回卡片类型 2字节	
	{
		if( PcdRequest( PICC_REQIDL, Card_type ) != MI_OK )//寻天线区内未进入休眠状态的卡，返回卡片类型 2字节	
		{
		   
			return FALSE;
		}	
	}
	
	if( PcdAnticoll( Card_SN ) != MI_OK ) //防冲撞，返回卡的序列号 4字节 
	{

		bWarn = 1;
		return FALSE;	
	}

	bPass = 1;
	send_bytes(Card_type,2);
	send_bytes(Card_SN,4);
	
	for(i=0;i<4;i++) //卡ID号转化成ASCII
	{
	  if(Card_SN[i]/16>9)	ID_ASC[i*2]=Card_SN[i]/16+'7';
	    else ID_ASC[i*2]=Card_SN[i]/16+'0';

      if(Card_SN[i]%16>9)	ID_ASC[i*2+1]=Card_SN[i]%16+'7';
	    else ID_ASC[i*2+1]=Card_SN[i]%16+'0';
	}
	display2(3,0,"ID: ",4); //显示卡号
	display2(3,2,ID_ASC,8);

	return TRUE;
}


void ctrl_process( void )
{		  
   INT8U i,key_count,key_value=16,table[8]="--------",statesbuf;	
	
	if(states!=statesbuf)  //状态改变，清屏
	{
	  	display_clear_line(2);
	    display_clear_line(3);
		statesbuf=states;
	}
    switch(states)
	{
       case 0:	          //IC卡读卡输入
		  	  display(1,0,1); 
		      if(IC_READ()) 
			  {
			   
                if ((Card_SN[0]==Card_SN_BUF[0])
			      &&(Card_SN[1]==Card_SN_BUF[1])
				  &&(Card_SN[2]==Card_SN_BUF[2])
				  &&(Card_SN[3]==Card_SN_BUF[3]))
				  {
					relay_ON();//灯开关
					display(2,0,5);
				
			      }
				  else 	 display(2,0,6);
				    relay_OFF();
			   }
			
			  break;

        case 1:		
		       display(1,0,2);    //密码输入
			   display2(3,0,table,8);
			   key_count=0;
               while(1)
			   {  
			      key_value=key_scan();	  
				  if(key_value==12) 
				   { 
				     states--;
	                 return;
					}

				  if(key_value==13)
				  {
				   states++;
				   return;
				  }				 
				  if(key_value>=0 && key_value <=9)//有按键输入
				  {		  				    				    
					table[key_count++]=key_value+'0';
					display2(3,0,table,8);			
				  }
				  
				  if(key_value==11)//退格
				  {								    
					table[--key_count]='-';
					display2(3,0,table,8); 				
				  }

				  if(key_count==8)
				  {
				     if( table[0]==KEY_BUF[0] &&
					     table[1]==KEY_BUF[1] &&
					 	 table[2]==KEY_BUF[2] &&
						 table[3]==KEY_BUF[3] &&
						 table[4]==KEY_BUF[4] &&
						 table[5]==KEY_BUF[5] &&
						 table[6]==KEY_BUF[6] &&
						 table[7]==KEY_BUF[7] )	  //密吗正确 
				
				     { 
					   bPass=1;
					   relay_ON();//灯开关
                       display(2,0,5);
					   relay_OFF();
					   break;				  
					
					 }
					  else 	 //密码错误
					  {	
					    relay_OFF();
						beep1()	 ;
						bWarn=1;
					    display(2,0,6);
						 break;
					 }	  				
				  }

			   }
		       break ;
		  
		 case 2:		
		       display(1,0,3);      //IC卡登记
	           if(IC_READ())  
			   {
					 
			    for(i=0;i<4;i++)
			    Card_SN_BUF[i] = Card_SN[i];

				EEPROM_WRITE(0,Card_SN,4);//写入EEPROM
			   }
		       break ;

		 case 3:		
		       display(1,0,4);      //密码设置
			   display2(3,0,table,8);
			   key_count=0;
			   while(1)
			   {  
			      key_value=key_scan();	  
				  if(key_value==12) 
				   { 
				     states--;
	                 return;
					}
			 
			      if(key_value>=0 && key_value <=9)//有按键输入
				  {		  				    				    
					table[key_count++]=key_value+'0';
					display2(3,0,table,8);			
				  }
				  
				  if(key_value==11)//退格
				  {								    
					table[--key_count]='-';
					display2(3,0,table,8); 				
				  }

				   if(key_count==8 && key_value == 15) //按下确定键
				  
				  { 
				  
				    for(i=0;i<8;i++)
				     KEY_BUF[i]=table[i];	
				   	 EEPROM_WRITE(7,KEY_BUF,8);//写入EEPROM
				 
				   break;
				   }
                } 			 
				
		       break ;

		default : break;

	 }	  		

	if( bPass )		 //处理成功
	{
		bPass = 0;
		pass();	
	
	}

	if( bWarn )	     //处理失败
	{
		bWarn = 0;
		warn();
	}

}

