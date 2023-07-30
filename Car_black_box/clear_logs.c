/*
 * File    : clear_logs.c
 * Author  : Gokulkrishnan
 * Project : Car Black Box
 * date    : 12-06-2022
 */

#include "main.h"

unsigned char clear_logs(){
        unsigned int key, pre_key = 0,delay = 0, delay1 = 0;

        while(1){
                //print message to user
                clcd_print("All logs cleared", LINE1(0));
                clcd_print("    /(^_^)/    ",LINE2(0));
                //read digital keypad input from user
                key = read_digital_keypad(LEVEL);

                //if SW4 presssed go bac to menue
                if(key == SW4 && key == pre_key  ){
                        if(delay++ == 30)
                        {
                                clear_screen();
                                delay = 0;
                                TMR2ON = 1;
                                clcd_print("Back to Menu",LINE1(2));
                                clcd_print("                ",LINE2(0));
                                for(long int i=600000;i--;);
                                clear_screen();
                                return LOGIN_MENU_FLAG;
                        }
                }
                else delay = 0;

                //if SW4 presssed go bac to home
                if( key == SW5 && pre_key == key  ){
                        if(delay1++ == 30)
                        {
                                clear_screen();
                                delay1 = 0;
                                clcd_print("Back to Home",LINE1(2));
                                clcd_print("Page",LINE2(6));
                                for(long int i=600000;i--;);
                                clear_screen();
                                return DASH_BOARD_FLAG;
                        }
                }
                else delay1 = 0;

                if(key != pre_key)
                        pre_key=key;
        }
}