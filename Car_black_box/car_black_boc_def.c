/*
 * File    : car_black_box_def.c
 * Author  : Gokulkrishnan
 * Project : Car Black Box
 * date    : 12-06-2022
 */

#include "main.h"

//variable declaration
unsigned char clock_reg[3];
char time[9];  // "HH:MM:SS"
char log[11]; //hhmmssecsp
char pos = -1;
unsigned int access = 0;
unsigned int sec,min, return_time;
char *menu[] = {"View log", "Clear log", "Download log", "Set time", "Change password"};
extern unsigned int check_flag, print_flag;

void get_time()
{
    clock_reg[0] = read_ds1307(HOUR_ADDR); // HH -> BCD 
    clock_reg[1] = read_ds1307(MIN_ADDR); // MM -> BCD 
    clock_reg[2] = read_ds1307(SEC_ADDR); // SS -> BCD 
     
    // HH -> 
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';
    
    // MM 
    time[2] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[3] = (clock_reg[1] & 0x0F) + '0';

    // SS
    time[4] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[5] = (clock_reg[2] & 0x0F) + '0';
    time[6] = '\0';
}

void display_time(){
    get_time();
    
    //HH:MM
    clcd_putch(time[0], LINE2(2));
    clcd_putch(time[1], LINE2(3));
    clcd_putch(':'    , LINE2(4));
    
    clcd_putch(time[2], LINE2(5));
    clcd_putch(time[3], LINE2(6));
    clcd_putch(':'    , LINE2(7));
    
    clcd_putch(time[4], LINE2(8));
    clcd_putch(time[5], LINE2(9));
    clcd_putch(':'    , LINE2(4));
}

void display_dash_board(unsigned char *event, unsigned int speed){
    clcd_print("  TIME     E  SP",LINE1(0));
    //to display time
    display_time();
    //to display time eventvent,LINE2(
    clcd_print(event,LINE2(11));
    //to display speed
    clcd_putch((speed/10)+'0',LINE2(14));
    clcd_putch((speed%10)+'0',LINE2(15));    
}
void log_car_event(){
    //clear_screen();
    char addr;
    addr = 5;
    pos++;
    if(pos == 10){
        pos = 0;
    }
    addr = pos *10 + addr;
    
    eeprom_at24c04_str_write(addr, log);
    if(access < 9){
        access++;
    }
   // clear_screen();
}

void log_event(unsigned char event[], unsigned char speed){
    //clear_screen();
    strncpy(log,time,6);
    strncpy(log+6,event,2);
    
    log[8]=speed/10+'0';
    log[9]=speed%10+'0';
    log[10]='\0';
    log_car_event();
  //  clear_screen();
}

unsigned char login(unsigned char reset_flag, unsigned char key){
    static char npassword[4],i;
    static unsigned char attempt_left;
    
    if ( reset_flag == RESET_PASSWORD){
        i=0;
        attempt_left = 3;        
        npassword[0] = '\0';
        npassword[1] = '\0';
        npassword[2] = '\0';
        npassword[3] = '\0';  
        key = ALL_RELEASED;  
        sec=0;
        min = 0;
        return_time = 3;
    }
    if(return_time == 0){
return RETURN_BACK;        
    }
    if( key == SW4 && i<4){
        npassword[i] = '1';
        i++;
        clcd_putch('*', LINE2(i+4));
        return_time = 3;
    }
    else if(key == SW5){
        npassword[i]='0';
        i++;
        clcd_putch('*', LINE2(i+4));
        return_time = 3;
    }
    if(i == 4){
        char spassword[4];
        //read system password from EEPROM
        for(int j=0;j<4;j++)
        spassword[j] = eeprom_at24c04_read(0x00+j);
        //compare user password with system passowrd
        if(strncmp(spassword, npassword,4) == 0 ){
            clear_screen();
            clcd_print("Login success",LINE1(2));
            __delay_ms(2000);
            return LOGIN_SUCCESS;
            //display main menu screen 
        }
        else{
            attempt_left--;
            if(attempt_left == 0){
                clear_screen();
                 clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                __delay_ms(100);
                clcd_print("you are blocked",LINE1(0));
                clcd_print("min left",LINE2(3));                
                return_time = 0;
                sec = 60;
                min = 15; 
                while(min){
                    clcd_putch(min/10+'0',LINE2(0));
                    clcd_putch(min%10+'0',LINE2(1));
                }
                //timer2 for 15 min
                attempt_left = 3;
            }
            else{
                clear_screen();
                clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                __delay_ms(100);
                clcd_print("Wrong password",LINE1(0));
                clcd_putch(attempt_left+'0', LINE2(1));
                clcd_print("attempt left",LINE2(3));
                __delay_ms(3000);                
            }
            clear_screen();
            clcd_print("Enter password",LINE1(1));
            clcd_write(LINE2(4),INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON,INST_MODE);
            __delay_ms(100);
            i=0;
            return_time = 3;
        }
        
    }
}

unsigned char login_menu(unsigned char reset_flag, unsigned char key, unsigned char key2){

    static unsigned char menu_pos, pre;
    if(reset_flag == RESET_PASSWORD){
        menu_pos = 0;
    }
    
    if(return_time == 0){
        TMR2ON = 0;
        clear_screen();
        return 5;   
    }    
    
    if(key == SW5  ){
        min = sec = 0;
        return_time = 5;
        pre= menu_pos;
        menu_pos++;  
        
        if(menu_pos > 4)
            menu_pos = 4;
 
        clear_screen();
    }
    else if(key == SW4){
        min = sec = 0;
        return_time = 5;
        clear_screen();
      pre= menu_pos;
        menu_pos--;
        if(!(menu_pos >=0 && menu_pos <=4))
            menu_pos = 0;
    }  
    
    if(key2 != SW5 && key2 != SW4){
    if(menu_pos == 4){
        clcd_putch('*',LINE2(0));  
    clcd_print(menu[menu_pos-1],LINE1(2));
    clcd_print(menu[menu_pos],LINE2(2)); 
    }
    else{
    clcd_putch('*',LINE1(0));    
    clcd_print(menu[menu_pos],LINE1(2));
    clcd_print(menu[menu_pos + 1],LINE2(2));
    }
    }
    return pre;
}
void clear_screen(){
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(500);
}
