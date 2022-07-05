#ifndef _ST7920_CONFIG_H /* Guard against multiple inclusion */
#define _ST7920_CONFIG_H

// Set this if you want to use the delay functions from the CORETIMER Harmony component. Otherwise these are expected to be defined in "delay.h"
// Functions required are DelayUs(unsigned long int) and DelayMs(unsigned long int)
#define USE_CORETIMER

//LCD_DATA (8-bit parallel) = RJ0..7
//LCD_RS = RH13
//LCD_RW = RH14
//LCD_EN = RH15
//LCD_RST = RK7
//LCD_PSB = RD6

#define ST7920_DATA_DIRN_IN()     (TRISJSET = 0x00FF)
#define ST7920_DATA_DIRN_OUT()    (TRISJCLR = 0x00FF)
#define ST7920_DATA_INPUT()       (PORTJ & 0x00FF)
#define ST7920_DATA_OUTPUT(d)     {LATJCLR = 0x00FF; LATJSET = d;}
#define ST7920_CTRL_DIRN_OUT()    {TRISHCLR = (1<<13)|(1<<14)|(1<<15); TRISKCLR = (1<<7); TRISDCLR = (1<<6);}
#define ST7920_RST_HIGH()         (LATKSET = (1<<7)) // RST -> K7
#define ST7920_RST_LOW()          (LATKCLR = (1<<7))
#define ST7920_E_HIGH()           (LATHSET = (1<<15)) // E -> H15
#define ST7920_E_LOW()            (LATHCLR = (1<<15))
#define ST7920_RW_HIGH()          (LATHSET = (1<<14)) // RW -> H14
#define ST7920_RW_LOW()           (LATHCLR = (1<<14))
#define ST7920_RS_HIGH()          (LATHSET = (1<<13)) // RS -> H13
#define ST7920_RS_LOW()           (LATHCLR = (1<<13))
#define ST7920_PSB_HIGH()         (LATDSET = (1<<6)) // PSB -> D6
#define ST7920_PSB_LOW()          (LATDCLR = (1<<6))

#endif /* _ST7920_CONFIG_H */
