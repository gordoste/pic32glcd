// Functions to write graphics into a buffer allocated by calling program.
// Set the buffer width and height in dispbuf-config.h
//
// pixel ordering is KS0108-style and converstion is needed for other pixel ordering (e.g. ST7920)

#ifndef _DISPBUF_H    /* Guard against multiple inclusion */
#define _DISPBUF_H

#include "dispbuf-config.h"

#include <stdbool.h>

void GLCD_ClearRegion_buf(unsigned char * buf, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
void GLCD_ClearScreen_buf(unsigned char * buf);
void GLCD_WriteChar_8x5_buf(unsigned char * buf, unsigned char charToWrite, unsigned char x, unsigned char y);
void GLCD_WriteChar_12x7_buf(unsigned char * buf, unsigned char charToWrite, unsigned char x, unsigned char y);
void GLCD_WriteChar_16x11_buf(unsigned char * buf, unsigned char charToWrite, unsigned char x, unsigned char y);
void GLCD_WriteChar_16x10_buf(unsigned char * buf, unsigned char charToWrite, unsigned char x, unsigned char y);
void GLCD_WriteString_16x11_buf(unsigned char * buf, char * stringToWrite, unsigned char x, unsigned char y);
void GLCD_WriteString_16x10_buf(unsigned char * buf, char * stringToWrite, unsigned char x, unsigned char y);
void GLCD_WriteString_12x7_buf(unsigned char * buf, char * stringToWrite, unsigned char x, unsigned char y);
void GLCD_WriteString_8x5_buf(unsigned char * buf, char * stringToWrite, unsigned char x, unsigned char y);
void GLCD_SetPixel_buf(unsigned char * buf, unsigned char x, unsigned char y);
void GLCD_WriteScreen_buf(unsigned char * buf, const unsigned char * bmp);
void GLCD_WriteBMP_buf(unsigned char * buf, const unsigned char * bmp,unsigned char x, unsigned char y, unsigned char dx, unsigned char dy);
void GLCD_Invert_buf(unsigned char *buf, unsigned char x, unsigned char y, unsigned char dx, unsigned char dy);
void GLCD_CopySection_buf(unsigned char *dst, unsigned char dstx, unsigned char dsty,
        const unsigned char *src, unsigned char srcx, unsigned char srcy, unsigned char dx, unsigned char dy, bool invert);

#endif /* _DISPBUF_H */

