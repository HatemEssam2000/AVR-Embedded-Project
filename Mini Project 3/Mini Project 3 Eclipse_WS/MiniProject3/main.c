/******************************************************************************
 *
 * Module: The main function of the application
 *
 * File Name: main.c
 *
 * Description: Source file for the main function Fan Controller system
 * using using ATmega32 microcontroller with frequency 1Mhz.
 *
 * The main connection on the hardware
 * For the LCD:
 * AVCC source
 * E  PD2
 * RW PD1
 * RS PD0
 * LCD data PORTC
 *
 * SENSOR: PA2/ADC2
 * H bridge of DC motor:
 * IN1 PB0
 * IN2 PB1
 * EN1 PB3/OC0
 *
 *
 * Author: Hatem Essam
 *
 * Created on: May 11, 2023
 *
 *******************************************************************************/

#include "lcd.h"
#include "lm35_sensor.h"
#include "DC_Motor.h"
#include "adc.h"
#include "std_types.h"




int main (void){

    /*initialize the used modules */
	ADC_ConfigType ADC_Configurations ={D_FACTOR_8,Internal_Vref};
	ADC_init(&ADC_Configurations);
	DcMotor_Init();
	LCD_init();

	/*local variable*/
	uint8 Temperture_value ;
	/*LCD strings you will use to display*/
	uint8 LCD_status_OFF[12]= "Fan is OFF";
	uint8 LCD_status_ON[12]= "Fan is ON ";
	uint8 LCD_Temp_Value[12]="Temp =    C";

	while (1){

		Temperture_value = LM35_getTemperature();

		/* Display the temperature value every time at same position */
		LCD_displayStringRowColumn(1,2,LCD_Temp_Value);
		LCD_moveCursor(1,9);
		if(Temperture_value >= 100)
		{
			LCD_intgerToString(Temperture_value);
		}
		else
		{
			LCD_intgerToString(Temperture_value);
			/* In case the digital value is two or one digits print space in the next digit place */
			LCD_displayCharacter(' ');
		}




		/*control the speed of the motor base on the sensed temperature
		 * and show the motor state on/off on the LCD
		 */
		if (Temperture_value < 30){
			//turn off the fan
			DcMotor_Rotate( stop, 0);
			LCD_displayStringRowColumn(0,2,LCD_status_OFF);

		}
		else if (Temperture_value >= 30 && Temperture_value < 60 ){
			// 25% of the maximum speed
			DcMotor_Rotate( CW, 25);
			LCD_displayStringRowColumn(0,2,LCD_status_ON);

		}
		else if (Temperture_value >= 60 && Temperture_value < 90 ){
			// 50% of the maximum speed
			DcMotor_Rotate( CW, 50);
			LCD_displayStringRowColumn(0,2,LCD_status_ON);

		}
		else if (Temperture_value >= 90 && Temperture_value < 120 ){
			// 75% of the maximum speed
			DcMotor_Rotate( CW, 75);
			LCD_displayStringRowColumn(0,2,LCD_status_ON);
		}
		else if(Temperture_value >= 120 && Temperture_value <= 150) {
			// 100% of the maximum speed
			DcMotor_Rotate( CW, 100);
			LCD_displayStringRowColumn(0,2,LCD_status_ON);
		}





	}/* close the loop*/

}/*end of the function*/
