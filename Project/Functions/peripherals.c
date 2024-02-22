#include "fundec.h"

//******************************************************************************
//
//! Array used to register single inputs from the joystick
//
//******************************************************************************
bool scorrendo[2]={true,true};

//******************************************************************************
//
//! Variable for checking weather a timer has already started or not
//
//******************************************************************************
bool timerB=true;

//******************************************************************************
//
//! Array used to store the results of the ADC conversions
//
//******************************************************************************
static long int res[2];

//******************************************************************************
//
//! Array to store the color of the led in use by storing the
//! port number and the pin
//
//******************************************************************************
uint16_t led[2]={GPIO_PORT_P2,GPIO_PIN6};

//******************************************************************************
//
//! Variable to enable/disable the automatic scroll for when a user
//! keeps the joystick either up or down
//
//******************************************************************************
bool scrollEnabled=true;

//******************************************************************************
//
//! Variable used when setting the bpm with the microphone
//
//******************************************************************************
bool first=true;

void emptyFun(void){};

//******************************************************************************
//
//! This function is used as the handler for the interrupt generated from
//! the GPIO pin, in particular P5.1 and P3.5.
//! When a signal is received from these pins the system will call the
//! the appropriate function to either go back (sezBack() ) in the menù or go
//! go forward (sezFor() ) to the page selected.
//!
//
//******************************************************************************
void handleGPIO(){
    if(GPIO_getInterruptStatus(GPIO_PORT_P5,GPIO_PIN1)){
        sectionForward();
        GPIO_clearInterruptFlag(GPIO_PORT_P5,GPIO_PIN1);
    }else if(GPIO_getInterruptStatus(GPIO_PORT_P3,GPIO_PIN5)){
        sectionBack();
        GPIO_clearInterruptFlag(GPIO_PORT_P3,GPIO_PIN5);
    }
}

//******************************************************************************
//
//! Function that initializes the necessary GPIO pins:
//! -the input pin 5.1 and 3.5 are set as such with pull-up resistors
//!   and with the function handleGPIO() as the interrupt handler
//! -the RGB Multicolor led pins are set and turned off
//! -the buzzer is initialized
//
//******************************************************************************
void setGpio(){
    //P5.1, forward button
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5,GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P5,GPIO_PIN1);
    GPIO_clearInterruptFlag(GPIO_PORT_P5,GPIO_PIN1);
    GPIO_registerInterrupt(GPIO_PORT_P5,handleGPIO);

    //P3.5 back button
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3,GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P3,GPIO_PIN5);
    GPIO_clearInterruptFlag(GPIO_PORT_P3,GPIO_PIN5);
    GPIO_registerInterrupt(GPIO_PORT_P3,handleGPIO);

    //led: Red 2.6, Green 2.4, Blue 5.6
    GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN4);
    GPIO_setAsOutputPin(GPIO_PORT_P5,GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN4);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5,GPIO_PIN6);

    //P2.7 buzzer
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2,GPIO_PIN7,GPIO_PRIMARY_MODULE_FUNCTION);
}
//******************************************************************************
//
//! This function is used to initialize the ACD devices:
//! -the two axys of the joystick, P4.4 and P6.0
//! -the microphone
//
//******************************************************************************
void setADC(){

        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN3, GPIO_TERTIARY_MODULE_FUNCTION);
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,GPIO_PIN4,GPIO_TERTIARY_MODULE_FUNCTION);
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,GPIO_PIN0,GPIO_TERTIARY_MODULE_FUNCTION);

        ADC14_enableModule();
        ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

        /* Configuring ADC Memory */
        ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM2, true);
        //y = A9
        ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A9,  ADC_NONDIFFERENTIAL_INPUTS);
        //x = A15
        ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);

        ADC14_configureConversionMemory(ADC_MEM2, ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A10, false);

        ADC14_setResultFormat(ADC_SIGNED_BINARY);

        ADC14_enableInterrupt(ADC_INT1);
        Interrupt_enableInterrupt(INT_ADC14);
        Interrupt_enableMaster();
        ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
        ADC14_enableConversion();
        ADC14_toggleConversionTrigger();
}

void micWait(){
    Timer_A_stopTimer(TIMER_A1_BASE);
    screenColor(GRAPHICS_COLOR_GREEN,0);
    time=emptyFun;
    first=true;
    ADC14_enableInterrupt(ADC_INT2);;
}

void micStartMetronome(){
    screenColor(bgColor,2);
    Timer_A_setCompareValue(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0,SECONDO/3);
    up=emptyFun;
    down=emptyFun;
    scrollEnabled=false;
    timerB=false;
    if(animation==2){
        time=&metronomeAnimatioComplex;
        uint16_t fps=bpm/10;
        Timer_A_setCompareValue(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0,SECONDO/fps);
        animationSpeed=100*bpm/(60*fps);
        Timer_A_startCounter(TIMER_A1_BASE,TIMER_A_UP_MODE);
    }
    buzzStart();
    Timer_A_setCompareValue(TIMER_A0_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0,SECONDO*60/bpm);
    sez=7;
    ADC14_disableInterrupt(ADC_INT2);
    draw();
    animationSetup();
}

void micListening(){
    Timer_A_stopTimer(TIMER_A1_BASE);
    bpm=60;
    micStartMetronome();
}

void handMic(int16_t val){
    if(val>800){
        if(first){
            time=micListening;
            first=false;
            Timer_A_setCompareValue(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0,SECONDO);
            Timer_A_startCounter(TIMER_A1_BASE,TIMER_A_UP_MODE);
            screenColor(GRAPHICS_COLOR_YELLOW,1);
        }else{
            Timer_A_stopTimer(TIMER_A1_BASE);
            uint16_t val=Timer_A_getCounterValue(TIMER_A1_BASE);
            long int bp=SECONDO*60/val;
            if(bp>210){
                bpm=210;
            }else{
                bpm=bp;
            }
            micStartMetronome();
        }
    }
}

void handADC(long int x,long int y){
    if(scorrendo[0]){
        if(x>32500 && x<52000 ){
            sectionBack();
            scorrendo[0]=false;
        }else if(x<32100 && x>22000){
            sectionForward();
            scorrendo[0]=false;
        }
    }else{
        if(x>58000 || x<4000){
            scorrendo[0]=true;
        }
    }
    if(scorrendo[1]){
        if(y>32500 && y<52000 ){
            (*up)();
            if(timerB){
                time=up;
                Timer_A_stopTimer(TIMER_A1_BASE);
                Timer_A_startCounter(TIMER_A1_BASE,TIMER_A_UP_MODE);
                timerB=false;
            }
        scorrendo[1]=false;
        }else if(y<32100 && y>22000){
            (*down)();
            if(timerB){
                time=down;
                Timer_A_stopTimer(TIMER_A1_BASE);
                Timer_A_startCounter(TIMER_A1_BASE,TIMER_A_UP_MODE);
                timerB=false;
            }
            scorrendo[1]=false;
        }
    }else{
        if(y>58000 || y<4000){
            if(scrollEnabled!=false){
                Timer_A_stopTimer(TIMER_A1_BASE);
            }
            timerB=scrollEnabled;
            scorrendo[1]=true;
        }
    }
}

void ADC14_IRQHandler(void){
    uint64_t status= ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);
    if(status & ADC_INT1){
        res[0]=ADC14_getResult(ADC_MEM0);
        res[1]=ADC14_getResult(ADC_MEM1);
        handADC(res[0],res[1]);
    }
    if(status & ADC_INT2){
        handMic(ADC14_getResult(ADC_MEM2));
    }
}
