#include <xc.h>
#include "interrupts.h"

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
************************************/
void Interrupts_init(void)
{   //PIE2bits.C1IE=1; 	//enable comparator
    //INTCONbits.GIEL=1;  // turn on peripheral interrupts
    PIE0bits.TMR0IE = 1;
    INTCONbits.GIE=1; 	//turn on interrupts globally
	// turn on global interrupts, peripheral interrupts and the interrupt source 
	// It's a good idea to turn on global interrupts last, once all other interrupt configuration is done.
}

/************************************
 * High priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
************************************/
void __interrupt(high_priority) HighISR()
{   if(PIR0bits.TMR0IF){    //check the interrupt source some code you want to execute here; 
    TMR0H=0b00001011;  // rewritting initial values
    TMR0L=0b11011011;  // rewritting initial values
    PIR0bits.TMR0IF = 0;  // clear the flag
	}
	//add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...
}

