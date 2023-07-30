/* File              : digital_keypad.c
 * Author            : Gokulkrishnan
 * Assignment        : A26 - Implement a 4 digit key press counter with persistence
 * Date              : 22/05/2023
 */

#include <xc.h>
#include "digital_keypad.h"

void init_digital_keypad(void)
{
        /* Set Keypad Port as input */
        KEYPAD_PORT_DDR = KEYPAD_PORT_DDR | INPUT_LINES;
}

unsigned char read_digital_keypad(unsigned char mode)
{
        static unsigned char once = 1;

        //if the mode is level than return pressed switch
        if (mode == LEVEL_DETECTION)
        {
                return KEYPAD_PORT & INPUT_LINES;
        }

        //else the mode is STATE update once value and return pressed switch
        else
        {
                if (((KEYPAD_PORT & INPUT_LINES) != ALL_RELEASED) && once)
                {
                        once = 0;

                        return KEYPAD_PORT & INPUT_LINES;
                }
                else if ((KEYPAD_PORT & INPUT_LINES) == ALL_RELEASED)
                {
                        once = 1;
                }
        }

        return ALL_RELEASED;
}