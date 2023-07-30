/*
 *
 * File    : isr.c
 * Author  : Gokulkrishnan
 * Project : Car Black Box
 * date    : 12-06-2022
 */

#include <xc.h>
#include "main.h"

extern unsigned int sec, return_time, min;

void __interrupt() isr(void)
{
        static unsigned int count = 0;

        if (TMR2IF == 1)
        {
                if (++count == 1250)
                {
                        count = 0;

                        //decrement the sec
                        if(sec>0)
                                sec--;
                        //decement min
                        else if(sec == 0 && return_time == 0){
                                min--;
                                sec = 60;
                        }
                        //decrement return time
                        else if(sec == 0 && return_time > 0 && min == 0){
                                return_time --;
                        }
                }

                TMR2IF = 0;
        }
}