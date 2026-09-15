/******************************************************************************
 *
 * Module: Timer0
 *
 * File Name: Timer0_PWM.h
 *
 * Description: Header file for the timer0 working as PWM module.
 *
 * Author: Hatem Essam
 *
 * Created on: May 11, 2023
 *
 *******************************************************************************/

#ifndef TIMER0_PWM_H_
#define TIMER0_PWM_H_

#include "std_types.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*Description:
➢ The function responsible for trigger the Timer0 with the PWM Mode.
➢ PWM mode with Non-Inverting.
➢ prescaler with F_CPU/8.
➢ compare value based on the required input duty cycle
➢ direction for OC0 as output pin through the GPIO driver.
➢ The generated PWM signal frequency will be 500Hz to control the DC Motor speed F_PWM=(F_CPU)/(256*N) = (10^6)/(256*8) = 500Hz.
Inputs:
➢ duty_cycle: The required duty cycle percentage of the generated PWM signal. Its value should be from 0 → 100
,so OCR0 is 255 value register 0->0%, and 64->25%, and 128->50%, and 192->75%, and 255->100%
*/
void PWM_Timer0_Start(uint8 duty_cycle);




#endif /* TIMER0_PWM_H_ */
