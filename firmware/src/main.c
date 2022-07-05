/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <xc.h>

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <string.h>

#include "glcd.h"
#include "dispbuf.h"

#include "definitions.h"                // SYS function prototypes

#include "menu.h"

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

// Toggle the LED every 3 times this is called
uint16_t tmr2_cnt = 0;
void tmr2_handler(uint32_t status, uintptr_t context) {
    tmr2_cnt++;
    if (tmr2_cnt>=3) { PWR_LED_Toggle(); tmr2_cnt=0; }
}

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );

    TMR2_CallbackRegister(tmr2_handler, NULL);
    TMR2_InterruptEnable();
    TMR2_Start();
    
    GLCD_Initialize(GLCD_ST7920); // GLCD_KS0108 if you have one of those.
    GLCD_ClearScreen();
    GLCD_WriteScreen_buf(KS0108Buf, menu);
    GLCD_Invert_buf(KS0108Buf, 12,12, 70,40);
    GLCD_WriteBufToScreen();

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}
