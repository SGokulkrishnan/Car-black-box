/* 
 * File    :   download.h
 * Author  : Gokulkrishnan
 * Project : Car Black Box
 * date    : 12-06-2022
 */

#ifndef DOWNLOAD_H
#define	DOWNLOAD_H

#define FOSC                20000000

unsigned char download();

void init_uart(unsigned long baud);
unsigned char getchar(void);
void putchar(unsigned char data);
void puts(const char *s);

#endif	/* DOWNLOAD_H */

