 /******************************************************************************
 *
 * Module: buzzer
 *
 * File Name: buzzer.c
 *
 * Description: header file for the buzzer
 *
 * Author: Hatem Essam
 *
 * Created on: Jul 2, 2023
 *
 *******************************************************************************/

#include "buzzer.h"
#include "gpio.h"

/*
 Description
⮚ Setup the direction for the buzzer pin as output pin t
⮚ Turn off the buzzer at the beginning
*/
void Buzzer_init(void){
	/* configure pin PC0 and PC1 as output pins */
		GPIO_setupPinDirection(buzzer_PORT, buzzer_PIN, PIN_OUTPUT);

		/* Buzzer is stop at the beginning */
		GPIO_writePin(buzzer_PORT, buzzer_PIN, LOGIC_LOW);
}


/*
 Description
⮚ Function to enable the Buzzer.
*/
void Buzzer_on(void){

	/* Start the Buzzer */
	GPIO_writePin(buzzer_PORT, buzzer_PIN, LOGIC_HIGH);
}
/*
 Description
⮚ Function to disable the Buzzer .
*/
void Buzzer_off(void){

	/* Stop the Buzzer */
	GPIO_writePin(buzzer_PORT, buzzer_PIN, LOGIC_LOW);
}
