#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include <stdio.h>

#define SECONDO 46875

extern void (*time)(void);
extern void setTimer();
extern void buzzStart();

extern uint16_t bpm;
extern void micWait();

extern uint16_t sez;
extern uint16_t animation;
extern int16_t animationSpeed;
extern int32_t bgColor;
extern bool updownStop;
extern void setDisplay();
extern void draw();
extern void sectionForward();
extern void sectionBack();
extern void metronomeAnimationSimple(bool t);
extern void metronomeAnimatioComplex();
extern void screenColor(int32_t color, int i);
extern void animationSetup();

extern void (*up)(void);
extern void (*down)(void);
extern uint16_t led[2];
extern void setGpio();
extern bool scrollEnabled;
extern bool timerB;
extern void micStartMetronome();
extern void setADC();

extern void valueDraw(int a);
