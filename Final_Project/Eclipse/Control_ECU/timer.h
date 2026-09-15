 /******************************************************************************
 *
 * Module: timer1
 *
 * File Name: timer.h
 *
 * Description: header file for the Timer 1
 *
 * Author: Hatem Essam
 *
 * Created on: Jul 2, 2023
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define Timer1_initial_value 0
#define Timer1_compare_value 23437


/*Timer1_Prescaler data type is declared as enum*/
typedef enum {
	NO_CLK, CLK, CLK_8, CLK_64, CLK_256, CLK_1024, FALL_EX_CLK, RIS_EX_CLK
}Timer1_Prescaler;

/*Timer1_Mode data type is declared as enum*/
typedef enum {
	NORMAL,  PWM = 3,CTC = 4
}Timer1_Mode;


typedef struct {
 uint16 initial_value;
 uint16 compare_value; // it will be used in compare mode only.
 Timer1_Prescaler prescaler;
 Timer1_Mode mode;
} Timer1_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 Description
⮚ Function to initialize the Timer driver
● Inputs: pointer to the configuration structure with type
  Timer1_ConfigType.
● Return: None
*/
void Timer1_init(const Timer1_ConfigType * Config_Ptr);
/*
 Description
⮚ Function to disable the Timer1.
● Inputs: None
● Return: None
*/
void Timer1_deInit(void);

/*
 Description
⮚ Function to set the Call Back function address.
● Inputs: pointer to Call Back function.
● Return: None
*/
void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER_H_ */
