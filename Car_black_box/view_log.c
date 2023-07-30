/*
 * File    : view_log.c
 * Author  : Gokulkrishnan
 * Project : Car Black Box
 * date    : 12-06-2022
 */

#include "main.h"

unsigned int add ,view_add = 0 ;

unsigned char view_log(unsigned char access,unsigned char key, unsigned int change, unsigned char key2){


        //print details
        if(access == 0){
                clcd_print("No logs available",LINE1(0));
                // __delay_ms(2000);
        }
        else{

                clcd_print("  TIME     E  SP",LINE1(0));
                clcd_putch('#'               , LINE1(0));
        }

        //SW4 pressed show next log
        if(key == SW4  ){
                if(view_add == access )
                        view_add = 0;
                else
                        view_add++;
                add = view_add *10 + 5;
        }
        //SW5 pressed show previous log
        else if(key == SW5){
                if(view_add == 0 )
                        view_add = access;
                else
                        view_add--;
                add = view_add *10 + 5;
        }

        //print logs
        if(access != 0 && key2 != SW5 && key2 != SW4){
                //HH:MM
                clcd_putch(view_add+'0'              , LINE2(0));

                clcd_putch(eeprom_at24c04_read(add+0), LINE2(2));
                clcd_putch(eeprom_at24c04_read(add+1), LINE2(3));
                clcd_putch(':'    , LINE2(4));

                clcd_putch(eeprom_at24c04_read(add+2), LINE2(5));
                clcd_putch(eeprom_at24c04_read(add+3), LINE2(6));
                clcd_putch(':'    , LINE2(7));

                clcd_putch(eeprom_at24c04_read(add+4), LINE2(8));
                clcd_putch(eeprom_at24c04_read(add+5), LINE2(9));
                clcd_putch(':'    , LINE2(4));

                //to display time eventvent,LINE2(
                clcd_putch(eeprom_at24c04_read(add+6),LINE2(11));
                clcd_putch(eeprom_at24c04_read(add+7),LINE2(12));

                //to display speed
                clcd_putch(eeprom_at24c04_read(add+8),LINE2(14));
                clcd_putch(eeprom_at24c04_read(add+9),LINE2(15));
        }
        return VIEW_LOG_FLAG;
}