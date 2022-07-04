#ifndef _ST7920_H /* Guard against multiple inclusion */
#define _ST7920_H

#include "ST7920-config.h"
#include "dispbuf-config.h"

#ifndef NOP
#define NOP() asm volatile("NOP")
#endif

#define HI_BYTE(w)  (((w) >> 8) & 0xFF)   // Extract high-order byte from unsigned word
#define LO_BYTE(w)  ((w) & 0xFF)          // Extract low-order byte from unsigned word

#define ST7920_WriteBufToScreen() ST7920_WriteBlock(ST7920Buf, 0, 0, DISPBUF_SCREEN_WIDTH, DISPBUF_SCREEN_HEIGHT)

extern uint16_t ST7920Buf[DISPBUF_SCREEN_WIDTH*DISPBUF_SCREEN_HEIGHT/16]; 

void    ST7920_WriteCommand(uint8_t cmd);
void    ST7920_WriteData(uint8_t data);
uint8_t ST7920_ReadData();
uint8_t ST7920_ReadStatus();
void    ST7920_WaitNotBusy();
bool    ST7920_Initialize(void);
void    ST7920_ClearScreen(void);
void    ST7920_WriteBlock(uint16_t *scnBuf, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void    ST7920_Test();

#endif /* _ST7920_H */
