 /******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: DC_Motor.h
 *
 * Description: header file for the DC Motor using H bridge
 *
 * Author: Hatem Essam
 *
 * Created on: May 11, 2023
 *
 *******************************************************************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/*L293D H-bridge Hardware connection*/
#define Port_IN1 PORTB_ID
#define IN1 PIN0_ID

#define Port_IN2 PORTB_ID
#define IN2 PIN1_ID

#define Port_EN1 PORTB_ID
#define EN1 PIN3_ID     //OC0

/*DcMotor_State data type is declared as enum*/
typedef enum {
	CW,  A_CW,  stop
}DcMotor_State;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*Description
➢ The Function responsible for setup the direction for the two
motor pins through the GPIO driver.
➢ Stop at the DC-Motor at the beginning through the GPIO driver.
 */
void DcMotor_Init(void);

/* Description:
➢ The function responsible for rotate the DC Motor CW/ or A-CW or
stop the motor based on the state input state value.
➢ Send the required duty cycle to the PWM driver based on the
required speed value.
• Inputs:
➢ state: The required DC Motor state, it should be CW or A-CW or stop.
➢ speed: decimal value for the required motor speed, it should be from
0 → 100.
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed);




#endif /* DC_MOTOR_H_ */
