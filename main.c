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
    unsigned char enemy_pos = 0XC0 + 25;  // position at the end of the second row
    unsigned char enemy_pos2 = 0X80;  // position at the end of the first row
    unsigned char bullet1 = 0XC0 + 25;
    unsigned char bullet2 = 0X80;
    unsigned int second = 0; 
    unsigned int temp = 0;
    char buf[0];

    LCD_update_screen(player_pos, enemy_pos, enemy_pos2, bullet1, bullet2);
    TRISFbits.TRISF2=1; //set TRIS value for pin (input)
    ANSELFbits.ANSELF2=0; //turn off analogue input on pin     
    TRISFbits.TRISF3=1; //set TRIS value for pin (input)
    ANSELFbits.ANSELF3=0; //turn off analogue input on pin  
    
   unsigned char player[] = {  // obstacle character
        0b01110,
        0b00100,
        0b01110,
        0b11111,
        0b01110,
        0b01010,
        0b01010,
        0b00000,  // cursor position
    };
   
    LCD_create_character(player, 0);
    
    while (1) {
        if (T0CON0bits.T0OUT){
            Timer0_restart();
            second += 1;
            if (--enemy_pos < 0xC0) {enemy_pos = 0xC0 + 20;}
            if (++enemy_pos2 > 0x80 + 15) {enemy_pos2 = 0x80;}
            if (second % 5 == 0){bullet1 = enemy_pos;} 
            if (second % 3 == 0){bullet2 = enemy_pos2;} 
            if (second == 1000){second = 0;}
            bullet1 -=2; 
            bullet2 +=2;                  
            LCD_update_screen(player_pos, enemy_pos, enemy_pos2, bullet1, bullet2);
        }
        if (PORTFbits.RF3 == 0){
            player_pos = LCD_move(player_pos,enemy_pos,enemy_pos2,bullet1,bullet2);
            __delay_ms(200); // Delay is needed to improve the smoothness of the player's movement
        }
        if (PORTFbits.RF3 == 0 && PORTFbits.RF2  == 0){
            LCD_setline(1);
            LCD_sendstring("((((  BOOM  ))))");               //display 'Game Over!!' in Line 1
            LCD_setline(2);
            LCD_sendstring("((((  BOOM  ))))");               //display 'Press RF2 to Go' in Line 2
            enemy_pos = 0XC0;  // position at the end of the second row
            enemy_pos2 = 0X80 + 42;  // position at the end of the first row
            bullet1 = 0XC0;
            bullet2 = 0X80 + 42;
            __delay_ms(2000);
        }
        
        if (PORTFbits.RF2 == 0 && PORTFbits.RF3 == 1){
            __delay_ms(200);
            temp = second;
            while (PORTFbits.RF2){
                LCD_setline(1);
                LCD_sendstring("You Paused!      ");
                LCD_setline(2);
                LCD_sendstring("Press RF2 to Go ");     //display 'Press RF2 to Go' in Line 2
            }
            second = temp;
            __delay_ms(200);
        }

        
        if (player_pos == enemy_pos || player_pos == enemy_pos2 || player_pos == bullet1 || player_pos == bullet2){
            LCD_setline(1);
            LCD_sendstring("Game Over!!     ");               //display 'Game Over!!' in Line 1
            LCD_setline(2);
            LCD_sendstring("Press RF2 to Go ");     //display 'Press RF2 to Go' in Line 2
            showresult(buf,second);
            __delay_ms(2000);
            while (PORTFbits.RF2){
                LCD_setline(1);
                LCD_sendstring("You survived");
                LCD_setline(2);
                LCD_sendstring(buf);
            };
            enemy_pos = 0XC0 + 25;  // position at the end of the second row
            enemy_pos2 = 0X80;  // position at the end of the first row
            bullet1 = 0XC0 + 25;
            bullet2 = 0X80;
            second = 0;
            player_pos = 0xC0 + 7;
            __delay_ms(200); // Delay is needed to improve the smoothness of the player's movement
            
        }
    }
}

