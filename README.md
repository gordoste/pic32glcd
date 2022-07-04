# pic32glcd
Library to allow your project to work with KS0108 and ST7920 LCD controllers

## Function
The KS0108 and ST7920 LCD controllers can both run in 8-bit parallel mode. However they have different memory layouts. This library lets you support both types of controllers, without having to worry about the details. Functions included allow you to copy a picture to the buffer, manipulate it in various ways, and then write it to the screen. (No line/circle/rectangle functions are currently implemented - pull requests welcome).

An example is also included showing the wiring and code to support both types of screens in the same firmware.
