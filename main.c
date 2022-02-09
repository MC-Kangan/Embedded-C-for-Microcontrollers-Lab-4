// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include <stdio.h>
#include "LCD.h"
#include "ADC.h"
#include "interrupts.h"
#include "timers.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) {
    LCD_Init();  
    //ADC_init();
    Interrupts_init();
    Timer0_init();
    //  unsigned int ADC_val = 0;
    //	char buf[0]; // Define an array to store string

    unsigned char player_pos = 0xC0 + 1;  // position at the second row
    unsigned char enemy_pos = 0XC0 + 15;  // position at the end of the second row
    unsigned char player[] = {  // person character
        0b10000,
        0b01000,
        0b00100,
        0b00011,
        0b00100,
        0b01000,
        0b10000,
        0b00000  // cursor position
    };
    unsigned char enemy[] = {  // enemy character
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b00000,  // cursor position
    };
    LCD_sendbyte(0b01000100,0);
    LCD_sendbyte(0b10111011,0);
    
    LCD_update_screen(player_pos, enemy_pos);
    TRISFbits.TRISF2=1; //set TRIS value for pin (input)
    ANSELFbits.ANSELF2=0; //turn off analogue input on pin  
    
    TRISFbits.TRISF3=1; //set TRIS value for pin (input)
    ANSELFbits.ANSELF3=0; //turn off analogue input on pin  
    
    while (1) {
    //LCD_scroll();
//        LCD_setline(1); // Set Line 1
//        ADC_val = ADC_getval(); // Get ADC_value
//        ADC2String(buf,ADC_val); // Create the string to be displayed
//        LCD_sendstring(buf);
//        __delay_ms(300);
        //Sleep();
        if (T0CON0bits.T0OUT){
            Timer0_restart();
            if (--enemy_pos < 0xC0) {enemy_pos = 0xC0 + 15;}
            LCD_sendbyte(1,0);  // clear display
            __delay_ms(2);
            LCD_update_screen(player_pos, enemy_pos);
        }
        if (player_pos == enemy_pos){
            LCD_setline(1);
            LCD_sendstring("Game Over!!");               //display 'Game Over!!' in Line 1
            LCD_setline(2);
            LCD_sendstring("Press RF2 to Go");     //display 'Press RF2 to Go' in Line 2
            while (PORTFbits.RF2);
            //break;
        }
    }
}

