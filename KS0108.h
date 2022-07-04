#ifndef _KS0108_H /* Guard against multiple inclusion */
#define _KS0108_H

#include "KS0108-config.h"
#include "dispbuf-config.h"

#define DISPLAY_SET_Y       0x40
#define DISPLAY_SET_X       0xB8
#define DISPLAY_START_LINE  0xC0
#define DISPLAY_ON_CMD		0x3E
#define DISPLAY_ON	0x01
#define DISPLAY_OFF	0x00
#define DISPLAY_STATUS_BUSY	0x80

#define KS0108_WriteBufToScreen() KS0108_WriteBuf(KS0108Buf, 0, 0, DISPBUF_SCREEN_WIDTH, DISPBUF_SCREEN_HEIGHT)

extern unsigned char screen_x, screen_y;
extern unsigned char KS0108Buf[DISPBUF_SCREEN_WIDTH*DISPBUF_SCREEN_HEIGHT/8]; 

void KS0108_InitializePorts(void);
void KS0108_WriteCommand(unsigned char commandToWrite, unsigned char controller);
unsigned char KS0108_ReadData(void);
void KS0108_WriteData(unsigned char dataToWrite);

void KS0108_Initialize(void);
void KS0108_ClearScreen(void);
void KS0108_GoTo(unsigned char, unsigned char);
unsigned char KS0108_ReadData(void);
void KS0108_WriteBuf(unsigned char * bmp, unsigned char x, unsigned char y, unsigned char dx, unsigned char dy);

#endif // _KS0108_H