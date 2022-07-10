# pic32glcd
Library to allow your project to work with KS0108 and ST7920 LCD controllers

## Function
The KS0108 and ST7920 LCD controllers can both run in 8-bit parallel mode. However they have different memory layouts. This library lets you support both types of controllers, without having to worry about the details. Functions included allow you to copy a picture to the buffer, manipulate it in various ways, and then write it to the screen. (No line/circle/rectangle functions are currently implemented - pull requests welcome).

## How It Works

The library will allocate its own screen buffer called `KS0108Buf` (I called it that because the memory layout is specific to the KS0108). Normally this buffer is 128x64 bits = 1KB. You can use the functions provided in `dispbuf.h` or write to this buffer directly. Then call `GLCD_WriteBufToScreen()` to write the buffer to screen. The library does not support writing parts of the screen (if you want it, please raise an issue - it wouldn't be hard).

ST7920 support allocates a second buffer and whenever `GLCD_WriteBufToScreen()` is called, the KS0108 screen buffer is copied across to the ST7920 buffer, transforming the data to the different memory layout. That buffer is then written to screen.  If you don't need ST7920 support, you can save space by commenting out `#define ST7920_SUPPORT` in `dispbuf.h`.

## Usage

### Setup

1. Copy these source files into your project.
2. Edit `KS0108-config.h` and `ST7920-config.h`. Adjust the macros to reflect the GPIO pins that the LCD is connected to.

### Drawing stuff

See [dispbuf.h](dispbuf.h) and [glcd.h](glcd.h) for function documentation. Note that all drawing functions require you to explicitly pass `KS0108Buf` as the first parameter. This is intended to allow the user use their own buffer and remove the declaration of `KS0108Buf` from `KS0108.c`.

Note that some functions are byte-aligned (i.e. drawing needs to start on row 0/8/16/.. and finish on row 7/15/23/..). This is noted in the description in [dispbuf.h](dispbuf.h).

```
GLCD_Initialize(GLCD_ST7920); // GLCD_KS0108 if you have one of those.
GLCD_ClearScreen();
GLCD_WriteScreen_buf(KS0108Buf, menu); // Write a menu image (defined in a header file, stored in flash) to the buffer
GLCD_Invert_buf(KS0108Buf, 12,12, 70,40); // Invert a rectangular section of the screen
GLCD_WriteBufToScreen();               // Write to screen
```

### Creating menus

Often these LCDs are used for displaying basic UIs like menus. A menu can quickly be created by doing the following:
1. Divide the screen into a grid - each box will be a menu item. Make sure each box has a height that is a multiple of 8. If your menu needs to scroll then all boxes must be the same size.
2. Draw a bitmap for each menu item and put it as a static const in your code using [image2cpp](http://javl.github.io/image2cpp/). Choose "Vertical - 1 bit per pixel" for the draw mode.
3. When showing your menu on screen, just write each bitmap in the appropriate position, and then invert the selected menu item before writing to the screen.

## Example

The example MPLAB X project in the firmware directory shows how to draw graphics using the library. The schematic below shows how a board can support both boards. JP3 and the potentiometer control the constrast. JP3 at GND makes the potentiometer range from GND to VCC, whereas setting it to VEE makes the pot range from VCC to VOUT (the output of the controller's onboard voltage doubler). You may need to experiment to get the right constrast setting, but I find JP3 to GND works best for ST7920 whereas VEE works best for KS0108. I prefer to use 20-pin IDC connectors for the LCD cable (with a small SIL-to-DIL adapter board soldered to the screen).

![Example schematic](firmware/pic32glcd_wiring.png)
 
## Credits

The actual drivers and drawing code is mostly other people's - I just added an abstraction layer to allow projects to support both.

KS0108 driver originally written by [Radosław Kwiecień](https://en.radzio.dxp.pl/ks0108/). Used with permission for non-commercial/educational purposes.

ST7920 driver originally written by [M.J. Bauer](https://github.com/M-J-Bauer/OLED-or-LCD-128x64-graphics-library). Used with permission.

Drawing functions and circuit by Phil Prosser.
