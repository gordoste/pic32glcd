//-------------------------------------------------------------------------------------------------
// Universal KS0108 driver library
// (c) Rados³aw Kwiecieñ, radek@dxp.pl
// Modified by Stephen Gordon
//-------------------------------------------------------------------------------------------------
#include <xc.h>
#include "KS0108.h"
#include "dispbuf-config.h"

#ifdef USE_CORETIMER
#include "peripheral/coretimer/plib_coretimer.h"
#define DelayUs(x) CORETIMER_DelayUs(x)
#define DelayMs(x) CORETIMER_DelayMs(x)
#else
#include "delay.h"
#endif


//-------------------------------------------------------------------------------------------------
unsigned char screen_x, screen_y;

unsigned char KS0108Buf[DISPBUF_SCREEN_WIDTH*DISPBUF_SCREEN_HEIGHT / 8];

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------

void KS0108_Initialize(void) {
    unsigned char i;
    KS0108_InitializePorts();
    for (i = 0; i < 2; i++)
        KS0108_WriteCommand((DISPLAY_ON_CMD | DISPLAY_ON), i);
}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------

void KS0108_GoTo(unsigned char x, unsigned char y) {
    unsigned char i;
    screen_x = x;
    screen_y = y;
    for (i = 0; i < DISPBUF_SCREEN_WIDTH / 64; i++) {
        KS0108_WriteCommand(DISPLAY_SET_Y | 0, i);
        KS0108_WriteCommand(DISPLAY_SET_X | y, i);
        KS0108_WriteCommand(DISPLAY_START_LINE | 0, i);
    }
    KS0108_WriteCommand(DISPLAY_SET_Y | (x % 64), (x / 64));
    KS0108_WriteCommand(DISPLAY_SET_X | y, (x / 64));
}


//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------

void KS0108_ClearScreen(void) {
    unsigned char i, j;
    for (j = 0; j < DISPBUF_SCREEN_HEIGHT / 8; j++) {
        KS0108_GoTo(0, j);
        for (i = 0; i < DISPBUF_SCREEN_WIDTH; i++)
            KS0108_WriteData(0x00);
    }
}

inline void KS0108_Delay_EH(void) {
    asm("nop");
    DelayUs(KS0108_EN_H);
}

inline void KS0108_Delay_EL(void) {
    asm("nop");
    DelayUs(KS0108_EN_L);
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------

void KS0108_InitializePorts(void) {
    KS0108_CTRL_DIRN_OUT();
    KS0108_CS1_HIGH();
    KS0108_CS2_HIGH();
    KS0108_RST_HIGH();
}
//-------------------------------------------------------------------------------------------------
// Enable Controller (0-2)
//-------------------------------------------------------------------------------------------------

void KS0108_EnableController(unsigned char controller) {
    KS0108_Delay_EH();
    switch (controller) {
#ifdef KS0108_SWAP_SIDES
        case 0: KS0108_CS1_LOW();
            break;
        case 1: KS0108_CS2_LOW();
            break;
#else
        case 0: KS0108_CS2_LOW();
            break;
        case 1: KS0108_CS1_LOW();
            break;
#endif
    }
    KS0108_Delay_EH();
}
//-------------------------------------------------------------------------------------------------
// Disable Controller (0-2)
//-------------------------------------------------------------------------------------------------

void KS0108_DisableController(unsigned char controller) {
    KS0108_Delay_EL();
    switch (controller) {
#ifdef KS0108_SWAP_SIDES
        case 0: KS0108_CS1_HIGH();
            break;
        case 1: KS0108_CS2_HIGH();
            break;
#else
        case 0: KS0108_CS2_HIGH();
            break;
        case 1: KS0108_CS1_HIGH();
            break;
#endif
    }
    KS0108_Delay_EL();
}
//-------------------------------------------------------------------------------------------------
// Read Status from specified controller (0-2)
//-------------------------------------------------------------------------------------------------

unsigned char KS0108_ReadStatus(unsigned char controller) {
    unsigned char status;
    KS0108_DATA_DIRN_IN();
    KS0108_RW_HIGH();
    KS0108_RS_LOW();
    KS0108_EnableController(controller);
    KS0108_E_HIGH();
    DelayUs(KS0108_EN_H);
    status = KS0108_DATA_INPUT();
    KS0108_E_LOW();
    KS0108_DisableController(controller);
    return status;
}
//-------------------------------------------------------------------------------------------------
// Write command to specified controller
//-------------------------------------------------------------------------------------------------

void KS0108_WriteCommand(unsigned char commandToWrite, unsigned char controller) {
    while (KS0108_ReadStatus(controller) & DISPLAY_STATUS_BUSY);
    KS0108_DATA_DIRN_OUT();
    KS0108_RW_LOW();
    KS0108_RS_LOW();
    KS0108_EnableController(controller);
    KS0108_DATA_OUTPUT(commandToWrite);
    KS0108_E_HIGH();
    KS0108_Delay_EL();
    KS0108_E_LOW();
    KS0108_DisableController(controller);
}
//-------------------------------------------------------------------------------------------------
// Read data from current position
//-------------------------------------------------------------------------------------------------

unsigned char KS0108_ReadData(void) {
    unsigned char data;
    while (KS0108_ReadStatus(screen_x / 64) & DISPLAY_STATUS_BUSY);
    KS0108_DATA_DIRN_IN();
    KS0108_RW_HIGH();
    KS0108_RS_HIGH();
    KS0108_EnableController(screen_x / 64);
    KS0108_E_HIGH();
    data = KS0108_DATA_INPUT();
    KS0108_E_LOW();
    KS0108_DisableController(screen_x / 64);
    screen_x++;
    return data;
}
//-------------------------------------------------------------------------------------------------
// Write data to current position
//-------------------------------------------------------------------------------------------------

void KS0108_WriteData(unsigned char dataToWrite) {
    while (KS0108_ReadStatus(screen_x / 64) & DISPLAY_STATUS_BUSY);
    KS0108_DATA_DIRN_OUT();
    KS0108_RW_LOW();
    KS0108_RS_HIGH();
    KS0108_DATA_OUTPUT(dataToWrite);
    KS0108_EnableController(screen_x / 64);
    KS0108_E_HIGH();
    KS0108_Delay_EL();
    KS0108_E_LOW();
    KS0108_DisableController(screen_x / 64);
    screen_x++;
    KS0108_Delay_EL();
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------

void KS0108_WriteBuf(unsigned char * bmp, unsigned char x, unsigned char y, unsigned char dx, unsigned char dy) {
    unsigned char i, j;
    for (j = 0; j < dy / 8; j++) {
        KS0108_GoTo(x, y + j);
        for (i = 0; i < dx; i++)
            KS0108_WriteData(*bmp++);
    }
}
