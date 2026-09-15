/******************************************************************************
 *
 * Module: timer1
 *
 * File Name: timer.c
 *
 * Description: header file for the Timer 1
 *
 * Author: Hatem Essam
 *
 * Created on: Jul 2, 2023
 *
 *******************************************************************************/
#include <avr/io.h> /* To use the TIMER1 Registers */
#include <avr/interrupt.h>
#include "timer.h"
#include "gpio.h"
#include "common_macros.h"
#include "std_types.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

//#define Timer1_initial_value 0
//#define Timer1_compare_value 100

/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/
static volatile void (*g_Timer1_call_Back_Ptr)(void) = NULL_PTR;

/*******************************************************************************
 *                      Functions Definition                                   *
 *******************************************************************************/
/*
 Description
⮚ Function to initialize the Timer driver
● Inputs: pointer to the configuration structure with type
  Timer1_ConfigType.
● Return: None
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr){




	Config_Ptr->mode;

	TCNT1 = Config_Ptr->initial_value ; //timer initial value
	OCR1A  = Config_Ptr->compare_value ; //compare value


	TIMSK |= (1<<OCIE1A); //enable compare interrupt for channel A
	/* Configure timer1 control registers
	 * 1. Non PWM mode FOC1A=1 and FOC1B=1
	 * 2. No need for OC1A & OC1B in this example so COM1A0=0 & COM1A1=0 & COM1B0=0 & COM1B1=0
	 * 3. CTC Mode and compare value in OCR1A WGM10=0 & WGM11=0 & WGM12=1 & WGM13=0
	 */
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);
	/*
	 * 4. Set the Clock at CS10 CS11 CS12 bits
	 */
	TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->prescaler);
	if((Config_Ptr->mode)==0)
	{

		/* Enable Timer1 overflow interrupts */
		SET_BIT(TIMSK,TOIE1);
		/* Enable overflow flag */
		SET_BIT(TIFR,TOV1);
	}
	else if((Config_Ptr->mode)==4)
	{

		/* Configure timer control register TCCR1B
		 * CTC Mode WGM12=1 WGM13=0
		 */
		SET_BIT(TCCR1B,WGM12);

		/*Set the compare value for compare mode */
		OCR1A=Config_Ptr->compare_value;

		/* Enable Timer1 Compare A Interrupt */
		SET_BIT(TIMSK,OCIE1A);
		/* Enable CTC flag */
		SET_BIT(TIFR,OCF1A);
	}

}
/*
 Description
⮚ Function to disable the Timer1.
● Inputs: None
● Return: None
 */
void Timer1_deInit(void){

	TCNT1 = 0;
	OCR1A  = 0;
	TCCR1A=0;

	g_Timer1_call_Back_Ptr = NULL_PTR;

}

/*
 Description
⮚ Function to set the Call Back function address.
● Inputs: pointer to Call Back function.
● Return: None
 */
void Timer1_setCallBack(void(*a_ptr)(void)){

	g_Timer1_call_Back_Ptr = a_ptr;
}


/*******************************************************************************
 *                      ISR                                                    *
 *******************************************************************************/

// Interrupt Service Routine for timer1 Oerflow mode channel A
ISR(TIMER1_OVF_vect)
{
	if(g_Timer1_call_Back_Ptr != NULL_PTR)
	{
		(*g_Timer1_call_Back_Ptr)();
	}
}
// Interrupt Service Routine for timer1 compare mode channel A
ISR(TIMER1_COMPA_vect)
{
	if(g_Timer1_call_Back_Ptr != NULL_PTR)
	{
		(*g_Timer1_call_Back_Ptr)();
	}
}
