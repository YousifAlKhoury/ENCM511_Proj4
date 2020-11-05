#include "IOs.h"
#include "TimeDelay.h"
#include <stdio.h>

void IOinit (void) {
    AD1PCFG = 0xFFFF; // Turn all analog pins as digital  
    /* outputs */
    TRISBbits.TRISB8 = 0; // Set pin RB8 as output
    /* inputs */
    TRISBbits.TRISB4 = 1;   // Set pin RB4 as input
    TRISAbits.TRISA2 = 1;   // Set pin RA2 as input
    TRISAbits.TRISA4 = 1;   // Set pin RA4 as input
    CNPU1bits.CN0PUE = 1;   //enables pull up resistor for RA4
    CNPU1bits.CN1PUE = 1;   //enables pull up resistor for RB4
    CNPU2bits.CN30PUE = 1;  //enables pull up resistor for RA2
    
    // configure change on pin interrupt
    CNEN1bits.CN0IE = 1;
    CNEN1bits.CN1IE = 1;
    CNEN2bits.CN30IE = 1;
    
    IEC1bits.CNIE = 1;          // enable CN interrupt

    IPC4bits.CNIP = 0b001;        //set priority level
    
    IFS1bits.CNIF = 0;          //clear IF flag
    
    return;
}

void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void) {
    if (IFS1bits.CNIF == 1) 
    {
        IEC1bits.CNIE = 0;          // temporarily turn off CN interrupt for timers to work
        delay_ms(30);               //debouncing the input
        
        while(PORTAbits.RA2 == 0)   // while PB1 is pushed
        {   
            if( PORTAbits.RA4 == 0 && PORTBbits.RB4 == 0 )  //if any other PB is pushed
            {
                LATBbits.LATB8 = 1; //turn on LED
                Disp2String("\n\r All PBs pressed");
            }
            else if (PORTAbits.RA4 == 0) 
            {
                LATBbits.LATB8 = 1; //turn on LED
                Disp2String("\n\r PB1 and PB2 are pressed");
            }
            else if (PORTBbits.RB4 == 0) 
            {
                LATBbits.LATB8 = 1; //turn on LED
                Disp2String("\n\r PB1 and PB3 are pressed");
            }
            else 
            {
                Disp2String("\n\r PB1 is Pressed");
                LATBbits.LATB8 = ~LATBbits.LATB8;   //toggle LED
                delay_ms(500); //delay 0.5s
            }
        }
        
        while(PORTAbits.RA4 == 0)   // while PB2 is pushed
        {     
            if( PORTAbits.RA2 == 0 && PORTBbits.RB4 == 0 )  //if any other PB is pushed
            {
                LATBbits.LATB8 = 1; //turn on LED
                Disp2String("\n\r All PBs pressed");
            }
            else if (PORTAbits.RA2 == 0) 
            {
                LATBbits.LATB8 = 1; //turn on LED
                Disp2String("\n\r PB2 and PB1 are pressed");
            }
            else if (PORTBbits.RB4 == 0) 
            {
                LATBbits.LATB8 = 1; //turn on LED
                Disp2String("\n\r PB2 and PB3 are pressed");
            }
            else
            {
                Disp2String("\n\r PB2 is Pressed");
                LATBbits.LATB8 = ~LATBbits.LATB8;   //toggle LED
                delay_ms(2000); //delay 2s
            }
            
        }
        while(PORTBbits.RB4 == 0)   // while PB3 is pushed
        {   
            if( PORTAbits.RA4 == 0 && PORTAbits.RA2 == 0 )  //if any other PB is pushed
            {
                LATBbits.LATB8 = 1; //turn on LED
                Disp2String("\n\r All PBs pressed");
            }
            else if (PORTAbits.RA2 == 0) 
            {
                LATBbits.LATB8 = 1; //turn on LED
                Disp2String("\n\r PB3 and PB1 are pressed");
            }
            else if (PORTAbits.RA4 == 0) 
            {
                LATBbits.LATB8 = 1; //turn on LED
                Disp2String("\n\r PB3 and PB2 are pressed");
            }
            else
            {
                Disp2String("\n\r PB3 is Pressed");
                LATBbits.LATB8 = ~LATBbits.LATB8;   //toggle LED
                delay_ms(3000); //delay 3s
            }
        }
        
        if (PORTBbits.RB4 + PORTAbits.RA2 + PORTAbits.RA4) {
            LATBbits.LATB8 = 0;
            Disp2String("\n\r Nothing is Pressed");
        }
        
        IEC1bits.CNIE = 1;      // turn CN interrupt back on
        IFS1bits.CNIF = 0;      // reset interrupt flag
    }
    
}
