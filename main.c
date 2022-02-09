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

    unsigned char player_pos = 0xC0 + 7;        // position at the second row
    unsigned char enemy_pos = 0XC0 + 18;  // position at the end of the second row
    unsigned char enemy_pos2 = 0X80;  // position at the end of the first row
    unsigned char bullet1 = 0;
    //unsigned char bullet2 = 0;
    unsigned char second = 0; 

    LCD_update_screen(player_pos, enemy_pos, enemy_pos2, bullet1);
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
            second += 1;
            if (second % 5 == 0){bullet1 = enemy_pos;} 
            //if (second % 7 == 0){bullet2 = enemy_pos2;} 
            if (second == 60){second = 0;}
            bullet1 -=2;
            //bullet2 +=3;
            //if (bullet1 < 0xC0){bullet1 = 0;}
            //if (bullet1 > 0x80){bullet2 = 0;}
            if (--enemy_pos < 0xC0) {enemy_pos = 0xC0 + 18;}
            //LCD_update_screen(player_pos, enemy_pos, enemy_pos2);
            if (++enemy_pos2 > 0x80 + 15) {enemy_pos2 = 0x80;}
            LCD_update_screen(player_pos, enemy_pos, enemy_pos2, bullet1);
        }
        if (PORTFbits.RF3 == 0){
            player_pos = LCD_move(player_pos,enemy_pos,enemy_pos2,bullet1);
            __delay_ms(200); // Delay is needed to improve the smoothness of the player's movement
        }
        if (player_pos == enemy_pos || player_pos == enemy_pos2 || player_pos == bullet1){
            //LCD_sendbyte(1,0); // Clear Display
            LCD_setline(1);
            LCD_sendstring("Game Over!!");               //display 'Game Over!!' in Line 1
            LCD_setline(2);
            LCD_sendstring("Press RF2 to Go");     //display 'Press RF2 to Go' in Line 2
            while (PORTFbits.RF2);
            __delay_ms(200); // Delay is needed to improve the smoothness of the player's movement
            //break;
        }
    }
}

