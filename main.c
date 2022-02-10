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


    unsigned char player_pos = 0xC0 + 7;        // position of the player1
    unsigned char enemy_pos = 0XC0 + 25;  // position of the enemy1 at the second row
    unsigned char enemy_pos2 = 0X80;  // position of the enemy2 at the first row
    unsigned char bullet1 = 0XC0 + 25;
    unsigned char bullet2 = 0X80;
    unsigned int second = 0; 
    unsigned int temp = 0;
    char buf[0];

    LCD_update_screen(player_pos, enemy_pos, enemy_pos2, bullet1, bullet2); // Update the screen
    TRISFbits.TRISF2=1; //set TRIS value for pin (input)
    ANSELFbits.ANSELF2=0; //turn off analogue input on pin     
    TRISFbits.TRISF3=1; //set TRIS value for pin (input)
    ANSELFbits.ANSELF3=0; //turn off analogue input on pin  
    
   unsigned char player[] = {  // Create character
        0b01110,
        0b00100,
        0b01110,
        0b11111,
        0b01110,
        0b01010,
        0b01010,
        0b00000,  // cursor position
    };
   
    LCD_create_character(player, 0); //Create character at CGRAM_loc = 0
    
    while (1) {
        if (T0CON0bits.T0OUT){
            Timer0_restart(); // Note: after some adjustments, the timer overflow every 0.5s
            second += 1; // Thus, second/2 will be the actual seconds in time
            if (--enemy_pos < 0xC0) {enemy_pos = 0xC0 + 20;} // Move the enemy 1 to the left
            if (++enemy_pos2 > 0x80 + 15) {enemy_pos2 = 0x80;} // Move the enemy 2 to the right
            if (second % 5 == 0){bullet1 = enemy_pos;} // Shoot a bullet every 5s (actually it is 2.5s)
            if (second % 3 == 0){bullet2 = enemy_pos2;} // Shoot a bullet every 3s (Actually it is 1.5s)
            if (second == 1000){second = 0;} // If second gets too high, reset the second
            bullet1 -=2; // Movement of bullet 1
            bullet2 +=2; // Movement of bullet 2                 
            LCD_update_screen(player_pos, enemy_pos, enemy_pos2, bullet1, bullet2); // Update the screen with the location of all characters
        }
        if (PORTFbits.RF3 == 0){ // if RF3 is pressed, the player jump up and down
            player_pos = LCD_move(player_pos,enemy_pos,enemy_pos2,bullet1,bullet2); // Function to move the player
            __delay_ms(200); // Delay is needed to improve the smoothness of the player's movement
        }
        if (PORTFbits.RF3 == 0 && PORTFbits.RF2  == 0){ // Once the two buttons are pressed at the same time, initiate a boom
            LCD_setline(1); // Write line 1
            LCD_sendstring("((((  BOOM  ))))");               
            LCD_setline(2); // Write line 2
            LCD_sendstring("((((  BOOM  ))))");               
            enemy_pos = 0XC0;        // Reset the location of enemy 1 
            enemy_pos2 = 0X80 + 40;  // Reset the location of enemy 2
            bullet1 = 0XC0;
            bullet2 = 0X80 + 40;
            __delay_ms(2000);
        }
        
        if (PORTFbits.RF2 == 0 && PORTFbits.RF3 == 1){ // When RF2 is pressed but RF3 is not pressed
            __delay_ms(200);
            temp = second; // Store the current second in temp
            while (PORTFbits.RF2){
                LCD_setline(1);
                LCD_sendstring("You Paused!      ");
                LCD_setline(2);
                LCD_sendstring("Press RF2 to Go ");     //display 'Press RF2 to Go' in Line 2
            }
            second = temp; // Continue to time 
            __delay_ms(200);
        }

        // If you get hit by the bullet and crashed by the plane (enemy)
        if (player_pos == enemy_pos || player_pos == enemy_pos2 || player_pos == bullet1 || player_pos == bullet2){
            LCD_setline(1);
            LCD_sendstring("Game Over!!     ");     //display 'Game Over!!' in Line 1
            LCD_setline(2);
            LCD_sendstring("Press RF2 to Go ");     //display 'Press RF2 to Go' in Line 2
            showresult(buf,second); // Show result will present the accumulative seconds that you played before game over
            __delay_ms(2000);
            while (PORTFbits.RF2){ //If the RF2 is not pressed, display the following information
                LCD_setline(1);
                LCD_sendstring("You survived");
                LCD_setline(2);
                LCD_sendstring(buf); // Print the message 'XX seconds' from the showresult function
            };
            enemy_pos = 0XC0 + 25;  // Reset the position of the enemy1
            enemy_pos2 = 0X80;  // Reset the position of the enemy2
            bullet1 = 0XC0 + 25;
            bullet2 = 0X80;
            second = 0; // After gameover, reset the time
            player_pos = 0xC0 + 7; // Reset the player location
            __delay_ms(200); // Delay is needed to improve the smoothness of the player's movement
            
        }
    }
}

