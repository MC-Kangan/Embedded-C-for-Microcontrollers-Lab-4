// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include <stdio.h>
#include "LCD.h"
#include "ADC.h"


#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) {
    LCD_Init();  
    ADC_init();
	//LCD_sendstring("Hello World! Nice to see u");
    //LCD_setline(2);
    //LCD_sendstring("Hello World!");
    
    unsigned int ADC_val = 0;
	char buf[0]; // Define an array to store string
    
//	LCD_sendbyte(0x48,1); //H
//	LCD_sendbyte(0x65,1); //E
//	LCD_sendbyte(0x6C,1); //L	
//	LCD_sendbyte(0x6C,1); //L
//	LCD_sendbyte(0x6F,1); //O
//	LCD_sendbyte(0x20,1); //(space)
//	LCD_sendbyte(0x57,1); //W
//	LCD_sendbyte(0x6F,1); //O
//	LCD_sendbyte(0x72,1); //R
//	LCD_sendbyte(0x6C,1); //L 
//	LCD_sendbyte(0x64,1); //D
//	LCD_sendbyte(0x21,1); //!

    while (1) {
    //LCD_scroll();
        LCD_setline(1); // Set Line 1
        ADC_val = ADC_getval(); // Get ADC_value
        ADC2String(buf,ADC_val); // Create the string to be displayed
        LCD_sendstring(buf);
        __delay_ms(300);
    }
}

