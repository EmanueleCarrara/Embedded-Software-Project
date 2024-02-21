#include "Functions/fundec.h"

//******************************************************************************
//
//! Pointer to the function associated with the up position of the joystick
//
//******************************************************************************
void (*up)(void);

//******************************************************************************
//
//! Pointer to the function associated with the down position of the joystick
//
//******************************************************************************
void (*down)(void);

//******************************************************************************
//
//! Pointer to the function that the timer need to call when it reach
//! the value in the compare register
//
//******************************************************************************
void (*time)(void);

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    // Halting WDT and disabling master interrupts
    WDT_A_holdTimer();
    Interrupt_disableMaster();
    // Set the core voltage level to VCORE1
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    // Set 2 flash wait states for Flash bank 0 and 1
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);
    // Initializes Clock System
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_8);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    setDisplay();
    setGpio();
    setTimer();
    setADC();
    draw();
     while(1){
         PCM_gotoLPM0();
     }
}
