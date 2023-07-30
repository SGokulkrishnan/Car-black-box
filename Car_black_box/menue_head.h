/* 
 * File:   menue_head.h
 * Author: Gokulkrishnan
 * Project : Car Black Box
 * date    : 12-06-2022
 */

#ifndef MENUE_HEAD_H
#define	MENUE_HEAD_H

//view function
unsigned char view_log(unsigned char access,unsigned char key, unsigned int change, unsigned char key2);


//clear function
unsigned char clear_logs();

//download function
unsigned char download();
void init_uart(unsigned long baud);
unsigned char getchar(void);
void putchar(unsigned char data);
void puts(const char *s);
unsigned char download();

//change password function
unsigned char change_password();

//EDIT TIME function
unsigned int time_edit_mode(unsigned char *,unsigned char key, unsigned int , unsigned char);

#endif	/* MENUE_HEAD_H */

