/*
 *
 * File    : download.c
 * Author  : Gokulkrishnan
 * Project : Car Black Box
 * date    : 12-06-2022
 */

#include"main.h"
extern unsigned int access;


unsigned char download(){
        unsigned char view_add = 0;
        //print message to user
        puts("Downloaded logs : \n");
        puts("\n");
        if(access == 0){
                puts("No Logs available \n");
                view_add++;
        }
        while(1){
                //transfer all the data to PC untill view_add becomes greater than access
                if(access > view_add) {

                        char add = 5;

                        add = view_add * 10 + add;

                        putchar(view_add+'0');
                        putchar(' '    );

                        putchar(eeprom_at24c04_read(add+0));
                        putchar(eeprom_at24c04_read(add+1));

                        putchar(' '    );
                        putchar(':'    );
                        putchar(' '    );

                        putchar(eeprom_at24c04_read(add+2));
                        putchar(eeprom_at24c04_read(add+3));

                        putchar(' '    );
                        putchar(':'    );
                        putchar(' '    );

                        putchar(eeprom_at24c04_read(add+4));
                        putchar(eeprom_at24c04_read(add+5));

                        putchar(' '    );
                        //to display time eventvent,LINE2(
                        putchar(eeprom_at24c04_read(add+6));
                        putchar(eeprom_at24c04_read(add+7));

                        putchar(' '    );

                        //to display speed
                        putchar(eeprom_at24c04_read(add+8));
                        putchar(eeprom_at24c04_read(add+9));

                        putchar('\n'    );

                        view_add++;
                        clcd_print("Data sending.. ",LINE1(0));
                        __delay_ms(600);
                }
                else{
                        //after completion of transfer data to PC return to main menu
                        puts("\n");
                        puts("Data received successfully \n");
                        clear_screen();
                        TMR2ON = 1;
                        clcd_print("Back to Menu",LINE1(2));
                        clcd_print("                ",LINE2(0));
                        for(long int i=600000;i--;);
                        clear_screen();
                        return LOGIN_MENU_FLAG;
                }
        }


}