/******************************************************************************
 *
 * Module: The main function of the application
 *
 * File Name: main.c
 *
 * Description: Source file for the main function of Door Locker Security System
 * using ATmega32 microcontroller with frequency 8Mhz.
 *
 * The main connection on the hardware
 * ATmega32 AVCC Pin -> source
 *
 * UART
 * RX -> PD0
 * TX -> PD1
 *
 * For the LCD use 2x16 LCD 4 Mode.:
 * E  -> PA2
 * RW -> GROUND
 * RS -> PA1
 * LCD data Bus -> PORTA  PA3:PA6
 *
 * 4*4 KEYPAD
 * ROW -> PB4:PB7
 * COL -> PD2:PD5
 *
 * Author: Hatem Essam
 *
 * Created on: July 2, 2023
 *
 *******************************************************************************/
#include <avr/io.h> /* To Enable Interrupts Registers */
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer.h"
#include <util/delay.h> /* For the delay functions */

/*******************************************************************************
 *                      Preprocessor                                           *
 *******************************************************************************/

#define F_CPU 8000000L  /*Define the CPU frequency of 8Mhz */
#define PASSWORD_LENGHT 5

/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/
uint8 g_key_num;
uint8 g_counter =0;
uint8 g_Error_checkByte = 0;
uint8 g_Password    [7 ]="";
uint8 g_Timer_tick = 0;
uint8 g_Fun_return ='R';

/*
uint8 LCD_string1 [16]="Plz Enter Pass:";
uint8 LCD_string2 [28]="Plz re-enter the same Pass:";
uint8 LCD_string3 [14]="+ : Open Door";
uint8 LCD_string4 [16]="- : Change Pass";
uint8 LCD_string5 [8 ]=" Alarm ";
uint8 LCD_string6 [18]="Door is Unlocking";
uint8 LCD_string7 [16]="Door is locking";
 */
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void Pass_read (void);
void Pass_save (void);

void Timer_tick(void);

/*******************************************************************************
 *                      Functions Definition                                   *
 *******************************************************************************/
void Pass_read (void){
	for (g_counter=0; g_counter == PASSWORD_LENGHT ;g_counter++){

		/* Get the pressed button from keypad */
		g_key_num = KEYPAD_getPressedKey();
		/*Save the password before send it*/
		g_Password[g_counter]= g_key_num;
		/* Wait 500msec before getting a new press from the keypad buttons, Press time is 500msec */
		_delay_ms(500);
		/*Check for enter bottom*/
		if (g_key_num == '='){
			break;
		}

		/*Display "*" for every number you enter */
		LCD_displayCharacter( '*' );
		/*and move one space for every number you enter */
		LCD_moveCursor(1,g_counter);


	}/*End of for loop*/

	g_counter =0; /*return the counter to zero */

}

void Pass_save (void){
	do{
		LCD_clearScreen();
		LCD_displayString("Plz Enter Pass:");
		LCD_moveCursor(1,0);


		Pass_read();

		/* Send the pressed keys to Control_ECU through UART */
		UART_sendString(g_Password);


		/*------------------- Re-enter the password ------------------------------------------*/

		LCD_clearScreen();
		LCD_displayString("Plz re-enter");
		LCD_moveCursor(1,0);

		Pass_read();

		/* Send the pressed keys to Control_ECU through UART */
		UART_sendString(g_Password);


		/*----------------------- check if it is Wrong --------------------------------------*/

		/* Error_checkByte if = 1 then now error and the opposite is true*/
		g_Error_checkByte = UART_recieveByte();

	}while (g_Error_checkByte == 1);
}

void Timer_tick(void){

	g_Timer_tick++;

}


/*******************************************************************************
 *                        Main Functions                                       *
 *******************************************************************************/

int main(void){

	/*initialize the used modules */
	Timer1_ConfigType Timer1_Configurations ={Timer1_initial_value,Timer1_compare_value,CLK_1024,CTC};

	/* Set the Call back function pointer in the Timer 1 driver */
	Timer1_setCallBack(Timer_tick);

	/* Initialize the UART driver with Baud-rate = 9600 bits/sec */
	UART_ConfigType UART_Configurations ={Eight_Bit,Even_Parity,One_Bit,Normal_Rate};
	UART_init(&UART_Configurations);


	/* Initialize the LCD driver */
	LCD_init();


	// Enable global interrupts in MC.
	SREG  |= (1<<7);




	while (1){


		switch (g_Fun_return){
		/*----------------------- Step 1: Create Password -------------------------------------*/
		/*The letter 'N' to Remake a New Password
		 *The letter 'R' to Rrenter the password
		 *The letter 'E' for Error
		 *The letter 'O' to Open the door */

		case 'R':
			UART_sendByte (g_Fun_return);
			Pass_save ();


			/*No break as the next step is to Take one of the two options */

		case 'D':
			/*----------------------- Step 2: Display main options --------------------------------*/
			/*The LCD will always display the main system option:*/
			LCD_clearScreen();
			LCD_moveCursor(0,0);
			LCD_displayStringRowColumn(0,0,"+ : Open Door");
			LCD_moveCursor(1,0);
			LCD_displayStringRowColumn(0,0,"- : Change Pass");


			/* Get the pressed button from keypad */
			g_key_num = KEYPAD_getPressedKey();


			/*----------------------- Step 3:  Open Door -----------------------------------------*/
			switch (g_key_num){
			case '+':
				do{
					LCD_clearScreen();
					LCD_displayString("Plz Enter Pass:");
					Pass_read();
					/* Send the pressed keys to Control_ECU through UART */
					UART_sendByte ('P');
					UART_sendString(g_Password);
					g_Error_checkByte = UART_recieveByte();

					if (g_Error_checkByte == 0){
						UART_sendByte ('O');
						LCD_clearScreen();
						LCD_displayString("Door is Unlocking");
						Timer1_init(&Timer1_Configurations);
						while (g_Timer_tick == 5);
						while (g_Timer_tick == 6);
						LCD_clearScreen();
						LCD_displayString("Door is locking");
						while (g_Timer_tick == 11);
						Timer1_deInit();
						g_Timer_tick = 0;
						g_counter = 0;
						g_Fun_return ='D';
					}
					else{
						g_counter ++;
					}

				}while(g_counter == 2);

				break;
				/*----------------------- Step 4:  Change Password -----------------------------------*/
			case '-':
				do{
					UART_sendByte ('R');
					LCD_clearScreen();
					LCD_displayString("Plz Enter Pass:");
					Pass_read();
					/* Send the pressed keys to Control_ECU through UART */
					UART_sendString(g_Password);
					g_Error_checkByte = UART_recieveByte();

					if (g_Error_checkByte == 0){
						Pass_save ();
						g_counter = 0;
						g_Fun_return ='D';
					}
					else{
						g_counter ++;
					}
				}while(g_counter == 2);

				break;

			} /*End of switch case*/

		}/*End of the large switch cases  */
		/*----------------------- Check for alarm -----------------------------------*/
		if (g_counter == 2){
			UART_sendByte ('E');
			LCD_clearScreen();
			/*calc time for 1min*/
			Timer1_init(&Timer1_Configurations);
			LCD_displayString(" Alarm ");
			while (g_Timer_tick == 20);
			Timer1_deInit();
			g_Timer_tick = 0;

			g_counter = 0;
			g_Fun_return ='D';
		}

	} /* End of while loop*/

}/*end of main function*/
