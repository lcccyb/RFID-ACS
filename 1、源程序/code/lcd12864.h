#ifndef _LCD12864_H_
#define _LCD12864_H_

#define uchar unsigned char
extern void lcd_init();
extern void display_all();
extern void display(uchar a,uchar b,uchar c); 
extern void display2(uchar a,uchar b,uchar *p,uchar num);
extern  void display_clear_line(uchar a);
#endif
