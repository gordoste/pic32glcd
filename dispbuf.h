// Functions to write graphics into a buffer allocated by calling program.
// Set the buffer width and height in dispbuf-config.h
//
// pixel ordering is KS0108-style and converstion is needed for other pixel ordering (e.g. ST7920)

#ifndef _DISPBUF_H    /* Guard against multiple inclusion */
#define _DISPBUF_H

#include "dispbuf-config.h"

#include <stdbool.h>


//-------------------------------------------------------------------------------------------------
// GLCD_ClearRegion_buf
// Clear a rectangular region of the buffer (buf) with top-left corner at (x1,y1) and bottom-right
// corner at (x2,y2)
// Expects x2 >= x1 and y2 >= y1
//-------------------------------------------------------------------------------------------------
void GLCD_ClearRegion_buf(unsigned char * buf, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);

//-------------------------------------------------------------------------------------------------
// GLCD_ClearScreen_buf
// Clear entire buffer (buf)
//-------------------------------------------------------------------------------------------------
void GLCD_ClearScreen_buf(unsigned char * buf);

//-------------------------------------------------------------------------------------------------
// GLCD_WriteChar_8x5_buf
// Write a 8x5 character (charToWrite) to the buffer (buf) at a specified position (x,y)
//-------------------------------------------------------------------------------------------------
void GLCD_WriteChar_8x5_buf(unsigned char * buf, unsigned char charToWrite, unsigned char x, unsigned char y);

//-------------------------------------------------------------------------------------------------
// GLCD_WriteChar_12x7_buf
// Write a 12x7 character (charToWrite) to the buffer (buf) at a specified position (x,y)
//-------------------------------------------------------------------------------------------------
void GLCD_WriteChar_12x7_buf(unsigned char * buf, unsigned char charToWrite, unsigned char x, unsigned char y);

//-------------------------------------------------------------------------------------------------
// GLCD_WriteChar_16x11_buf
// Write a 16x11 character (charToWrite) to the buffer (buf) at a specified position (x,y)
//-------------------------------------------------------------------------------------------------
void GLCD_WriteChar_16x11_buf(unsigned char * buf, unsigned char charToWrite, unsigned char x, unsigned char y);

//-------------------------------------------------------------------------------------------------
// GLCD_WriteChar_16x10_buf
// Write a 16x10 character (charToWrite) to the buffer (buf) at a specified position (x,y)
//-------------------------------------------------------------------------------------------------
void GLCD_WriteChar_16x10_buf(unsigned char * buf, unsigned char charToWrite, unsigned char x, unsigned char y);

//-------------------------------------------------------------------------------------------------
// GLCD_WriteString_16x11_buf
// Write a 16x11 string (stringToWrite) to the buffer (buf) at a specified position (x,y)
//-------------------------------------------------------------------------------------------------
void GLCD_WriteString_16x11_buf(unsigned char * buf, char * stringToWrite, unsigned char x, unsigned char y);

//-------------------------------------------------------------------------------------------------
// GLCD_WriteString_16x10_buf
// Write a 16x10 string (stringToWrite) to the buffer (buf) at a specified position (x,y)
//-------------------------------------------------------------------------------------------------
void GLCD_WriteString_16x10_buf(unsigned char * buf, char * stringToWrite, unsigned char x, unsigned char y);

//-------------------------------------------------------------------------------------------------
// GLCD_WriteString_12x7_buf
// Write a 12x7 string (stringToWrite) to the buffer (buf) at a specified position (x,y)
//-------------------------------------------------------------------------------------------------
void GLCD_WriteString_12x7_buf(unsigned char * buf, char * stringToWrite, unsigned char x, unsigned char y);

//-------------------------------------------------------------------------------------------------
// GLCD_WriteString_8x5_buf
// Write a 8x5 string (stringToWrite) to the buffer (buf) at a specified position (x,y)
//-------------------------------------------------------------------------------------------------
void GLCD_WriteString_8x5_buf(unsigned char * buf, char * stringToWrite, unsigned char x, unsigned char y);

//-------------------------------------------------------------------------------------------------
// GLCD_SetPixel_buf
// Set the pixel at a specified position (x,y) in the buffer (buf)
//-------------------------------------------------------------------------------------------------
void GLCD_SetPixel_buf(unsigned char * buf, unsigned char x, unsigned char y);

//-------------------------------------------------------------------------------------------------
// GLCD_WriteScreen_buf
// Copy a complete screen image (bmp) into the buffer (buf)
// Expects bmp to be the same size as buf
//-------------------------------------------------------------------------------------------------
void GLCD_WriteScreen_buf(unsigned char * buf, const unsigned char * bmp);

//-------------------------------------------------------------------------------------------------
// GLCD_WriteBMP_buf
// Copy a bitmap (bmp) with specified size (dx,dy) into the buffer (buf) at a specified position (x,y)
// y and dy must be divisible by 8
//-------------------------------------------------------------------------------------------------
void GLCD_WriteBMP_buf(unsigned char * buf, const unsigned char * bmp,unsigned char x, unsigned char y, unsigned char dx, unsigned char dy);

//-------------------------------------------------------------------------------------------------
// GLCD_Invert_buf
// Invert a rectangular section in the buffer (buf) at specified location (x,y) with specified size (dx,dy)
// y and dy do NOT need to be divisible by 8
//-------------------------------------------------------------------------------------------------
void GLCD_Invert_buf(unsigned char *buf, unsigned char x, unsigned char y, unsigned char dx, unsigned char dy);

//-------------------------------------------------------------------------------------------------
// GLCD_CopySection_buf
// Copy a rectangular section of an image (src) at a specified position (srcx,srcy) with specified size (dx,dy)
// to a destination buffer (dst) at a specified target position (dstx, dsty). Optionally, invert bits copied (invert)
// dsty, srcy and dy must all be divisible by 8
//-------------------------------------------------------------------------------------------------
void GLCD_CopySection_buf(unsigned char *dst, unsigned char dstx, unsigned char dsty,
        const unsigned char *src, unsigned char srcx, unsigned char srcy, unsigned char dx, unsigned char dy, bool invert);

#endif /* _DISPBUF_H */

