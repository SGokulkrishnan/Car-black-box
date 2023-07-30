/* 
 * File    : car_black_box.h
 * Author  : Gokulkrishnan
 * Project : Car Black Box
 * date    : 12-06-2022
 */

#ifndef CAR_BLACK_BOX_H
#define	CAR_BLACK_BOX_H

void display_dash_board(unsigned char *event, unsigned int speed);
void log_event(unsigned char event[], unsigned char speed);
void clear_screen();
unsigned char login(unsigned char reset_flag, unsigned char key);
unsigned char login_menu(unsigned char reset_flag, unsigned char key, unsigned char key2);

#endif	/* CAR_BLACK_BOX_H */

