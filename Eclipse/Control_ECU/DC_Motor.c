/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: DC_Motor.c
 *
 * Description:
 * Control the DC Motor direction using L293D H-bridge.
 * Control The DC Motor Speed using PWM from MC. connect the
 * PWM PIN of Timer0 OC0(PB3) to the enable of the Motor.
 * duty cycle 100% --> Maximum Speed
 * duty cycle 75%  --> three quarters Maximum Speed
 * duty cycle 50%  --> Half Maximum Speed
 * duty cycle 25%  --> Quarter Maximum Speed
 * duty cycle 0%   --> Motor Stop
 *
 * Author: Hatem Essam
 *
 * Created on: May 11, 2023
 *
 *******************************************************************************/
#include "DC_Motor.h"
#include "Timer0_PWM.h"
#include "gpio.h"


void DcMotor_Init(void){
	/* configure pin PC0 and PC1 as output pins */
	GPIO_setupPinDirection(Port_IN1, IN1, PIN_OUTPUT);
	GPIO_setupPinDirection(Port_IN2, IN2, PIN_OUTPUT);

	/* Motor is stop at the beginning */
	GPIO_writePin(Port_IN1, IN1, LOGIC_LOW);
	GPIO_writePin(Port_IN2, IN2, LOGIC_LOW);

}


void DcMotor_Rotate(DcMotor_State state,uint8 speed){

	/* generate duty cycle as [duty cycle = (speed/100) *255]
	 * to change the motor speed based on a given percentage
	 */
	PWM_Timer0_Start((uint8)((speed*255)/100));
	/*choose the motor state and direction*/
	switch (state){
	case CW:             // Rotate the motor --> clock wise
		GPIO_writePin(Port_IN1, IN1, LOGIC_LOW);
		GPIO_writePin(Port_IN2, IN2, LOGIC_HIGH);
		break;
	case A_CW:           // Rotate the motor --> anti-clock wise
		GPIO_writePin(Port_IN1, IN1, LOGIC_HIGH);
		GPIO_writePin(Port_IN2, IN2, LOGIC_LOW);
		break;
	case stop:          // Stop the motor
		GPIO_writePin(Port_IN1, IN1, LOGIC_LOW);
		GPIO_writePin(Port_IN2, IN2, LOGIC_LOW);
		break;
	default:
		break;
	}

}



