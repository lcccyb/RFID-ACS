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

INT8U states=0;//״̬��

INT8U Card_type[2],	//��Ƭ����
      Card_SN[4]={0},//IC����
	  Card_SN_BUF[4]={0xff,0xff,0xff,0xff},//���ŵǼǻ�����	   
	  KEY_BUF[8]={'0','0','0','0','0','0','0','0'};

INT8U bPass = 0; bWarn = 0;	   

void uart_over( void )	//�������ݻ�ԭ
{
	UartCount = 0;
	UartStart = FALSE;
	UartComp = FALSE;
}

void pass( void )
{
   beep1();
   Delay_ms(2000);//�ÿ�Ƭ�뿪
   LED_BLINK_1();
}

void warn(void)
{
  LED_BLINK_2();
}


INT8U IC_READ( void )
{
    INT8U ID_ASC[8],i;
	if( PcdRequest( PICC_REQIDL, Card_type ) != MI_OK )//Ѱ��������δ��������״̬�Ŀ������ؿ�Ƭ���� 2�ֽ�	
	{
		if( PcdRequest( PICC_REQIDL, Card_type ) != MI_OK )//Ѱ��������δ��������״̬�Ŀ������ؿ�Ƭ���� 2�ֽ�	
		{
		   
			return FALSE;
		}	
	}
	
	if( PcdAnticoll( Card_SN ) != MI_OK ) //����ײ�����ؿ������к� 4�ֽ� 
	{

		bWarn = 1;
		return FALSE;	
	}

	bPass = 1;
	send_bytes(Card_type,2);
	send_bytes(Card_SN,4);
	
	for(i=0;i<4;i++) //��ID��ת����ASCII
	{
	  if(Card_SN[i]/16>9)	ID_ASC[i*2]=Card_SN[i]/16+'7';
	    else ID_ASC[i*2]=Card_SN[i]/16+'0';

      if(Card_SN[i]%16>9)	ID_ASC[i*2+1]=Card_SN[i]%16+'7';
	    else ID_ASC[i*2+1]=Card_SN[i]%16+'0';
	}
	display2(3,0,"ID: ",4); //��ʾ����
	display2(3,2,ID_ASC,8);

	return TRUE;
}


void ctrl_process( void )
{		  
   INT8U i,key_count,key_value=16,table[8]="--------",statesbuf;	
	
	if(states!=statesbuf)  //״̬�ı䣬����
	{
	  	display_clear_line(2);
	    display_clear_line(3);
		statesbuf=states;
	}
    switch(states)
	{
       case 0:	          //IC����������
		  	  display(1,0,1); 
		      if(IC_READ()) 
			  {
			   
                if ((Card_SN[0]==Card_SN_BUF[0])
			      &&(Card_SN[1]==Card_SN_BUF[1])
				  &&(Card_SN[2]==Card_SN_BUF[2])
				  &&(Card_SN[3]==Card_SN_BUF[3]))
				  {
					relay_ON();//�ƿ���
					display(2,0,5);
				
			      }
				  else 	 display(2,0,6);
				    relay_OFF();
			   }
			
			  break;

        case 1:		
		       display(1,0,2);    //��������
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
				  if(key_value>=0 && key_value <=9)//�а�������
				  {		  				    				    
					table[key_count++]=key_value+'0';
					display2(3,0,table,8);			
				  }
				  
				  if(key_value==11)//�˸�
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
						 table[7]==KEY_BUF[7] )	  //������ȷ 
				
				     { 
					   bPass=1;
					   relay_ON();//�ƿ���
                       display(2,0,5);
					   relay_OFF();
					   break;				  
					
					 }
					  else 	 //�������
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
		       display(1,0,3);      //IC���Ǽ�
	           if(IC_READ())  
			   {
					 
			    for(i=0;i<4;i++)
			    Card_SN_BUF[i] = Card_SN[i];

				EEPROM_WRITE(0,Card_SN,4);//д��EEPROM
			   }
		       break ;

		 case 3:		
		       display(1,0,4);      //��������
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
			 
			      if(key_value>=0 && key_value <=9)//�а�������
				  {		  				    				    
					table[key_count++]=key_value+'0';
					display2(3,0,table,8);			
				  }
				  
				  if(key_value==11)//�˸�
				  {								    
					table[--key_count]='-';
					display2(3,0,table,8); 				
				  }

				   if(key_count==8 && key_value == 15) //����ȷ����
				  
				  { 
				  
				    for(i=0;i<8;i++)
				     KEY_BUF[i]=table[i];	
				   	 EEPROM_WRITE(7,KEY_BUF,8);//д��EEPROM
				 
				   break;
				   }
                } 			 
				
		       break ;

		default : break;

	 }	  		

	if( bPass )		 //����ɹ�
	{
		bPass = 0;
		pass();	
	
	}

	if( bWarn )	     //����ʧ��
	{
		bWarn = 0;
		warn();
	}

}

