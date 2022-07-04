#include "dispbuf-config.h"

#include <stdbool.h>

#include "font5x8.h"
#include "font7x12.h"
#include "font10x16.h"
#include "font11x16.h"

//-------------------------------------------------------------------------------------------------
// Expects X2 >= x1 and y2 >= y1
//-------------------------------------------------------------------------------------------------
void GLCD_ClearRegion_buf(unsigned char * buf, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
unsigned char i, j, k, background;

/* trap erroneous input*/
if ((x2 >= x1) && (y2 >= y1))
    {
    /* How many rows of bytes?*/
    k = y2 - y1;
    /* start with first row of bytes on display in Y dimension */
    j = y1/8;

    /* we have either zero or up to 7 pixels to update on the first row*/
    if ((k%8) > 0)
        {
        for(i = x1; i < x2; i++)
            {
                background = *(char *)( (int)buf + j * DISPBUF_SCREEN_WIDTH + i);
                * (char *)( (int)buf + j * DISPBUF_SCREEN_WIDTH + i)= background & (0xFF-((0x01 >> k) -1));
            }
        j++;
        k = k - k%8;
        }

    while (k>=8)
        {
        for(i = x1; i < x2; i++)
            * (char *)( (int)buf + j * DISPBUF_SCREEN_WIDTH + i)=(0x00);
        j++; /* increment y rows */
        k -= 8; /* decrement k  - if I wasnt lazy these would use one variable*/
        }
    
    /* we have either zero or up to 7 pixels to update now*/
    if (k >0)
        {
        for(i = x1; i < x2; i++)
            {
                background = *(char *)( (int)buf + j * DISPBUF_SCREEN_WIDTH + i);
                * (char *)( (int)buf + j * DISPBUF_SCREEN_WIDTH + i)= background & ((0x01 >> k) -1);
            }
        }
    }
}



//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_ClearScreen_buf(unsigned char * buf)
{
unsigned char i, j;
for(j = 0; j < DISPBUF_SCREEN_HEIGHT/8; j++)
  {
  for(i = 0; i < DISPBUF_SCREEN_WIDTH; i++)
    * (char *)( (int)buf + j * DISPBUF_SCREEN_WIDTH + i)=(0x00);
  }
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteChar_8x5_buf(unsigned char * buf, unsigned char charToWrite, unsigned char x, unsigned char y)
{
int i, tc, y_rem,loop, y_mask, y_temp, work_c;
// fix offset of ASCII characters to the internal character set
charToWrite -= 32;
loop = 0;

// Work out how many unused bits are at the top of the character
y_rem = y%8; 
y_mask = 0xff>>(8-y_rem);

if((y + 8) <= DISPBUF_SCREEN_HEIGHT)
    {
    //for a 5 pixel wide char...
    for(i = 0; i < 5; i++)
    {
        if((x + 5) < DISPBUF_SCREEN_WIDTH)
        { 
            // get a copy of what is in there
            tc = *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)(y/8)) + (int)x + (int)i);
            tc = tc & y_mask;
            work_c = *(char *)((int)font5x8 + (5 * (int)charToWrite) + (int)i);
            work_c = work_c <<y_rem;
            work_c += tc;
            *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)(y/8)) + (int)x + (int)i) = work_c;
        }
    }
    if((x + 5) < DISPBUF_SCREEN_WIDTH)
    {
            // get a copy of what is in there
            tc = *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)(y/8)) + (int)x + (int)5);
            tc = tc & (y_mask);
            work_c = 0x00;
            work_c += tc;
            *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)(y/8)) + (int)x + (int)5) = work_c;
    }


    // Work out how many unused bits are at the top of the character
    y_temp = 8  - (8-y_rem);

    do
    {
        if (y_temp >= 8)
            y_mask = 0;
        else
            y_mask = 0xff<<(y_temp);

        //for a 5 pixel wide char...
        for(i = 0; i < 5; i++)
        {
            if((x + 5) < DISPBUF_SCREEN_WIDTH)
            { 
                // get a copy of what is in there
                tc = *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)((y/8) + 1 + loop)) + (int)x + (int)i);
                tc = tc & y_mask;
                work_c = (*(char *)((int)font5x8 + (5 * (int)charToWrite) + 5*loop + (int)i));
                work_c = work_c >> (8-y_rem);
                work_c = work_c & (0xff >> (8-y_rem));
                work_c += tc;
    //            if (loop == 0)
    //                work_c += (*(char *)((int)font5x8 + (5 * (int)charToWrite) + 5 + (int)i)) << y_rem;
                *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)((y/8) + 1 + loop)) + (int)x + (int)i) = work_c;
            }
        }
        if((x + 5) < DISPBUF_SCREEN_WIDTH)
        {
                // get a copy of what is in there
                tc = *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)((y/8) + 1 + loop)) + (int)x + (int)5);
                tc = tc & (y_mask);
                work_c = 0;
                work_c = work_c + tc;
                *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)((y/8) + 1 + loop)) + (int)x + (int)5) = work_c;
        }
        y_temp -= 8;
        //increment the row
        loop++;
    }while (y_temp >=0);
}
}


//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteChar_12x7_buf(unsigned char * buf, unsigned char charToWrite, unsigned char x, unsigned char y)
{
int i, tc, y_rem,loop, y_mask, y_temp, work_c;
// fix offset of ASCII characters to the internal character set
charToWrite -= 32;
loop = 0;

// Work out how many unused bits are at the top of the character
y_rem = y%8; 
y_mask = 0xff>>(8-y_rem);


if((y + 12) <= DISPBUF_SCREEN_HEIGHT)
    {

    //for a 7 pixel wide char...
    for(i = 0; i < 7; i++)
    {
        if((x + 6) < DISPBUF_SCREEN_WIDTH)
        { 
            // get a copy of what is in there
            tc = *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)(y/8)) + (int)x + (int)i);
            tc = tc & y_mask;
            work_c = *(char *)((int)font7x12 + (14 * (int)charToWrite) + (int)i);
            work_c = work_c <<y_rem;
            work_c += tc;
            *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)(y/8)) + (int)x + (int)i) = work_c;
        }
    }
    if((x + 7) < DISPBUF_SCREEN_WIDTH)
    {
            // get a copy of what is in there
            tc = *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)(y/8)) + (int)x + (int)7);
            tc = tc & (y_mask);
            work_c = 0x00;
            work_c += tc;
            *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)(y/8)) + (int)x + (int)7) = work_c;
    }

    // Work out how many unused bits are at the top of the character
    y_temp = 12  - (8-y_rem);

    do
    {
        if (y_temp >= 8)
            y_mask = 0;
        else
            y_mask = 0xff<<(y_temp);

        //for a 7 pixel wide char...
        for(i = 0; i < 7; i++)
        {
            if((x + 6) < DISPBUF_SCREEN_WIDTH)
            { 
                // get a copy of what is in there
                tc = *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)((y/8) + 1 + loop)) + (int)x + (int)i);
                tc = tc & y_mask;
                work_c = (*(char *)((int)font7x12 + (14 * (int)charToWrite) + 7*loop + (int)i));
                work_c = work_c >> (8-y_rem);
                work_c = work_c & (0xff >> (8-y_rem));
                work_c += tc;
                if (loop == 0)
                    work_c += (*(char *)((int)font7x12 + (14 * (int)charToWrite) + 7 + (int)i)) << y_rem;
                *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)((y/8) + 1 + loop)) + (int)x + (int)i) = work_c;
            }
        }
        if((x + 7) < DISPBUF_SCREEN_WIDTH)
        {
                // get a copy of what is in there
                tc = *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)((y/8) + 1 + loop)) + (int)x + (int)7);
                tc = tc & (y_mask);
                work_c = 0;
                work_c = work_c + tc;
                *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)((y/8) + 1 + loop)) + (int)x + (int)7) = work_c;
        }


        y_temp -= 8;
        //increment the row
        loop++;
    }while (y_temp >=0);
}
}


//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteChar_16x11_buf(unsigned char * buf, unsigned char charToWrite, unsigned char x, unsigned char y)
{
int i, tc, y_rem,loop, y_mask, y_temp, work_c;
// fix offset of ASCII characters to the internal character set
charToWrite -= 32;
loop = 0;

// Work out how many unused bits are at the top of the character
y_rem = y%8; 
y_mask = 0xff>>(8-y_rem);

if((y + 16) <= DISPBUF_SCREEN_HEIGHT)
    {

    //for a 7 pixel wide char...
    for(i = 0; i < 11; i++)
    {
        if((x + 11) < DISPBUF_SCREEN_WIDTH)
        { 
            // get a copy of what is in there
            tc = *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)(y/8)) + (int)x + (int)i);
            tc = tc & y_mask;
            work_c = *(char *)((int)font11x16 + (22 * (int)charToWrite) + (int)i);
            work_c = work_c <<y_rem;
            work_c += tc;
            *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)(y/8)) + (int)x + (int)i) = work_c;
        }
    }
    if((x + 11) < DISPBUF_SCREEN_WIDTH)
    {
            // get a copy of what is in there
            tc = *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)(y/8)) + (int)x + (int)11);
            tc = tc & (y_mask);
            work_c = 0x00;
            work_c += tc;
            *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)(y/8)) + (int)x + (int)11) = work_c;
    }

    // Work out how many unused bits are at the top of the character
    y_temp = 16  - (8-y_rem);

    do
    {
        if (y_temp >= 8)
            y_mask = 0;
        else
            y_mask = 0xff<<(y_temp);

        //for a 11 pixel wide char...
        for(i = 0; i < 11; i++)
        {
            if((x + 11) < DISPBUF_SCREEN_WIDTH)
            { 
                // get a copy of what is in there
                tc = *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)((y/8) + 1 + loop)) + (int)x + (int)i);
                tc = tc & y_mask;
                work_c = (*(char *)((int)font11x16 + (22 * (int)charToWrite) + 11*loop + (int)i));
                work_c = work_c >> (8-y_rem);
                work_c = work_c & (0xff >> (8-y_rem));
                work_c += tc;
                if (loop == 0)
                    work_c += (*(char *)((int)font11x16 + (22 * (int)charToWrite) + 11 + (int)i)) << y_rem;
                *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)((y/8) + 1 + loop)) + (int)x + (int)i) = work_c;
            }
        }
        if((x + 11) < DISPBUF_SCREEN_WIDTH)
        {
                // get a copy of what is in there
                tc = *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)((y/8) + 1 + loop)) + (int)x + (int)11);
                tc = tc & (y_mask);
                work_c = 0;
                work_c = work_c + tc;
                *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)((y/8) + 1 + loop)) + (int)x + (int)11) = work_c;
        }
        y_temp -= 8;
        //increment the row
        loop++;
    }while (y_temp >=0);
    }
}


//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteChar_16x10_buf(unsigned char * buf, unsigned char charToWrite, unsigned char x, unsigned char y)
{
int i, tc, y_rem,loop, y_mask, y_temp, work_c;
// fix offset of ASCII characters to the internal character set
charToWrite -= 32;
loop = 0;

// Work out how many unused bits are at the top of the character
y_rem = y%8; 
y_mask = 0xff>>(8-y_rem);

if((y + 10) <= DISPBUF_SCREEN_HEIGHT)
    {
    //for a 7 pixel wide char...
    for(i = 0; i < 10; i++)
    {
        if((x + 10) < DISPBUF_SCREEN_WIDTH)
        { 
            // get a copy of what is in there
            tc = *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)(y/8)) + (int)x + (int)i);
            tc = tc & y_mask;
            work_c = *(char *)((int)font10x16 + (20 * (int)charToWrite) + (int)i);
            work_c = work_c <<y_rem;
            work_c += tc;
            *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)(y/8)) + (int)x + (int)i) = work_c;
        }
    }
    if((x + 11) < DISPBUF_SCREEN_WIDTH)
    {
            // get a copy of what is in there
            tc = *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)(y/8)) + (int)x + (int)10);
            tc = tc & (y_mask);
            work_c = 0x00;
            work_c += tc;
            *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)(y/8)) + (int)x + (int)10) = work_c;
    }

    // Work out how many unused bits are at the top of the character
    y_temp = 16  - (8-y_rem);

    do
    {
        if (y_temp >= 8)
            y_mask = 0;
        else
            y_mask = 0xff<<(y_temp);

        //for a 11 pixel wide char...
        for(i = 0; i < 10; i++)
        {
            if((x + 10) < DISPBUF_SCREEN_WIDTH)
            { 
                // get a copy of what is in there
                tc = *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)((y/8) + 1 + loop)) + (int)x + (int)i);
                tc = tc & y_mask;
                work_c = (*(char *)((int)font10x16 + (22 * (int)charToWrite) + 10*loop + (int)i));
                work_c = work_c >> (8-y_rem);
                work_c = work_c & (0xff >> (8-y_rem));
                work_c += tc;
                if (loop == 0)
                    work_c += (*(char *)((int)font10x16 + (20 * (int)charToWrite) + 10 + (int)i)) << y_rem;
                *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)((y/8) + 1 + loop)) + (int)x + (int)i) = work_c;
            }
        }
        if((x + 10) < DISPBUF_SCREEN_WIDTH)
        {
                // get a copy of what is in there
                tc = *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)((y/8) + 1 + loop)) + (int)x + (int)10);
                tc = tc & (y_mask);
                work_c = 0;
                work_c = work_c + tc;
                *(char *)((int)buf + ((int)DISPBUF_SCREEN_WIDTH * (int)((y/8) + 1 + loop)) + (int)x + (int)10) = work_c;
        }
        y_temp -= 8;
        //increment the row
        loop++;
    }while (y_temp >=0);
}
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteString_16x11_buf(unsigned char * buf, char * stringToWrite, unsigned char x, unsigned char y)
{
char tmp;

tmp = 0;
while(*stringToWrite)
    {
        GLCD_WriteChar_16x11_buf(buf, *stringToWrite++,x + tmp,y);
        tmp += 12;
    }
}


//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteString_16x10_buf(unsigned char * buf, char * stringToWrite, unsigned char x, unsigned char y)
{
char tmp;

tmp = 0;
while(*stringToWrite)
    {
        GLCD_WriteChar_16x11_buf(buf, *stringToWrite++,x + tmp,y);
        tmp += 12;
    }
}



//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteString_12x7_buf(unsigned char * buf, char * stringToWrite, unsigned char x, unsigned char y)
{
char tmp;

tmp = 0;
while(*stringToWrite)
    {
        GLCD_WriteChar_12x7_buf(buf, *stringToWrite++,x + tmp,y);
        tmp += 8;
    }
}


//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteString_8x5_buf(unsigned char * buf, char * stringToWrite, unsigned char x, unsigned char y)
{
char tmp;

tmp = 0;
while(*stringToWrite)
    {
        GLCD_WriteChar_8x5_buf(buf, *stringToWrite++,x + tmp,y);
        tmp += 6;
    }
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_SetPixel_buf(unsigned char * buf, unsigned char x, unsigned char y)
{
unsigned char temp_buf, ydiv;

    temp_buf = *(buf + (x + ((y/8) * DISPBUF_SCREEN_WIDTH)));
    ydiv = y%8;
    temp_buf = temp_buf | 1<<ydiv;
    *(buf + (x + ((y/8) * DISPBUF_SCREEN_WIDTH))) = temp_buf;
}

//-------------------------------------------------------------------------------------------------
//
// WriteScreen_Buf(*buf, *bmp) - write a full screen image
// *buf is the buffer - target address
// *bmp is the bitmap to be copied
//-------------------------------------------------------------------------------------------------
void GLCD_WriteScreen_buf(unsigned char * buf, const unsigned char * bmp)
{
unsigned char i, j;

for(j = 0; j < DISPBUF_SCREEN_HEIGHT / 8; j++)
  {
     for(i = 0; i < DISPBUF_SCREEN_WIDTH; i++) 
         *(buf + (i + j*DISPBUF_SCREEN_WIDTH)) = *(bmp++);
  }
}

//-------------------------------------------------------------------------------------------------
//
// WriteBMP (*buf, *bmp, x, y, dx, dy)
// *buf is the buffer - target address
// *bmp is the bitmap to be copied
// x - start x address
// y - start y address - Must be on an 8 pixel boundary
// dx - x range
// dy - y range - must be a multiple of 8 bits in size
//-------------------------------------------------------------------------------------------------
void GLCD_WriteBMP_buf(unsigned char * buf, const unsigned char * bmp,unsigned char x, unsigned char y, unsigned char dx, unsigned char dy)
{
unsigned char i, j, Temp_Byte;

for(j = 0; j < (dy/8); j++)
  {
     for(i = 0; i < (dx); i++) 
        {
        Temp_Byte = *(bmp + (i + (j * dx)));
        *(buf + (x + i + (j + y/8)*DISPBUF_SCREEN_WIDTH)) = Temp_Byte;
        }
  }
    
}

//-------------------------------------------------------------------------------------------------
//
// GLCD_Invert_buf(*buf, x, y, dx, dy)
// Invert part of a buffer
// *buf is the buffer - target address
// x - start x address
// y - start y address
// dx - x range
// dy - y range
//-------------------------------------------------------------------------------------------------
void GLCD_Invert_buf(unsigned char *buf, unsigned char x, unsigned char y, unsigned char dx, unsigned char dy) {
    unsigned char _y8; 
    unsigned char _x;
    // Iterate through 8-row blocks
    for (_y8=y/8;_y8<DISPBUF_SCREEN_HEIGHT/8 && _y8 < ((y+dy)/8);_y8++) {
        int a = y-(_y8*8); // How many pixels from start row to start of this block
        int b=(y+dy)-(_y8*8); // How many pixels from end row to start of this block
        unsigned char mask = 0xFF;
        if (a>=0 && a<8) mask &= (0xFF << a);
        if (b>=0 && b<8) mask &= (0xFF >> b);
        for (_x=x;_x<(x+dx) && _x<DISPBUF_SCREEN_WIDTH; _x++) {
            int pix_offset = _y8*DISPBUF_SCREEN_WIDTH + _x;
            buf[pix_offset] ^= mask;
        }
    }
}

//-------------------------------------------------------------------------------------------------
//
// GLCD_CopySection_buf(*dst, dstx, dsty, *src, srcx, srcy, dx, dy)
// *dst is the buffer - target address
// dstx - dest x address
// dsty - dest y address - Must be on an 8 pixel boundary
// *src is the source buffer
// srcx - src x address
// srcy - src y address - Must be on an 8 pixel boundary
// dx - x range
// dy - y range - must be a multiple of 8 bits in size
// invert - invert pixels
//-------------------------------------------------------------------------------------------------
void GLCD_CopySection_buf(unsigned char *dst, unsigned char dstx, unsigned char dsty,
        const unsigned char *src, unsigned char srcx, unsigned char srcy,
        unsigned char dx, unsigned char dy, bool invert) {
    unsigned i, j;
    for(j = 0; j < (dy/8); j++) {
        for(i = 0; i < dx; i++) {
            if (invert) {
                dst[dstx + i + (j + dsty/8)*DISPBUF_SCREEN_WIDTH] = ~(src[srcx+i+(j+srcy/8)*DISPBUF_SCREEN_WIDTH]);
            }
            else {
                dst[dstx + i + (j + dsty/8)*DISPBUF_SCREEN_WIDTH] = src[srcx+i+(j+srcy/8)*DISPBUF_SCREEN_WIDTH];
            }
        }
    }
}
