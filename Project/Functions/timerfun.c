#include "fundec.h"

//******************************************************************************
//
//! config used by the main timer. This timer is in charge of
//! various tasks like the scroll in the menu
//
//******************************************************************************
Timer_A_UpModeConfig confPrimary={TIMER_A_CLOCKSOURCE_SMCLK,
                           TIMER_A_CLOCKSOURCE_DIVIDER_64,
                           SECONDO/3,
                           TIMER_A_TAIE_INTERRUPT_DISABLE,
                           TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
                           TIMER_A_DO_CLEAR
};

//******************************************************************************
//
//! configuration for the secondary timer, used to turn off the led lights
//! after it's turned on when the metronome is on
//
//******************************************************************************
Timer_A_UpModeConfig confSecondary={TIMER_A_CLOCKSOURCE_SMCLK,
                           TIMER_A_CLOCKSOURCE_DIVIDER_64,
                           SECONDO*0.1,
                           TIMER_A_TAIE_INTERRUPT_ENABLE,
                           TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
                           TIMER_A_DO_CLEAR
};

//******************************************************************************
//
//! Function used to generate the PWM that powers the buzzer
//
//******************************************************************************
Timer_A_PWMConfig confPWM={TIMER_A_CLOCKSOURCE_SMCLK,
                           TIMER_A_CLOCKSOURCE_DIVIDER_64,
                           SECONDO,
                           TIMER_A_CAPTURECOMPARE_REGISTER_4,
                           TIMER_A_OUTPUTMODE_TOGGLE_SET,
                           95
};

//******************************************************************************
//
//! Handler used by the timer that generates the PWM
//! for both the led and the animation in the metronome
//
//******************************************************************************
void ledHandler(){
    if(sez==7){
        if(led[0]!=1){
            GPIO_setOutputLowOnPin(led[0],led[1]);
        }
        if(animation!=0){
            metronomeAnimationSimple(false);
        }
    }
    Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_1);
    Timer_A_clearInterruptFlag(TIMER_A2_BASE);
    Timer_A_stopTimer(TIMER_A2_BASE);

}

//******************************************************************************
//
//! Handler used by the primary timer
//
//******************************************************************************
void timerHandler(){
    (*time)();
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

//******************************************************************************
//
//! Handler used by the timer that is in charge of turning off the led
//! and reset the animation
//
//******************************************************************************
void metHand(){
    if(led[0]!=1){
        GPIO_setOutputHighOnPin(led[0],led[1]);
    }
    if(animation!=0){
        metronomeAnimationSimple(true);
    }
    Timer_A_startCounter(TIMER_A2_BASE,TIMER_A_UP_MODE);
    Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_4);
}

//******************************************************************************
//
//! This function is used to initialize the two timers:
//! -the timers are set in upmode and is based on the speed of the SMLCK clock
//! -the devider is 64 for both, because the starting frequency of the clock is high
//!  due to the setting in the main
//! -for the first, the number to count to is set as 46875/3, meaning that it will
//!  tick three times each second
//! -for the second timer the count is set so that each second it will tick ten times
//
//******************************************************************************
void setTimer(){
    //Base timer
    Timer_A_configureUpMode(TIMER_A1_BASE,&confPrimary);
    Timer_A_enableCaptureCompareInterrupt(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0);
    Timer_A_registerInterrupt(TIMER_A1_BASE,TIMER_A_CCR0_INTERRUPT ,timerHandler);

    Timer_A_configureUpMode(TIMER_A2_BASE,&confSecondary);
    Timer_A_enableCaptureCompareInterrupt(TIMER_A2_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_1);
    Timer_A_registerInterrupt(TIMER_A2_BASE,TIMER_A_CCRX_AND_OVERFLOW_INTERRUPT ,ledHandler);

    //Buzzer
    Timer_A_enableCaptureCompareInterrupt(TIMER_A0_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_4);
    Timer_A_registerInterrupt(TIMER_A0_BASE,TIMER_A_CCRX_AND_OVERFLOW_INTERRUPT ,metHand);
}

void buzzStart(){
    Timer_A_generatePWM(TIMER_A0_BASE,&confPWM);
}

