 /******************************************************************************
 *
 * Module: buzzer
 *
 * File Name: buzzer.h
 *
 * Description: header file for the buzzer
 *
 * Author: Hatem Essam
 *
 * Created on: Jul 2, 2023
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define buzzer_PORT PORTA_ID
#define buzzer_PIN PIN0_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 Description
⮚ Setup the direction for the buzzer pin as output pin t
⮚ Turn off the buzzer at the beginning
*/
void Buzzer_init(void);

/*
 Description
⮚ Function to enable the Buzzer.
*/
void Buzzer_on(void);
/*
 Description
⮚ Function to disable the Buzzer .
*/
void Buzzer_off(void);

#endif /* BUZZER_H_ */
