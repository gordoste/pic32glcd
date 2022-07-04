#ifndef _GLCD_H /* Guard against multiple inclusion */
#define _GLCD_H

// Comment out and delete the ST7920* files from your project if you don't need it and want to save space.
#define ST7920_SUPPORT      

#include <stdbool.h>

#include "KS0108.h"

#ifdef ST7920_SUPPORT
#include "ST7920.h"
#endif

typedef enum {
    GLCD_KS0108 = 0,
#ifdef ST7920_SUPPORT
    GLCD_ST7920,
#endif
    GLCD_NONE
} screen_type_t;

void GLCD_Initialize(screen_type_t);
void GLCD_ClearScreen();
void GLCD_WriteBufToScreen();

#ifdef ST7920_SUPPORT
void BufCopy_KS0108_to_ST7920(uint16_t *dst_st7920, unsigned char *src_ks0108);
#endif

#endif /* _GLCD_H */
