/*
 * File    : change_password.c
 * Author  : Gokulkrishnan
 * Project : Car Black Box
 * date    : 12-06-2022
 */

#include "main.h"

unsigned char change_password(){
        char npassword[4], re_npassword[4], key, i=0, pass_time = 1;

        //print details
        clcd_print("Enter new passwd", LINE1(0));
        __delay_ms(2000);
        while(1){
                //read the digital kepady input
                key = read_digital_keypad(STATE);
                for(int j=300;j--;);

                //read pass from user
                if(pass_time == 1){
                        if( key == SW4 && i<4 ){
                                npassword[i] = '1';
                                i++;
                                clcd_putch('*', LINE2(i+4));
                        }
                        else if( key == SW5 && i<4){
                                npassword[i]='0';
                                i++;
                                clcd_putch('*', LINE2(i+4));
                        }
                        if(i == 4){
                                pass_time = 2;
                                i=0;
                                clear_screen();
                                clcd_print("Re Enter new pas", LINE1(0));
                        }
                }
                //read pass 2md time from user
                else if(pass_time == 2)
                {
                        if( key == SW4 && i<4 ){
                                re_npassword[i] = '1';
                                i++;
                                clcd_putch('*', LINE2(i+4));
                        }
                        else if( key == SW5 && i<4){
                                re_npassword[i]='0';
                                i++;
                                clcd_putch('*', LINE2(i+4));
                        }
                        if(i == 4){
                                pass_time = 0;
                                clear_screen();
                        }
                }
                //is both pass are match update else show error message and dont update
                else if(pass_time == 0){

                        if(strncmp(npassword, re_npassword,4) == 0){
                                clear_screen();
                                eeprom_at24c04_str_write(0x00,npassword);
                                clcd_print("Password changed",LINE1(0));
                                clcd_print("Successfully!",LINE2(1));
                                __delay_ms(2000);
                                clear_screen();
                                TMR2ON = 1;
                                return LOGIN_MENU_FLAG;
                        }
                        else {
                                clear_screen();

                                clcd_print("Password change",LINE1(0));
                                clcd_print("Failed",LINE2(5));
                                __delay_ms(2000);
                                clear_screen();
                                TMR2ON = 1;
                                return LOGIN_MENU_FLAG;
                        }

                }

        }
}