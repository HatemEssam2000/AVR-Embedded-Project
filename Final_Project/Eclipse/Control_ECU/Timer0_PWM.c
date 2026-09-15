/******************************************************************************
 *
 * Module: Timer0
 *
 * File Name: Timer0_PWM.c
 *
 * Description: Source file for the timer0 working as PWM module.
 *
 * Author: Hatem Essam
 *
 * Created on: May 11, 2023
 *
 *******************************************************************************/

#include "Timer0_PWM.h"
#include <avr/io.h>


/*
 * Description:
 * Generate a PWM signal with frequency 500Hz
 * Timer0 will be used with pre-scaler F_CPU/8
 * F_PWM=(F_CPU)/(256*N) = (10^6)/(256*8) = 500Hz
 * Duty Cycle can be changed by updating the value
 * in The Compare Register, Its value should be from 0 → 100
 */
void PWM_Timer0_Start(uint8 duty_cycle){

	TCNT0 = 0; // Set Timer Initial Value to 0

	OCR0  = (uint8)(duty_cycle); //Set Compare value

	DDRB  = DDRB | (1<<PB3); // Configure PB3/OC0 as output pin --> pin where the PWM signal is generated from MC

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);

}
