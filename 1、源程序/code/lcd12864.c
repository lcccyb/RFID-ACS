#include <reg52.h>
#include <intrins.h>
#include "main.h"
#define uchar unsigned char
#define uint  unsigned int

/*LCD12864  �˿ڶ���*/
#define LCD_data  P0            //���ݿ�
sbit LCD_RS  =  P3^5;            //�Ĵ���ѡ������ 
sbit LCD_RW  =  P3^6;            //Һ����/д����
sbit LCD_EN  =  P3^4;            //Һ��ʹ�ܿ���
sbit LCD_PSB =  P3^7;            //��/����ʽ����,����Һ������Ҫ

uchar code dis0[]="  IC���Ž�ϵͳ  ";
uchar code dis1[]="1.IC������      ";
uchar code dis2[]="2.��������:8λ  ";
uchar code dis3[]="3.IC���Ǽ�      ";
uchar code dis4[]="4.��������:8λ  ";
uchar code dis5[]="�ɹ�            ";
uchar code dis6[]="ʧ��            ";


void delay(int ms)	 //��ʱxms
{
    while(ms--)
	{
      uchar i;
	  for(i=0;i<250;i++)  
	   {
	    _nop_();			   
		_nop_();
		_nop_();
		_nop_();
	   }
	}
}	

void delayNOP()  //��ʱ4us
{ _nop_(); _nop_(); _nop_(); _nop_();}
 /*******************************************************************/
/*                                                                 */
/*��Ϊ���ŵĶ���DB0..DB7�Ǵ�P1.7...P1.0���е�                                                    */
/*����˳����Ҫ����һ��                                             */
/*                                                                 */
/*******************************************************************/
  INT8U swapbit(INT8U udata)
{
  INT8U ii,tmp=0;
  for(ii=0;ii<8;ii++)
  {
    tmp<<=1;
    if(udata&0x01)
    {
      tmp|=0x01;
    }
    udata>>=1;
  }
  return tmp;
}

/*******************************************************************/
/*                                                                 */
/*���LCDæ״̬                                                    */
/*lcd_busyΪ1ʱ��æ���ȴ���lcd-busyΪ0ʱ,�У���дָ�������ݡ�      */
/*                                                                 */
/*******************************************************************/
bit lcd_busy()
 {                          
    bit result;
    LCD_RS = 0;
    LCD_RW = 1;
    LCD_EN = 1;
    delayNOP();
    result = (bit)(LCD_data&0x80);
    LCD_EN = 0;
    return(result); 
 }
/*******************************************************************/
/*                                                                 */
/*дָ�����ݵ�LCD                                                  */
/*RS=L��RW=L��E=�����壬D0-D7=ָ���롣                             */
/*                                                                 */
/*******************************************************************/
void write_cmd(unsigned char WCLCM,BuysC)
{                          
   unsigned int K=20000;     
   while(lcd_busy() && K--);  //Ԥ��û��LCDʱ��ͣ����
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 0;
    _nop_();
    _nop_(); 
    LCD_data = swapbit(WCLCM);
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0;  
}
/*******************************************************************/
/*                                                                 */
/*д��ʾ���ݵ�LCD                                                  */
/*RS=H��RW=L��E=�����壬D0-D7=���ݡ�                               */
/*                                                                 */
/*******************************************************************/
void lcd_wdat(unsigned char WDLCM)
{                          
   unsigned int K=20000;     
   while(lcd_busy() && K--);  //Ԥ��û��LCDʱ��ͣ����
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_EN = 0;
    LCD_data= swapbit(WDLCM);
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0; 
}
/*******************************************************************/
/*                                                                 */
/*  LCD��ʼ���趨                                                  */
/*                                                                 */
/*******************************************************************/
void lcd_init()
{ 

   LCD_PSB = 1;         //���ڷ�ʽ	������Һ������Ҫ
    write_cmd(0x36);      //����ָ�����
    delay(5);
    write_cmd(0x30);      //����ָ���
	delay(5);
    write_cmd(0x0C);      //��ʾ�����ع��
    delay(5);
    write_cmd(0x01);      //���LCD����ʾ����
    delay(5);
}	  
/*********************************************************/
/*                                                       */
/* �趨��ʾλ��                                          */
/*                                                       */
/*********************************************************/
void lcd_pos(uchar X,uchar Y)
{                          
   uchar  pos;
   if (X==0)
     {X=0x80;}
   else if (X==1)
     {X=0x90;}
   else if (X==2)
     {X=0x88;}
   else if (X==3)
     {X=0x98;}
   pos = X+Y ;  
   write_cmd(pos);     //��ʾ��ַ
}

/*********************************************************/
/*														 */
/* ��ʾ������           							     */
/*                                                       */
/*********************************************************/
void display_all()
 {

    uchar i;          
	   	  
	  lcd_pos(0,0);             //��һ����ʾ����0~9
	  i = 0;   
      while(dis0[i] != '\0')
       {
         lcd_wdat(dis0[i]);      //��ʾ��ĸ
         i++;
       }
	     
      lcd_pos(1,0);             //�ڶ�����ʾ��ĸ
	  i = 0;   
      while(dis1[i] != '\0')
       {
         lcd_wdat(dis1[i]);      //��ʾ��ĸ
         i++;
       }

 	  lcd_pos(2,0);             //��������ʾ����
	  i = 0;
      while(dis2[i] != '\0')
       {
         lcd_wdat(dis2[i]);     
         i++;
       }

	  lcd_pos(3,0);             //��������ʾ�㹤���ߵ���
	  i = 0;
      while(dis3[i] != '\0')
       {
         lcd_wdat(dis3[i]);     
         i++;
       }
	  
}

/*********************************************************/
/*														 */
/* �ض��ַ���ʾ������           							     */
/*                                                     

C:
0="  IC����̨��    ";
1="1.������....    ";
2="2.�Ǽ�....      ";
3="�ɹ�            ";
4="ʧ��            ";

/*********************************************************/
void display(uchar a,uchar b,uchar c)
 {

      uchar i;          	  
	  lcd_pos(a,b);             //��һ����ʾ����0~9
	  i = 0; 
	  switch(c)
	  {
	       case 0: 
                while(dis0[i] != '\0')
              {
                lcd_wdat(dis0[i]);      //��ʾ��ĸ
                i++;
              }
			  break;

	  	   case 1: 
               while(dis1[i] != '\0')
              {
                lcd_wdat(dis1[i]);      //��ʾ��ĸ
                i++;
              }
			  break;

			 case 2: 
               while(dis2[i] != '\0')
              {
                lcd_wdat(dis2[i]);      //��ʾ��ĸ
                i++;
              }
			  break;


			  case 3: 
               while(dis3[i] != '\0')
              {
                lcd_wdat(dis3[i]);      //��ʾ��ĸ
                i++;
              }
			  break;	  
			  
			  
	   case 4: 
               while(dis4[i] != '\0')
              {
                lcd_wdat(dis4[i]);      //��ʾ��ĸ
                i++;
              }		 
			  break;	  

	   case 5: 
               while(dis5[i] != '\0')
              {
                lcd_wdat(dis5[i]);      //��ʾ��ĸ
                i++;
              }		 
			  break;
	   case 6: 
               while(dis6[i] != '\0')
              {
                lcd_wdat(dis6[i]);      //��ʾ��ĸ
                i++;
              }		 
			  break; 

			  default :break;
	  }
 }

 /*********************************************************/
/*														 */
/* һ���ַ���ʾ������           						 */
/*                                                     

*p:����
c:����

/*********************************************************/
void display2(uchar a,uchar b,uchar *p,uchar num)
 {

      uchar i;       	  
	  lcd_pos(a,b);             //��һ����ʾ����0~9	
      for(i=0;i<num;i++)
          lcd_wdat(p[i]);      //��ʾ��ĸ
              
 }

 /********************
 ���ĳһ��
 *********************/
 void display_clear_line(uchar a)
 {
   uchar i;
   lcd_pos(a,0);
   for(i=0;i<16;i++)
     lcd_wdat(' ');
 }
