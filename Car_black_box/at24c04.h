/* 
 * File    : at24c04.h
 * Author  : Gokulkrishnan
 * Project : Car Black Box
 * date    : 12-06-2022
 */

#ifndef AT24C04_H
#define	AT24C04_H

#define SLAVE_WRITE_EE               0XA0 
#define SLAVE_READ_EE                0XA1 

#define MEMORY_ADDRESS               0x00


unsigned char  eeprom_at24c04_read(unsigned char memory_loc);
void eeprom_at24c04_byte_write(unsigned char memory_loc,unsigned char data);
void eeprom_at24c04_str_write(unsigned char memory_loc,unsigned char *data);

#endif	/* AT24C04_H */

