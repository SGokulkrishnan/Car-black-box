/*
 *
 * File    : time_edit_mode.c
 * Author  : Gokulkrishnan
 * Project : Car Black Box
 * date    : 12-06-2022
 */
#include "main.h"

//extern char time[9];
static int flag = 10;

unsigned int time_edit_mode(unsigned char *clock_reg,unsigned char key, unsigned int change, unsigned char key2){
        //print message to user
        clcd_print("Time (HH:MM:SS)", LINE1(0));
        unsigned int hr, min, sec, *pos ;
        //blink the chosen position
        if(flag == 1){
                pos = &sec;
                clcd_putch(' ' ,LINE2(7));
                clcd_putch(' ' ,LINE2(6));
        }
        else if(flag == 2){
                pos = &min;
                clcd_putch(' ' ,LINE2(4));
                clcd_putch(' ' ,LINE2(3));
        }
        else if(flag == 3){
                pos = &hr;
                clcd_putch(' ' ,LINE2(1));
                clcd_putch(' ' ,LINE2(0));
        }

        //read the time form RTC at 1st time
        if(flag == 10){
                hr  = (((clock_reg[0] >> 4) & 0x03) * 10);
                hr += ((clock_reg[0] & 0x0F));

                min  = ((clock_reg[1] >> 4) & 0x07) *10;
                min += (clock_reg[1] & 0x0F);

                sec = ((clock_reg[2] >> 4) & 0x07) *10;
                sec += (clock_reg[2] & 0x0F);

                //pos = &sec;
                flag = 1;
        }

        //SW4 is pressed increment pointing respective field
        if(key == SW4  ){
                (*pos)++;
                if(pos == &hr && *(pos)==24)
                        (*pos) = 0;
                else if((*pos) == 60)
                        (*pos) = 0;
        }
        //SW5 is pressed decrement pointing respective field
        else if(key == SW5){
                (*pos)--;
                if(pos == &hr && *(pos) == 0)
                        *(pos) = 23;
                else if((*pos) == 0)
                        (*pos) = 59;
        }
        //SW6 is pressed change field
        if(key == SW6){
                flag++;
                if(flag >= 4){
                        flag = 1;
                }
        }
        //if change is true change the RTC time
        if(change){

                (*pos)--;
                if(pos == &hr && *(pos) == 0)
                        *(pos) = 23;
                else if((*pos) == 0)
                        (*pos) = 59;

                write_ds1307(SEC_ADDR,  sec);
                write_ds1307(MIN_ADDR,  min);
                write_ds1307(HOUR_ADDR, hr);
                clear_screen();
                clcd_print("Time has changed",LINE1(0));
                clcd_print("     (^_^)/     ",LINE2(0));
                for(long int i=500000;i--;);
                clear_screen();
                TMR2ON = 1;
                return LOGIN_MENU_FLAG;
        }

        //print the time
        if(key2 != SW5 && key2 != SW4){
                clcd_putch((hr/10) + '0' ,LINE2(0));
                clcd_putch((hr%10) + '0' ,LINE2(1));
                clcd_putch(':',LINE2(2));

                clcd_putch((min/10) + '0' ,LINE2(3));
                clcd_putch((min%10) + '0' ,LINE2(4));;
                clcd_putch(':',LINE2(5));

                clcd_putch((sec/10) + '0' ,LINE2(6));
                clcd_putch((sec%10) + '0' ,LINE2(7));
        }
        return SET_TIME_FLAG;

}