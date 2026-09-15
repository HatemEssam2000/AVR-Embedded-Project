/******************************************************************************
 *
 * Module: The main function of the application
 *
 * File Name: main.c
 *
 * Description: Source file for the main function of distance measurement
 * using ATmega32 microcontroller with frequency 8Mhz.
 *
 * The main connection on the hardware
 * ATmega32 AVCC Pin -> source
 *
 * For the LCD use 4x16 LCD.:
 * E  -> PB1
 * RW -> GROUND
 * RS -> PB0
 * LCD data Bus -> PORTA
 *
 * SENSOR: HC-SR04 Ultrasonic
 * VCC -> Power
 * GND -> GROUND
 * TR  -> PB5
 * ECHO-> PD6
 *
 *
 * Author: Hatem Essam
 *
 * Created on: June 27, 2023
 *
 *******************************************************************************/
#include "std_types.h"
#include <avr/io.h>
#include "lcd.h"
#include "HC-SR04_Ultrasonic.h"

int main (void){

	LCD_init();


	/*local variable*/
	uint16 distance;
	uint8 LCD_Text[16]="Distance=    cm";

	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);

	while (1){

		Ultrasonic_init();


		distance = Ultrasonic_readDistance();

				/* Display the Distance value every time at same position */
				LCD_displayStringRowColumn(1,1,LCD_Text);
				LCD_moveCursor(1,12);
				if(distance >= 100)
				{
					LCD_intgerToString(distance);
				}
				else
				{
					LCD_intgerToString(distance);
					/* In case the digital value is two or one digits print space in the next digit place */
					LCD_displayCharacter(' ');
				}



	} /* close of the while loop*/
	return 0;
} /*close of the main function*/
