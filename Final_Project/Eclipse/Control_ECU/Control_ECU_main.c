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
 * EEPROM
 * SCL -> PC0
 * SDA -> PC1
 *
 * Output device: Buzzer
 * VCC -> Power
 * GND -> PA0
 *
 * H bridge of DC motor:
 * IN1 PB0
 * IN2 PB1
 * EN1 PB3/OC0
 *
 *
 * Author: Hatem Essam
 *
 * Created on: July 2, 2023
 *
 *******************************************************************************/
#include <avr/io.h> /* To Enable Interrupts Registers */
#include "uart.h"
#include "twi.h"
#include "external_eeprom.h"
#include "timer.h"
#include "DC_Motor.h"
#include "buzzer.h"
#include <util/delay.h> /* For the delay functions */


/*******************************************************************************
 *                      Preprocessor                                           *
 *******************************************************************************/

#define F_CPU 8000000L  /*Define the CPU frequency of 8Mhz */
#define EEPEOM_START_ADD 0x0311
#define PASSWORD_LENGHT 5

/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/
uint8 g_counter =0;
uint8 g_Error_check = 0;
uint8 g_Password    [7 ]="";
uint8 g_Re_Password [7 ]="";
uint8 g_Timer_tick = 0;
uint8 g_Fun_return=0;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void Receive_savePass (void);

uint8 Error_Check (uint8 const *arr1,uint8 const *arr2, uint8 Ele_Num );

uint8 Memmory_Comp(uint8 const *arr, uint8 arr_num);

void Open_Door (void);

/*******************************************************************************
 *                      Functions Definition                                   *
 *******************************************************************************/
void Receive_savePass (void){
	do{

		UART_receiveString(g_Password);

		UART_receiveString(g_Re_Password);


		g_Fun_return = Error_Check(g_Password, g_Re_Password, PASSWORD_LENGHT);

		UART_sendByte (g_Fun_return);


	}while(g_Fun_return == 1);
}

/*-----------------------------------------------------------------------------*/

uint8 Error_Check(uint8 const *arr1,uint8 const *arr2, uint8 Ele_Num  ) {

	uint8 Error;
	for (g_counter =0; g_counter <= Ele_Num;g_counter++){
		if (arr1[g_counter] == arr2[g_counter] ){
			Error = FALSE;

		}
		else{
			Error = TRUE;
			break;
		}
	}
	return Error;
}/*end of the function*/

/*-----------------------------------------------------------------------------*/
uint8 Memmory_Comp(uint8 const *arr1, uint8 arr_num){

	uint8 Error;
	for (g_counter =0; g_counter <= arr_num; g_counter++){
		EEPROM_readByte(EEPEOM_START_ADD+g_counter, &g_Error_check);  /* Read 0x0F from the external EEPROM */
		if (arr1[g_counter] == g_Error_check ){
			Error = FALSE;

		}
		else{
			Error = TRUE;
			break;
		}
	}
	return Error;

}/*end of the function*/
/*-----------------------------------------------------------------------------*/
void Open_Door (void){

	/*rotates motor for 15-seconds CW and display a message on the screen
      “Door is Unlocking”*/
	DcMotor_Rotate( CW,100);
	_delay_ms(3000);
	while (g_Timer_tick == 5);
	Timer1_deInit();
	/*hold the motor for 3-seconds.*/
	DcMotor_Rotate( stop,0);
	_delay_ms(3000);
	/*rotates motor for 15-seconds A-CW and display a message on the screen
      “Door is Locking”*/
	DcMotor_Rotate( A_CW,100);
	_delay_ms(1500);
	/*Stop the motor*/
	DcMotor_Rotate( stop,0);

}/*end of the function*/
/*-----------------------------------------------------------------------------*/

void Timer_tick(void){

	g_Timer_tick++;

}
/*******************************************************************************
 *                        Main Functions                                       *
 *******************************************************************************/

int main(void){
	/*initialize the used modules */
	Timer1_ConfigType Timer1_Configurations ={Timer1_initial_value,Timer1_compare_value,CLK_1024,CTC};
	Timer1_init(&Timer1_Configurations);
	/* Set the Call back function pointer in the Timer 1 driver */
	Timer1_setCallBack(Timer_tick);

	/* Initialize the UART driver with Baud-rate = 9600 bits/sec */
	UART_ConfigType UART_Configurations ={Eight_Bit,Even_Parity,One_Bit,Normal_Rate};
	UART_init(&UART_Configurations);

	/*initialize the TWI module */
	TWI_ConfigType TWI_Configurations ={TWI_add,Fast_mode};
	TWI_init(&TWI_Configurations);

	/*initialize the Buzzer module */
	Buzzer_init();
	/*initialize the DcMotor module */
	DcMotor_Init();


	// Enable global interrupts in MC.
	SREG  |= (1<<7);

	/*Local variables*/



	while (1){

		g_Fun_return =UART_recieveByte();
		switch (g_Fun_return){


		/*The letter N to Remake a New Password
		 *The letter R to Reenter the password
		 *The letter E for Error
		 *The letter O to Open the door
		 *The letter P to check the password */

		case 'N':
			UART_receiveString(g_Re_Password);
			g_Fun_return = Memmory_Comp(g_Re_Password, PASSWORD_LENGHT);
			UART_sendByte (g_Fun_return);

			/*No break as the next step is to Receive and Save Password */

		case 'R':
			/*------------- Receive and Save Password   ------------------------------*/
			Receive_savePass();

			/*------------- Save the Password in the EEPROM ---------------------*/
			if (g_Fun_return == 0){

				for(g_counter = 0;g_counter < PASSWORD_LENGHT; g_counter++ )
				{

					EEPROM_writeByte(EEPEOM_START_ADD +g_counter, g_Password[g_counter]); /* Write 0x0F in the external EEPROM */
					_delay_ms(10);
				}/*End for loop*/
			}/*End if condition*/

			/*No break as the next step is to take a decission */
			/*-------------  take a decission  ---------------------------------*/
		case 'P':
			UART_receiveString(g_Re_Password);
			g_Fun_return = Memmory_Comp(g_Re_Password, PASSWORD_LENGHT);
			UART_sendByte (g_Fun_return);
			g_Fun_return =UART_recieveByte();

			break;


		case 'O':

			/*rotates motor for 15-seconds CW and display a message on the screen
			      “Door is Unlocking”*/
			DcMotor_Rotate( CW,100);
			Timer1_init(&Timer1_Configurations);
			while (g_Timer_tick == 5);
			Timer1_deInit();
			/*hold the motor for 3-seconds.*/
			DcMotor_Rotate( stop,0);
			Timer1_init(&Timer1_Configurations);
			while (g_Timer_tick == 6);
			Timer1_deInit();
			/*rotates motor for 15-seconds A-CW and display a message on the screen
			      “Door is Locking”*/
			DcMotor_Rotate( A_CW,100);
			Timer1_init(&Timer1_Configurations);
			while (g_Timer_tick == 11);
			Timer1_deInit();
			/*Stop the motor*/
			DcMotor_Rotate( stop,0);
			g_Timer_tick = 0;
			break;

		case 'E':

			Buzzer_on();
			/*Cal time for 1min*/
			Timer1_init(&Timer1_Configurations);
			while (g_Timer_tick == 20);
			Timer1_deInit();
			g_Timer_tick = 0;

			Buzzer_off();
			break;

		}/*End of swich*/




	} /* End of while loop*/

}/*end of main function*/

