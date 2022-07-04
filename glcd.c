#include "glcd.h"

#include <string.h>

screen_type_t currentScreen = GLCD_NONE;

void GLCD_Initialize(screen_type_t screenType) {
    switch (screenType) {
        case GLCD_KS0108:
            KS0108_Initialize();
            break;
#ifdef ST7920_SUPPORT
        case GLCD_ST7920:
            ST7920_Initialize();
            break;
#endif
        default:
            break;
    }
    currentScreen = screenType;
}

void GLCD_ClearScreen() {
    switch (currentScreen) {
        case GLCD_KS0108:
            KS0108_ClearScreen();
            break;
#ifdef ST7920_SUPPORT
        case GLCD_ST7920:
            ST7920_ClearScreen();
            break;
#endif
        default:
            break;
    }
}

void GLCD_WriteBufToScreen() {
    switch (currentScreen) {
        case GLCD_KS0108:
            KS0108_WriteBufToScreen();
            break;
#ifdef ST7920_SUPPORT
        case GLCD_ST7920:
            BufCopy_KS0108_to_ST7920(ST7920Buf, KS0108Buf);
            ST7920_WriteBufToScreen();
            break;
#endif
        default:
            break;
    }
}

#ifdef ST7920_SUPPORT
// Copy a screen buffer with vertical byte order (KS0108-style) into one with horizontal word order (ST7920-style)
// Both buffers are DISPBUF_SCREEN_WIDTH*DISPBUF_SCREEN_HEIGHT/8 bytes)
//
// Processes in 16x8 blocks - One block is shown
/*
 *          KS0108                                  ST7920
 * ---------------------------------|--------------------------------|
 * | a0 b0 c0 d0 e0  .. p0          | a15 a14 .. a0                  |
 * | a1 b1 c1 d1 e1  .. p1          | b15 b14 .. b0                  | 
 * | ..                             | ..                             |
 * | a7 b7 c7 d7 e7  .. p7          | h15 h14 .. h0                  |
 * ------------------------------------------------------------------|
 *
 * Although, in the ST7920 example, words a and b are not contiguous (words for an entire row are contiguous)
 * 
 * Copying 128x64 screen takes ~400ns @ 192MHz :-)
 */
void BufCopy_KS0108_to_ST7920(uint16_t *dst_st7920, unsigned char *src_ks0108) {
    unsigned char tmp[16];
    int i,j,k,l;
    unsigned int src_offset, dst_offset;
    uint16_t x;
    unsigned char mask;
    // Iterate through groups of 8 rows
    for (i=0; i<DISPBUF_SCREEN_HEIGHT/8; i++) {
        // Iterate through 16x8 blocks
        for (j=0; j<DISPBUF_SCREEN_WIDTH/16; j++) {
            src_offset=(i*DISPBUF_SCREEN_WIDTH)+(j*16);
            memcpy(tmp, &(src_ks0108[src_offset]), 16); // Copy 16x8 area to buffer
            for (k=0; k<8; k++) { // Iterate through the 8 rows
                x=0;
                dst_offset=((i*8+k)*(DISPBUF_SCREEN_WIDTH/16))+j;
                mask=1<<k;
                for (l=0; l<15; l++) {
                  x+=(tmp[l]&mask)?1:0;
                  x = x<<1;
                }
                x+= (tmp[15]&mask)?1:0;
                dst_st7920[dst_offset]=x;
            }
        }
    }
}
#endif