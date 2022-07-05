# pic32glcd
Library to allow your project to work with KS0108 and ST7920 LCD controllers

## Function
The KS0108 and ST7920 LCD controllers can both run in 8-bit parallel mode. However they have different memory layouts. This library lets you support both types of controllers, without having to worry about the details. Functions included allow you to copy a picture to the buffer, manipulate it in various ways, and then write it to the screen. (No line/circle/rectangle functions are currently implemented - pull requests welcome).

## Example

The example MPLAB X project in the firmware directory shows how to draw graphics using the library. The schematic below shows how a board can support both boards. JP3 should be connected to GND for ST7920, and should be connected to VOUT (output of the voltage doubler) for KS0108. Personally, I prefer to use 20-pin IDC connectors for the LCD cable (with a small SIL-to-DIL adapter board soldered to the screen).

![Example schematic](firmware/pic32glcd_wiring.png)
 

