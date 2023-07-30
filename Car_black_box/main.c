/*
 * File    :   main.c
 * Author  : Gokulkrishnan
 * Project : Car Black Box
 * date    : 12-06-2022
 */

#include "main.h"

#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)

extern unsigned int return_time;
extern unsigned char clock_reg[3];
extern unsigned int access;
unsigned int check_flag = 1, print_flag = 1;
unsigned int add = 5,view_add = 0;

static void init_config(void) {
        //write initialization code here
        //initialize clcd
        init_clcd();

        //initialize i2c
        init_i2c(100000);

        //initialize ds1307
        init_ds1307();
        //initialize digital_keypad
        init_digital_keypad();
        //initialize adc
        init_adc();
        //initialize timer2
        init_timer2();

        init_uart(9600);

        PEIE=1;
        GIE=1;
}
//fuction to print welcome message
void welcome(){
        clear_screen();
        clcd_print("    Welcome     ",LINE1(0));
        clcd_print("     _||_      ",LINE2(0));
        for(long int i=700000;i--;);
        clear_screen();

        clcd_print("Project",LINE1(4));
        clcd_print("Car Black Box",LINE2(2));
        for(long int i=700000;i--;);
        clear_screen();

        clcd_print("Project by",LINE1(3));
        clcd_print("Gokulkrishnan S", LINE2(0));
        for(long int i=700000;i--;);
        clear_screen();
}
void main(void) {

        init_config();
        welcome();

        unsigned char control_flag = DASH_BOARD_FLAG , reset_flag;
        unsigned char speed = 0;
        char event[3] = "ON" ;
        char *gear[]={"GN ","GR ","G1","G2","G3","G4"};
        unsigned char key=0, gr = 0, menu_pos, key_2;
        unsigned long int delay = 0, delay1 = 0, change = 0, change_v = 0, delay_v = 0;

        //function call to store log event
        log_event(event,speed);
        //store the system password
        eeprom_at24c04_str_write(0x00,"1010");

        //super loop
        while (1) {

                //calculate potentiometer speed
                speed = read_adc()/10;
                if(speed >99){
                        speed = 99;
                }

                //read digital keypad input for single press
                key = read_digital_keypad(STATE);
                for(int j=200;j--;);

                // read digital keypad input for long press
                key_2 = read_digital_keypad(LEVEL);
                if(key_2 == SW4)
                {
                        delay++;
                }
                else if (key_2 != SW4)
                {
                        delay=0;

                }
                if(key_2 == SW5){
                        delay1++;
                }
                else if (key_2 != SW5)
                        delay1 = 0;

                //if SW5 is long pressed head back to home
                if(delay1 == 2000 && key_2 == SW5){
                        clear_screen();
                        clcd_print("Back to Home",LINE1(2));
                        clcd_print("Page",LINE2(6));
                        for(long int i=200000;i--;);
                        clear_screen();
                        control_flag = DASH_BOARD_FLAG;
                }

                //in login menu SW4 is long pressed and take a action where * is pointing
                else if( (control_flag == LOGIN_MENU_FLAG) && key_2== SW4 && delay == 1000){
                        TMR2ON = 0;
                        return_time= 5;
                        delay = 0;
                        clear_screen();

                        switch(menu_pos){
                                case 0:
                                        control_flag = VIEW_LOG_FLAG;
                                        clcd_print("View Logs",LINE1(3));
                                        clcd_print("Selected!",LINE2(3));
                                        for(long int i=100000;i--;);
                                        clear_screen();

                                        break;
                                case 1:
                                        control_flag = CLEAR_LOG_FLAG;
                                        clcd_print("Clear Logs",LINE1(3));
                                        clcd_print("Selected!",LINE2(3));
                                        for(long int i=100000;i--;);
                                        clear_screen();
                                        break;
                                case 2:
                                        control_flag = DOWNLOAD_LOG_FLAG;
                                        clcd_print("Download Logs",LINE1(1));
                                        clcd_print("Selected!",LINE2(3));
                                        for(long int i=300000;i--;);
                                        clear_screen();
                                        break;
                                case 3:
                                        control_flag = SET_TIME_FLAG;
                                        clcd_print("Set time",LINE1(4));
                                        clcd_print("Selected!",LINE2(3));
                                        for(long int i=100000;i--;);
                                        clear_screen();
                                        break;
                                case 4:
                                        control_flag = CHANGE_PASSWORD_FLAG ;
                                        clcd_print("Change pass Logs",LINE1(0));
                                        clcd_print("Selected!",LINE2(3));
                                        for(long int i=100000;i--;);
                                        clear_screen();

                                        break;
                        }

                }

                //copy logs and increment/decrement gear with respective of key(SW1/SW2/SW3))
                if(key == SW1){
                        strcpy(event, "C ");
                        log_event(event,speed);
                }
                else if(key == SW2 && gr < 6){

                        gr++;
                        strcpy(event,gear[gr]);
                        log_event(event,speed);
                }
                else if(key == SW3 && gr > 0){
                        gr--;
                        strcpy(event,gear[gr]);
                        log_event(event,speed);
                }

                //if SW4/SW5 pressed read password from user
                else if( (control_flag == DASH_BOARD_FLAG) && (key == SW4 || key == SW5)){
                        control_flag = LOGIN_FLAG;
                        clear_screen();
                        clcd_print("Enter password",LINE1(1));
                        clcd_write(LINE2(4),INST_MODE);
                        clcd_write(DISP_ON_AND_CURSOR_ON,INST_MODE);
                        __delay_ms(100);
                        reset_flag = RESET_PASSWORD;
                        /* Switching on the Timer2 */
                        TMR2ON = 1;

                }

                //goto the function where control flag pointing to and call respective function and get return.
                switch(control_flag){
                        //for DASH_BOARD_FLAG
                        case DASH_BOARD_FLAG:
                                display_dash_board(event,speed);
                                break;
                                //for LOGIN_FLAG
                        case LOGIN_FLAG:
                                switch(login(reset_flag, key)){
                                        //for RETURN_BACK
                                        case RETURN_BACK:
                                                clear_screen();
                                                clcd_write(DISP_ON_AND_CURSOR_OFF ,INST_MODE);
                                                __delay_ms(100);
                                                control_flag = DASH_BOARD_FLAG;
                                                TMR2ON = 0;
                                                break;
                                                //for LOGIN_SUCCESS
                                        case LOGIN_SUCCESS:
                                                clear_screen();
                                                clcd_write(DISP_ON_AND_CURSOR_OFF ,INST_MODE);
                                                __delay_ms(100);
                                                control_flag = LOGIN_MENU_FLAG;
                                                TMR2ON = 1;
                                                return_time= 5;
                                                reset_flag = RESET_PASSWORD;
                                                continue;
                                                break;
                                }
                                break;
                                //for LOGIN_MENU_FLAG
                        case LOGIN_MENU_FLAG:
                                menu_pos = login_menu(reset_flag,key,key_2 );
                                if(menu_pos == 5){
                                        control_flag = DASH_BOARD_FLAG;
                                        clcd_print("Back to Home",LINE2(2));
                                        clcd_print("Due to inactive",LINE1(0));
                                        for(long int i=300000;i--;);
                                        clear_screen();
                                }
                                break;
                                //for VIEW_LOG_FLAG
                        case VIEW_LOG_FLAG:
                                if(delay == 40 && key_2==SW4 ){
                                        delay = 0;
                                        clear_screen();
                                        control_flag = LOGIN_MENU_FLAG;
                                        clcd_print("Back to Menu",LINE1(2));
                                        clcd_print("                ",LINE2(0));
                                        for(long int i=600000;i--;);
                                        clear_screen();
                                        TMR2ON = 1;

                                }
                                else if(delay1 == 40 && key_2 == SW5){
                                        delay = 0;
                                        clear_screen();
                                        control_flag = DASH_BOARD_FLAG;
                                        clcd_print("Back to Home",LINE1(2));
                                        clcd_print("Page",LINE2(6));
                                        for(long int i=600000;i--;);
                                        clear_screen();
                                }
                                else
                                        control_flag = view_log(access, key, change_v, key_2);
                                break;
                                //for CLEAR_LOG_FLAG
                        case CLEAR_LOG_FLAG:
                                access = 0;
                                control_flag = clear_logs();
                                break;
                                //for DOWNLOAD_LOG_FLAG
                        case DOWNLOAD_LOG_FLAG:
                                control_flag = download();
                                break;
                                //for SET_TIME_FLAG
                        case SET_TIME_FLAG:
                                if(delay == 40 && key_2==SW4){
                                        delay = 0;
                                        change = 1;
                                }
                                control_flag = time_edit_mode(clock_reg, key, change, key_2);
                                break;
                                //for CHANGE_PASSWORD_FLAG
                        case CHANGE_PASSWORD_FLAG:
                                control_flag = change_password();
                                break;
                }
                reset_flag = RESET_NOTHING;
        }
        return;
}