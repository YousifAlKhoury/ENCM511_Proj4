#include "TimeDelay.h"
#include <stdio.h>

void delay_ms (uint16_t time_ms) {
    NewClk(32);             //configure clock
    
    //configuring T2CON
    T2CONbits.TON = 1;      //start 16 bit timer
    T2CONbits.TSIDL = 0;    //continue operation in Idle mode
    T2CONbits.TCS = 0;      //use Internal Clock
    T2CONbits.T32 = 0;      //use Timer2 as standalone 16 bit timer
    T2CONbits.TCKPS = 0b01; //PreScaler 1:8
    //configuring T2CON 
    //T2CON = 0x8000;       //alternate way to set T2CON bits
    
    IFS0bits.T2IF=0;        //Clear timer 2 interrupt flag
    
    PR2 = time_ms*2;        //PR2 = time/(prescaler*(2/fclk)*1000)
 
    Idle();
    
}

// Timer 2 interrupt subroutine
void __attribute__((interrupt, no_auto_psv))_T2Interrupt(void)
{
    IFS0bits.T2IF=0;        //Clear timer 2 interrupt flag
    T2CONbits.TON=0;
    TMR2 = 0;               //clear TMR2
    //TMR2flag = 1;         // global variable created by user
}