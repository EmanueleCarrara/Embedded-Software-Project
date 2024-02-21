#include "fundec.h"
#include <ti/grlib/string.c>
#include <ti/grlib/context.c>
#include <ti/grlib/display.c>
#include <ti/grlib/line.c>
#include <ti/grlib/rectangle.c>
#include <ti/grlib/circle.c>
#include <ti/grlib/fonts/fontcm16.c>

//******************************************************************************
//
//! Variable used in the initialization of the arrays for the menù,
//! rappresents the number of different pages that the system can show
//
//******************************************************************************
#define ROW 11

//******************************************************************************
//
//! Variable that rappresent the current page in the menù
//
//******************************************************************************
uint16_t sez=0;

//******************************************************************************
//
//! Variable that rappresent highlighted element in the menù
//
//******************************************************************************
uint16_t sel=1;

Graphics_Context g_sContext;

//******************************************************************************
//
//! Variable used for various purposes:
//! -higlight text
//! -draw an animation
//
//******************************************************************************
Graphics_Rectangle r={10,0,118,0};

//******************************************************************************
//
//! Variable connected to the animation setting. Values:
//! 0: animation off
//! 1: simple animation
//! 2: complex animation
//
//******************************************************************************
uint16_t animation=2;

//******************************************************************************
//
//! Variable that rappresents the speed at which the animation moves
//
//******************************************************************************
int16_t animationSpeed=1;

//******************************************************************************
//
//! Variable to signal when the animation is currently running
//
//******************************************************************************
bool animationOn=false;

//******************************************************************************
//
//! Variable for the selected color of the background
//
//******************************************************************************
int32_t bgColor=GRAPHICS_COLOR_RED;

//******************************************************************************
//
//! Variable for the selected color of the text
//
//******************************************************************************
int32_t txtColor=GRAPHICS_COLOR_WHITE;

//******************************************************************************
//
//! This struct is used to store the data for the menu interface.
//! Every row starts with the page title
//
//******************************************************************************
static char *menu[ROW][6]={
      {"Home","-Metronome","-Settings"},
      {"Metronome","-Digital","-Microphone"},
      {"Settings","-Led on/off","-Led color","-Animation","-Background","-Text"},
      {"Led","ON","OFF"},
      {"Color","Red","Green","Blue"},
      {"Animation","OFF","Static","Moving"},
      {"Bpm"},
      {"Playing..."},
      {"Loading..."},
      {"Background","Red","Black"},
      {"Text","White","Black","Green"}
};

//******************************************************************************
//
//! This array is used to limit how far a user can scroll in a certain page
//
//******************************************************************************
static int limits[ROW]={3,3,6,3,4,4,1,1,1,3,4};

//******************************************************************************
//
//! Variable that is used for the speed of the metronome
//
//******************************************************************************
uint16_t bpm=60;

//******************************************************************************
//
//! This function change the selection of a menu item downwards
//
//******************************************************************************
void selectionDown(){
    if(sel<limits[sez]-1){
       sel++;
       draw();
    }
}


//******************************************************************************
//
//! This function change the selection of a menu item upwards
//
//******************************************************************************
void selectionUp(){
    if(sel>1){
        sel--;
        draw();
    }
}

//******************************************************************************
//
//! This function is used to initialize the lcd screen
//
//******************************************************************************
void setDisplay(){
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, bgColor);
    Graphics_setBackgroundColor(&g_sContext, bgColor);
    GrContextFontSet(&g_sContext, &g_sFontCm16);
    Graphics_clearDisplay(&g_sContext);
    up=&selectionDown;
    down=&selectionUp;
}

//******************************************************************************
//
//! Function to draw a single value in the center of the screen
//
//******************************************************************************
void valueDraw(int a){
    r.yMin=75-10;
    r.yMax=75+12;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE );
    Graphics_fillRectangle(&g_sContext,&r);
    Graphics_setForegroundColor(&g_sContext, bgColor);
    char string[10];
    sprintf(string, "%d", a);
    Graphics_drawStringCentered(&g_sContext,(int8_t *)string,8,64,75,false);
}


//******************************************************************************
//
//! This function decrease the bpm
//
//******************************************************************************
void bpmUp(){
    if(bpm>45){
        bpm--;
        draw();
    }
}
//******************************************************************************
//
//! This function increases the bpm
//
//******************************************************************************
void bpmDown(){
    if(bpm<210){
        bpm++;
        draw();
    }
}

//******************************************************************************
//
//! Function to either reset the screen background color or to draw the
//! different pages for the metronome when the microphone is used
//! as the input
//
//******************************************************************************
void screenColor(int32_t color,int i){
    Graphics_setBackgroundColor(&g_sContext, color);
    Graphics_clearDisplay(&g_sContext);
    r.yMin=64-10;
    r.yMax=64+12;
    Graphics_setForegroundColor(&g_sContext, txtColor );
    if(i!=2){
        Graphics_fillRectangle(&g_sContext,&r);
    }
    Graphics_setForegroundColor(&g_sContext, bgColor);
    if(i==0){
            Graphics_drawStringCentered(&g_sContext,"First sound...",30,64,64,false);
    }else if(i==1){
        Graphics_drawStringCentered(&g_sContext,"Second sound...",30,64,64,false);
    }
}

//******************************************************************************
//
//! This function is used when there is the need to rewrite the current
//! menu page
//
//******************************************************************************
void draw(){
    //reset the rectangle width
    r.xMin=10;
    r.xMax=117;

    //set the rectangle height to draw the page title highlight
    r.yMin=15;
    r.yMax=37;

    //draw the page title and his highlight
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE );
    Graphics_fillRectangle(&g_sContext,&r);
    Graphics_setForegroundColor(&g_sContext, bgColor);
    Graphics_drawStringCentered(&g_sContext,(int8_t *)menu[sez][0],AUTO_STRING_LENGTH,64,25,false);

    //section used for when a user scrolls down in a menu with more than 3 elements
    int i;
    if(sel-1<2){
        i=1;
    }else{
        if(sel-1==limits[sez]){
            i=sel-2;
        }else{
            i=sel-1;
        }
    }

    //rest of the page (if present)
    int cont=1;
    for(;i<limits[sez] && cont<4;i++ && cont++){
        r.yMin+=25;
        r.yMax+=25;
        if(sel==i){ //white rectangle for selected item
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE );
            Graphics_fillRectangle(&g_sContext,&r);
            Graphics_setForegroundColor(&g_sContext, bgColor );
        }else{ //red rectangle to clear the background of an item
                Graphics_setForegroundColor(&g_sContext, bgColor );
                Graphics_fillRectangle(&g_sContext,&r);
                Graphics_setForegroundColor(&g_sContext, txtColor);
        }
        Graphics_drawStringCentered(&g_sContext,(int8_t *)menu[sez][i],AUTO_STRING_LENGTH,64,25+25*cont,false);
    }

    //section fro drawing the arrows if needed
    if(cont!=4){
        r.yMin+=25;
        r.yMax=127;
        Graphics_setForegroundColor(&g_sContext, bgColor );
        Graphics_fillRectangle(&g_sContext,&r);
    }else if(limits[sez]>3){
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE );
        Graphics_drawLine(&g_sContext,40,115,64,125);
        Graphics_drawLine(&g_sContext,64,125,88,115);
        Graphics_drawLine(&g_sContext,40,115,88,115);
    }

    //sections with particular pages, like the bpm set
    if(sez==6){
        valueDraw(bpm);
    }
}

//******************************************************************************
//
//! Function to prepare the screen for the metronome animation
//
//******************************************************************************
void animationSetup(){
    if(animation==1){
        r.yMin=75-17;
        r.yMax=75+17;
        r.xMin=64-17;
        r.xMax=64+17;
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_fillRectangle(&g_sContext,&r);
    }
    r.yMin=75-14;
    r.yMax=75+14;
    r.xMin=50;
    r.xMax=78;
}

//******************************************************************************
//
//! Function to draw on screen the rectangle that is used for the animation
//
//******************************************************************************
void metronomeAnimationSimple(bool t){
        if(t){
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE );
            Graphics_fillRectangle(&g_sContext,&r);
            if(animation==1){
                Graphics_setForegroundColor(&g_sContext, 0x00990000);
            }else{
                Graphics_setForegroundColor(&g_sContext, bgColor);
                animationOn=true;
            }
        }else{
            Graphics_fillRectangle(&g_sContext,&r);
            animationOn=false;
        }

}

//******************************************************************************
//
//! Function to clear, move and then draw the animation square
//
//******************************************************************************
void metronomeAnimatioComplex(){
    Graphics_setForegroundColor(&g_sContext, bgColor );
    Graphics_fillRectangle(&g_sContext,&r);
    r.xMax+=animationSpeed;
    r.xMin+=animationSpeed;
    if(r.xMax>114 || r.xMin<14){
        animationSpeed*=-1;
    }
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK );
    if(!animationOn){
        Graphics_fillRectangle(&g_sContext,&r);
    }
}

//******************************************************************************
//
//! Function to move between pages when a user selects a section
//
//******************************************************************************
void sectionForward(){
    bool move=false;
    Timer_A_stopTimer(TIMER_A1_BASE);
    switch(sez){
    case 0:
        sez=sel;
    break;
    case 1:
        if(sel==1){
            sez=6;
            up=&bpmUp;
            down=&bpmDown;
        }else if(sel==2){
            sez=8;
            Timer_A_setCompareValue(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0,SECONDO);
            time=&micWait;
            scrollEnabled=false;
            Timer_A_startCounter(TIMER_A1_BASE,TIMER_A_UP_MODE);
        }
    break;
    case 2:
        if(sel<4){
            sez=sel+2;
        }else{
            sez=sel+5;
        }
    break;
    case 3:
        if(sel==2){
            led[0]=1;
        }else if(led[0]==1){
            led[0]=GPIO_PORT_P2;
            led[1]=GPIO_PIN6;
        }
        sez=2;
    break;
    case 4:
        switch(sel){
        case 1:
            led[0]=GPIO_PORT_P2;
            led[1]=GPIO_PIN6;
        break;
        case 2:
            led[0]=GPIO_PORT_P2;
            led[1]=GPIO_PIN4;
        break;
        case 3:
            led[0]=GPIO_PORT_P5;
            led[1]=GPIO_PIN6;
        break;
        default:
        break;
        }
    sez=2;
    break;
    case 5:
        sez=2;
        animation=sel-1;
        up=&selectionDown;
        down=&selectionUp;
    break;
    case 6:
        micStartMetronome();
    break;
    case 9:
        if(sel==1){
            bgColor=GRAPHICS_COLOR_RED;
        }else{
            bgColor=GRAPHICS_COLOR_BLACK;
        }
        screenColor(bgColor,2);
        sez=2;
    break;
    case 10:
        if(sel==1){
            txtColor=GRAPHICS_COLOR_WHITE;
        }else if(sel==2){
            txtColor=GRAPHICS_COLOR_BLACK;
        }else{
            txtColor=GRAPHICS_COLOR_GREEN_YELLOW;
        }
        sez=2;
    break;
    default:
        move=true;
    break;
    }
    if(!move){
        Graphics_clearDisplay(&g_sContext);
        sel=1;
        draw();
    }
    if(sez==7){
        animationSetup();
    }
}

//******************************************************************************
//
//! Function to move between menu pages when a user goes back to the
//! previous page
//
//******************************************************************************
void sectionBack(){
    Timer_A_stopTimer(TIMER_A1_BASE);
    Timer_A_stopTimer(TIMER_A0_BASE);
    bool move=false;

    if(sez==1 || sez==2){
        sez=0;
    }else if(sez<6){
        sez=2;
    }else if(sez==6){
        up=&selectionDown;
        down=&selectionUp;
        sez=1;
    }else if(sez==7){
        up=&bpmUp;
        down=&bpmDown;
        scrollEnabled=true;
        sez=6;
        if(led[0]!=1){
            GPIO_setOutputLowOnPin(led[0],led[1]);
        }
        Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_1);
        Timer_A_clearInterruptFlag(TIMER_A2_BASE);
        Timer_A_stopTimer(TIMER_A2_BASE);
        timerB=true;
    }else if (sez==8){
        up=&selectionDown;
        down=&selectionUp;
        sez=1;
        scrollEnabled=true;
        Timer_A_setCompareValue(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0,SECONDO/3);
        screenColor(bgColor,2);
    }else if(sez<11){
        sez=2;
    }else{
        move=true;
    }
    if(!move){
        Graphics_clearDisplay(&g_sContext);
        sel=1;
        draw();
    }
}



