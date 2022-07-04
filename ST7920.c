#include <xc.h>
#include <stdbool.h>
#include <string.h>

#include "ST7920.h"
#include "dispbuf-config.h"

#ifdef USE_CORETIMER
#include "peripheral/coretimer/plib_coretimer.h"
#define DelayUs(x) CORETIMER_DelayUs(x)
#define DelayMs(x) CORETIMER_DelayMs(x)
#else
#include "delay.h"
#endif

uint16_t ST7920Buf[DISPBUF_SCREEN_WIDTH*DISPBUF_SCREEN_HEIGHT/16]; 

/*
 * Low-level driver functions for monochrome Graphic LCD module, 128 x 64 pixels.
 * 
 * Original code by M. J. Bauer https://github.com/M-J-Bauer/OLED-or-LCD-128x64-graphics-library
 * Modifications by Stephen Gordon
 *
 */

/*----------------------------------------------------------------------------------
 * Name               :  ST7920_WriteCommand()
 * Function           :  Write 8-bit command to LCD controller
 *                       Delay 100us (minimum) before exit.
 * Entry arg(s)       :  cmd
 * Return             :  --
------------------------------------------------------------------------------------*/
void ST7920_WriteCommand(uint8_t cmd)
{
    ST7920_DATA_DIRN_OUT();
    ST7920_RS_LOW();
    ST7920_RW_LOW();
    NOP();
    ST7920_E_HIGH();
    ST7920_DATA_OUTPUT(cmd);
    DelayUs(1);         // E pulse width >= 320ns
    ST7920_E_LOW();
    NOP();
    NOP();
    ST7920_RW_HIGH();
    DelayUs(100);           // Delay after command write (80us min.)
}


/*----------------------------------------------------------------------------------
 * Name               :  ST7920()
 * Function           :  Write 8-bit data to LCD controller
 * Entry arg(s)       :  data
 * Return             :  --
------------------------------------------------------------------------------------*/
void ST7920_WriteData(uint8_t data)
{
    ST7920_DATA_DIRN_OUT();
    ST7920_RS_HIGH();
    ST7920_RW_LOW();
    NOP();
    ST7920_E_HIGH();
    ST7920_DATA_OUTPUT(data);
    DelayUs(1);           // E pulse width >= 320ns
    ST7920_E_LOW();
    NOP();
    NOP();
    ST7920_RW_HIGH();
    DelayUs(100);             // Delay after write data (80us min.)
}


/*----------------------------------------------------------------------------------
 * Name               :  ST7920_ReadData()
 * Function           :  Read 8-bit data from LCD controller
 * Entry arg(s)       :  --
 * Return             :  data byte
------------------------------------------------------------------------------------*/
uint8_t ST7920_ReadData()
{
    uint8_t data;

    ST7920_DATA_DIRN_IN();
    ST7920_RS_HIGH();
    ST7920_RW_HIGH();
    NOP();
    ST7920_E_HIGH();
    DelayUs(1);           // E pulse width >= 320ns
    data = ST7920_DATA_INPUT();
    ST7920_E_LOW();
    DelayUs(100);             // Delay after read data (80us min.)

    return data;
}


/*----------------------------------------------------------------------------------
 * Name               :  ST7920_ReadStatus()
 * Function           :  Read 8-bit status reg from LCD controller
 * Entry arg(s)       :  --
 * Return             :  status byte
------------------------------------------------------------------------------------*/
uint8_t ST7920_ReadStatus()
{
    uint8_t status;

    ST7920_DATA_DIRN_IN();
    ST7920_RS_LOW();
    ST7920_RW_HIGH();
    NOP();
    ST7920_E_HIGH();
    DelayUs(1);           // E pulse width >= 320ns
    status = ST7920_DATA_INPUT();
    ST7920_E_LOW();
    DelayUs(2);           // E cycle time >= 1800ns

    return status;
}


/*----------------------------------------------------------------------------------
 * Name               :  ST7920_WaitNotBusy()
 * Function           :  Wait until LCD controller is not busy.
 * Entry arg(s)       :  --
 * Return             :  --
------------------------------------------------------------------------------------*/
void  ST7920_WaitNotBusy()
{
    while (ST7920_ReadStatus() & 0x80)  {/* WAIT */}
}


/*----------------------------------------------------------------------------------
 * Name               :  ST7920_Initialize()
 * Function           :  Initialize ST7920 LCD controller in graphics mode.
 * Input              :  none.
 * Return             :  False (0) if ST7920 not detected, else True (1)
------------------------------------------------------------------------------------*/
bool  ST7920_Initialize(void)
{
    bool    result = 1;
    uint8_t dummy;

    ST7920_CTRL_DIRN_OUT();
    NOP();
    ST7920_PSB_HIGH();            // Parallel mode
    ST7920_E_LOW();
    ST7920_RS_HIGH();
    ST7920_RW_HIGH();

    ST7920_RST_LOW();             // LCD controller hard reset
    DelayMs(10);
    ST7920_RST_HIGH();
    DelayMs(50);

    ST7920_WriteCommand(0x30);    // Function Set: basic
    ST7920_WriteCommand(0x30);    // Function Set: basic commands
    ST7920_WriteCommand(0x0C);    // LCD on/off control: display ON, no blink
    ST7920_WriteCommand(0x30);    // Function Set: basic
    ST7920_WriteCommand(0x34);    // Function Set: extended commands
    ST7920_WriteCommand(0x36);    // Function Set: select graphic mode
    DelayMs(10);
    
    // Write test data to GDRAM to detect if LCD module is connected...
    ST7920_WriteCommand(0x80);    // set vert addr = 0
    ST7920_WriteCommand(0x80);    // set horiz addr = 0
    ST7920_WriteData(0x69);       // write GDRAM data MSB
    ST7920_WriteData(0xC3);       // write GDRAM data LSB
    
    ST7920_WriteCommand(0x80);    // set vert addr = 0
    ST7920_WriteCommand(0x80);    // set horiz addr = 0
    dummy = ST7920_ReadData();
    if (ST7920_ReadData() != 0x69) result = 0;  // read back MSB
    if (ST7920_ReadData() != 0xC3) result = 0;  // read back LSB

    return result;
}


/*----------------------------------------------------------------------------------
 * Name               :  ST7920_ClearScreen()
 *
 * Function           :  Clear entire LCD graphics data RAM.
 *              
------------------------------------------------------------------------------------*/
void   ST7920_ClearScreen(void)
{
    uint8_t x, y;

    for (y = 0;  y < 64;  y++)
    {
        ST7920_WriteCommand(0x80 | y);
        ST7920_WriteCommand(0x80);
        for (x = 0;  x < 16;  x++)
        {
            ST7920_WriteData(0x00);
            ST7920_WriteData(0x00);
        }
    }
}


/*----------------------------------------------------------------------------------
 * Function   :  ST7920_WriteBlock()
 *
 * Overview   :  Copies a rectangular block of pixels from an external screen
 *               buffer in MCU RAM to the LCD controller GDRAM (graphics data RAM).
 *               In general, the GDRAM pixel format differs from the MCU RAM screen
 *               buffer, so a transformation of some sort needs to be performed.
 *
 * Input      :  scnBuf = address of screen buffer in MCU RAM
 *               x, y = pixel coords of upper LHS of block to be copied
 *               w, h = width and height (pixels) of block to be copied
 *  
 * Return     :  --
------------------------------------------------------------------------------------*/
void   ST7920_WriteBlock(uint16_t *scnBuf, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    uint8_t vertAddr, horizAddr;    // row and column address to be sent to ST7920
    int     row, col;               // row and column word index for screen buffer
    int     firstCol;               // first (leftmost) column-word in a row to be modified
    int     lastCol;                // last (rightmost) column-word in a row to be modified
    uint16_t  pixelWord;              // row of 16 pixels in column-word to be modified
    uint16_t  *pBuf;

    if (x > 127)  x = 0;            // prevent writing past end-of-row
    if ((x + w) > 128) w = 128 - x;

    firstCol = x / 16;   
    lastCol = (x + w - 1) / 16;

    for (row = y;  row < (y + h) && row < 64;  row++)
    {
        for (col = firstCol;  col <= lastCol && col < 8;  col++)
        {
            pBuf = scnBuf + (row * 8 + col);
            pixelWord = *pBuf;

            if (row < 32)  { vertAddr = row;  horizAddr = col; }  // Top half of screen
            else  { vertAddr = row - 32;  horizAddr = col + 8; }  // Bottom half

            ST7920_WriteCommand(0x80 | vertAddr);
            ST7920_WriteCommand(0x80 | horizAddr);
            ST7920_WriteData(HI_BYTE(pixelWord));
            ST7920_WriteData(LO_BYTE(pixelWord));
        }
    }
}


/*----------------------------------------------------------------------------------
 * Name               :  ST7920_Test()
 * Function           :  Low-level test of ST7920 graphics write sequence.
 *                       The screen should show a pattern of diagonal lines,
 *                       slope 45 degrees, thickness 2 pixels, spaced 16 pixels apart.
 * Input              :  --
------------------------------------------------------------------------------------*/
void  ST7920_Test()
{
    uint8_t vertAddr, horizAddr;    // row and column address to be sent to ST7920
    int     row, col;               // row and column word index for screen buffer
    uint16_t  pixelWord = 0x0000;

    for (row = 0;  row < 64;  row++)
    {
        if ((row % 16) == 0) pixelWord = 0xC000;

        for (col = 0;  col < 8;  col++)
        {
            if (row < 32)  { vertAddr = row;  horizAddr = col; }  // Top half of screen
            else  { vertAddr = row - 32;  horizAddr = col + 8; }  // Bottom half

            ST7920_WriteCommand(0x80 | vertAddr);
            ST7920_WriteCommand(0x80 | horizAddr);
            ST7920_WriteData(HI_BYTE(pixelWord));
            ST7920_WriteData(LO_BYTE(pixelWord));
        }
        
        pixelWord = pixelWord >> 1;
        if (pixelWord == 0x0001) pixelWord = 0x8001;
    }
}

