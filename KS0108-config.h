#ifndef _KS0108_CONFIG_H    /* Guard against multiple inclusion */
#define _KS0108_CONFIG_H

#include <xc.h>

// Set this if you want to use the delay functions from the CORETIMER Harmony component. Otherwise these are expected to be defined in "delay.h"
#define USE_CORETIMER

// Each controller handles a 64x64 half of the screen. Set this if they are around the wrong way
//#define KS0108_SWAP_SIDES

//LCD_DATA (8-bit parallel) = RJ0..7
//LCD_RS = RH13
//LCD_RW = RH14
//LCD_EN = RH15
//LCD_RST = RK7
//LCD_CS1 = RD6
//LCD_CS2 = RD7

#define KS0108_DATA_DIRN_IN()     (TRISJSET = 0x00FF)
#define KS0108_DATA_DIRN_OUT()    (TRISJCLR = 0x00FF)
#define KS0108_DATA_INPUT()       (PORTJ & 0x00FF)
#define KS0108_DATA_OUTPUT(d)     {LATJCLR = 0x00FF; LATJSET = d;}
#define KS0108_CTRL_DIRN_OUT()    {TRISHCLR = (1<<13)&(1<<14)&(1<<15); TRISKCLR = (1<<7); TRISDCLR = (1<<6)&(1<<7);}
#define KS0108_RST_HIGH()         (LATKSET = (1<<7)) // RST -> K7
#define KS0108_RST_LOW()          (LATKCLR = (1<<7))
#define KS0108_E_HIGH()           (LATHSET = (1<<15)) // E -> H15
#define KS0108_E_LOW()            (LATHCLR = (1<<15))
#define KS0108_RW_HIGH()          (LATHSET = (1<<14)) // RW -> H14
#define KS0108_RW_LOW()           (LATHCLR = (1<<14))
#define KS0108_RS_HIGH()          (LATHSET = (1<<13)) // RS -> H13
#define KS0108_RS_LOW()           (LATHCLR = (1<<13))
#define KS0108_CS1_HIGH()         (LATDSET = (1<<6))  // CS1 -> D6
#define KS0108_CS1_LOW()          (LATDCLR = (1<<6))
#define KS0108_CS2_HIGH()         (LATDSET = (1<<7))  // CS2 -> D7
#define KS0108_CS2_LOW()          (LATDCLR = (1<<7))


/* these work with the small LCD*/
#define KS0108_EN_H 2
#define KS0108_EN_L 2

#endif /* _KS0108_CONFIG_H */
