#include <reg52.h>
#include <intrins.h>
#include "main.h"
#define uchar unsigned char
#define uint  unsigned int

/*LCD12864  端口定义*/
#define LCD_data  P0            //数据口
sbit LCD_RS  =  P3^5;            //寄存器选择输入 
sbit LCD_RW  =  P3^6;            //液晶读/写控制
sbit LCD_EN  =  P3^4;            //液晶使能控制
sbit LCD_PSB =  P3^7;            //串/并方式控制,部分液晶不需要

uchar code dis0[]="  IC卡门禁系统  ";
uchar code dis1[]="1.IC卡输入      ";
uchar code dis2[]="2.密码输入:8位  ";
uchar code dis3[]="3.IC卡登记      ";
uchar code dis4[]="4.密码设置:8位  ";
uchar code dis5[]="成功            ";
uchar code dis6[]="失败            ";


void delay(int ms)	 //延时xms
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

void delayNOP()  //延时4us
{ _nop_(); _nop_(); _nop_(); _nop_();}
 /*******************************************************************/
/*                                                                 */
/*因为引脚的定义DB0..DB7是从P1.7...P1.0排列的                                                    */
/*所以顺序需要调换一下                                             */
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
/*检查LCD忙状态                                                    */
/*lcd_busy为1时，忙，等待。lcd-busy为0时,闲，可写指令与数据。      */
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
/*写指令数据到LCD                                                  */
/*RS=L，RW=L，E=高脉冲，D0-D7=指令码。                             */
/*                                                                 */
/*******************************************************************/
void write_cmd(unsigned char WCLCM,BuysC)
{                          
   unsigned int K=20000;     
   while(lcd_busy() && K--);  //预防没插LCD时候停在这
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
/*写显示数据到LCD                                                  */
/*RS=H，RW=L，E=高脉冲，D0-D7=数据。                               */
/*                                                                 */
/*******************************************************************/
void lcd_wdat(unsigned char WDLCM)
{                          
   unsigned int K=20000;     
   while(lcd_busy() && K--);  //预防没插LCD时候停在这
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
/*  LCD初始化设定                                                  */
/*                                                                 */
/*******************************************************************/
void lcd_init()
{ 

   LCD_PSB = 1;         //并口方式	，部分液晶不需要
    write_cmd(0x36);      //扩充指令操作
    delay(5);
    write_cmd(0x30);      //基本指令操
	delay(5);
    write_cmd(0x0C);      //显示开，关光标
    delay(5);
    write_cmd(0x01);      //清除LCD的显示内容
    delay(5);
}	  
/*********************************************************/
/*                                                       */
/* 设定显示位置                                          */
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
   write_cmd(pos);     //显示地址
}

/*********************************************************/
/*														 */
/* 显示主程序           							     */
/*                                                       */
/*********************************************************/
void display_all()
 {

    uchar i;          
	   	  
	  lcd_pos(0,0);             //第一行显示数字0~9
	  i = 0;   
      while(dis0[i] != '\0')
       {
         lcd_wdat(dis0[i]);      //显示字母
         i++;
       }
	     
      lcd_pos(1,0);             //第二行显示字母
	  i = 0;   
      while(dis1[i] != '\0')
       {
         lcd_wdat(dis1[i]);      //显示字母
         i++;
       }

 	  lcd_pos(2,0);             //第三行显示文字
	  i = 0;
      while(dis2[i] != '\0')
       {
         lcd_wdat(dis2[i]);     
         i++;
       }

	  lcd_pos(3,0);             //第四行显示广工无线电社
	  i = 0;
      while(dis3[i] != '\0')
       {
         lcd_wdat(dis3[i]);     
         i++;
       }
	  
}

/*********************************************************/
/*														 */
/* 特定字符显示主程序           							     */
/*                                                     

C:
0="  IC卡点台灯    ";
1="1.读卡中....    ";
2="2.登记....      ";
3="成功            ";
4="失败            ";

/*********************************************************/
void display(uchar a,uchar b,uchar c)
 {

      uchar i;          	  
	  lcd_pos(a,b);             //第一行显示数字0~9
	  i = 0; 
	  switch(c)
	  {
	       case 0: 
                while(dis0[i] != '\0')
              {
                lcd_wdat(dis0[i]);      //显示字母
                i++;
              }
			  break;

	  	   case 1: 
               while(dis1[i] != '\0')
              {
                lcd_wdat(dis1[i]);      //显示字母
                i++;
              }
			  break;

			 case 2: 
               while(dis2[i] != '\0')
              {
                lcd_wdat(dis2[i]);      //显示字母
                i++;
              }
			  break;


			  case 3: 
               while(dis3[i] != '\0')
              {
                lcd_wdat(dis3[i]);      //显示字母
                i++;
              }
			  break;	  
			  
			  
	   case 4: 
               while(dis4[i] != '\0')
              {
                lcd_wdat(dis4[i]);      //显示字母
                i++;
              }		 
			  break;	  

	   case 5: 
               while(dis5[i] != '\0')
              {
                lcd_wdat(dis5[i]);      //显示字母
                i++;
              }		 
			  break;
	   case 6: 
               while(dis6[i] != '\0')
              {
                lcd_wdat(dis6[i]);      //显示字母
                i++;
              }		 
			  break; 

			  default :break;
	  }
 }

 /*********************************************************/
/*														 */
/* 一般字符显示主程序           						 */
/*                                                     

*p:内容
c:个数

/*********************************************************/
void display2(uchar a,uchar b,uchar *p,uchar num)
 {

      uchar i;       	  
	  lcd_pos(a,b);             //第一行显示数字0~9	
      for(i=0;i<num;i++)
          lcd_wdat(p[i]);      //显示字母
              
 }

 /********************
 清除某一行
 *********************/
 void display_clear_line(uchar a)
 {
   uchar i;
   lcd_pos(a,0);
   for(i=0;i<16;i++)
     lcd_wdat(' ');
 }
