/*
 * main.c
 *
 *  Created on: Apr 17, 2023
 *      Author: Hatem Essam
 *     Project: Stop watch 7 Segment Mini Project with external interrupts
 */
/*The # includes */
#include <avr/io.h>         /* Include AVR std. library file */
#include <avr/interrupt.h>  /* Include */
#include <util/delay.h>     /* Include Delay header file */
/*---------------------------------------------------*/

/* The used frequency is 1MHz*/
#define F_CBU 1000000UL
/*---------------------------------------------------*/

/*Global Variables*/

/* global variables contain the 7-segment count values in order*/
unsigned char seg1 = 0;
unsigned char seg2 = 0;
unsigned char seg3 = 0;
unsigned char seg4 = 0;
unsigned char seg5 = 0;
unsigned char seg6 = 0;

/*---------------------------------------------------*/

/*Function prototypes*/
void INT0_Init(void);              // Enable and configure external INT0
void INT1_Init(void);             // Enable and configure external INT1
void INT2_Init(void); 			// Enable and configure external INT2

void Segment_display (unsigned char);        //7_segment display using decoder.

void Timer1_Init (void);            //Configure Timer1

/*---------------------------------------------------*/

/*ISRs*/

/* External INT0 Interrupt Service Routine */
ISR(INT0_vect)
{
	 seg1 = 0;
	 seg2 = 0;
	 seg3 = 0;
     seg4 = 0;
	 seg5 = 0;
	 seg6 = 0;
	/*reset the timer*/
	TCNT1 = 0;              /* Set timer1 initial count to zero */
	// reset stop watch time
}

/* External INT1 Interrupt Service Routine */
ISR(INT1_vect)
{
	/*No clock source (Timer/Counter stopped).*/
	/*clear CS12 bit, CS11 bit, and CS10 bit to stop Timer1*/
	TCCR1B &= ~(1 << CS12) &~(1 << CS11) &~(1 << CS10);
	/*Clear the flag happen automatic*/
}

/* External INT2 Interrupt Service Routine */
ISR(INT2_vect)
{

	/*No clock source (Timer/Counter stopped).*/
	/*set CS12 bit, and CS10 bit to clkI/O/1024 (From prescaler)*/
	TCCR1B |= (1 << CS12) | (1 << CS10);
	/*Clear the flag happen automatic*/

}
ISR(TIMER1_COMPA_vect)
{
	// check the 7-segment as its maximum count is 9
	if(seg1 == 9)
	{
		seg1 = 0;
		/* check  the second 7 segment*/
		if (seg2 == 5){
			seg2=0;
			/*-----------------*/
			if (seg3 == 9){
				seg3=0;
				/*-----------------*/
				if (seg4 == 5){
					seg4=0;
					/*-----------------*/
					if (seg5 == 9 || (seg5==4 && seg6==2)){
						seg5=0;
						/*-----------------*/
						if (seg6 == 2){
							seg6=0;
						}
						else {
							seg6++;
						}
						/*-----------------*/
					}
					else {
						seg5++;
					}
					/*-----------------*/
				}
				else {
					seg4++;
				}
				/*-----------------*/
			}
			else {
				seg3++;
			}
			/*-----------------*/
		}
		else {
			seg2++;
		}

	}
	else
	{
		// increment the first 7-segment every interrupt
		seg1++;
	}
}
/*---------------------------------------------------*/

/*External interrupts enable and configuration functions*/

/* External INT0 enable and configuration function */
void INT0_Init(void)
{
	DDRD  &= (~(1<<PD2));               // Configure INT0/PD2 as input pin
	PORTD |= (1 << PD2);               // enable internal pull-up resistor for INT0 pin
	// Trigger INT1 with the falling edge
	MCUCR |= (1<<ISC01);
	MCUCR &= ~(1<<ISC00);
	GICR  |= (1<<INT0);                 // Enable external interrupt pin INT0

}
/* External INT1 enable and configuration function */
void INT1_Init(void)
{
	DDRD  &= (~(1<<PD3));               // Configure INT1/PD3 as input pin
	MCUCR |= (1<<ISC10) | (1<<ISC11);   // Trigger INT0 with the raising edge
	GICR  |= (1<<INT1);                 // Enable external interrupt pin INT1

}
/* External INT2 enable and configuration function */
void INT2_Init(void)
{
	DDRB   &= (~(1<<PB2));      // Configure INT2/PB2 as input pin
	PORTB |= (1 << PB2);       // enable internal pull-up resistor for INT2 pin
	MCUCSR &= ~(1<<ISC2);     // Trigger INT2 with the falling edge
	GICR   |= (1<<INT2);	 // Enable external interrupt pin INT2

}
/*---------------------------------------------------*/

/* Function to display the numbers from 0 to 9 using decoder 7447*/
void Segment_display (unsigned char Num_seg){

	unsigned char number= 0;
	switch (Num_seg) {
	case 0:
		number = seg1;
		break;
	case 1:
		number = seg2;
		break;
	case 2:
		number = seg3;
		break;
	case 3:
		number = seg4;
		break;
	case 4:
		number = seg5;
		break;
	case 5:
		number = seg6;
		break;
	default:
		number =9;
		break;
	}
		// Display the required number on the 7-segment
		PORTC = (PORTC & 0xF0) | (number & 0x0F);
	}
	/*---------------------------------------------------*/

	/*Timers configuration to count approximately 1 second */
	void Timer1_Init (void){

		TCNT1 = 0;              /* Set timer1 initial count to zero */
		/* Configure timer control register TCCR1A
		 * 1. Normal port operation OC1A COM1A1=0 COM1A0=0
		 * 2. Disconnect OC1B  COM1B0=0 COM1B1=0
		 * 3. FOC1A=1 FOC1B=1 because these bits are only active in case non-pwm mode | (1<<FOC1B)
		 * 4. Clear Timer on Compare Match (CTC) Mode with the TOP in OCR1A WGM10=0 WGM11=0 (Mode Number 4)
		 */
		TCCR1A = (1<<FOC1A)  ;
		/* Configure timer control register TCCR1B
		 * 1. Clear Timer on Compare Match (CTC) Mode with the TOP in OCR1A WGM12=1 WGM13=0 (Mode Number 4)
		 * 2. Prescaler = F_CPU/1024
		 */
		TCCR1B = (1 << CS12) | (1 << CS10) | (1<< WGM12);
		/* Set TOP count (compare value) for timer1 in OCR1A register
		 * OCR1A = (system clock frequency / prescaler value) - 1
		 */
		OCR1A = 976;
		TIMSK |= (1 << OCIE1A);   /*Enable the Timer1 output compare interrupt*/

	}
	/*---------------------------------------------------*/


	int main(void)
	{

		/*Local variables*/
		int i =0;    /*counter*/

		/*----------------------*/

		SREG   |= (1<<7);        // Enable interrupts by setting I-bit

		INT0_Init();              // Enable and configure external INT0
		INT1_Init();             // Enable and configure external INT1
		INT2_Init(); 			// Enable and configure external INT2

		Timer1_Init ();        /*Enable Timer1 */


		DDRC |= 0x0F; 			 // Configure the first four pins in PORTC as output pins of the decoder.
		DDRA |= 0x3F;           // Configure the first six pins in PORTA as output pins of the six 7-segments.
		PORTC &= 0xF0; 		   // 7-segment display zero at the beginning.
		PORTA &= 0xC0; 		   // 7-segment disabled at the beginning.

		while (1){


			/*use for loop to multiplex the six 7_segments in time*/
			for (i=0; i<6 ;i++){
				PORTA = (PORTA & 0x00) | (1<<i);         //Enable different 7_segment
				Segment_display (i);    //display the number on the 7 segment
				_delay_us(2);         //delay 2ms
			}
		}



	}



