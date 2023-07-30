/*
 * File    : at24c04.c
 * Author  : Gokulkrishnan
 * Project : Car Black Box
 * date    : 12-06-2022
 */

#include <xc.h>
#include "at24c04.h"
#include "i2c.h"

//function read from EEPROM
unsigned char eeprom_at24c04_read(unsigned char memory_loc)
{
    unsigned char recived_data = 0;
    
    i2c_start();
    i2c_write(SLAVE_WRITE_EE);
    i2c_write(memory_loc);
    i2c_rep_start();
    i2c_write(SLAVE_READ_EE);
    recived_data = i2c_read(0);
    i2c_stop();
    
    return recived_data;
}
//function char write on EEPROM
void eeprom_at24c04_byte_write(unsigned char memory_loc,unsigned char data){
   
    i2c_start();
    i2c_write(SLAVE_WRITE_EE);
    i2c_write(memory_loc);
    i2c_write(data);
    i2c_stop();
    
}
//function write string on EEPROM
void eeprom_at24c04_str_write(unsigned char memory_loc,unsigned char *data){
   
    while(*data != 0){
        eeprom_at24c04_byte_write(memory_loc,*data);
        data++;
        memory_loc++;
    }
    
}


