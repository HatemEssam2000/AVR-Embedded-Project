/*
 ============================================================================
 Name        : Mini_Project1.c
 Author      : Hatem Essam
 Version     : First
 Copyright   : Your copyright notice
 Description : Vehicle Control system
 Link        : https://drive.google.com/drive/folders/1ayj33uqTwypug_CVcEVSwg5WGyUCK3qK?usp=share_link
 ============================================================================
 */

#include <stdio.h>

char input; // Global variable for input.

// define a bool type for easer understanding of the code.
#define OFF 0
#define ON 1
typedef unsigned char bool;
//--------------------------------------------------------.

struct vehicle_states {
	bool  Engine_state ;
	bool AC ;
	unsigned short Vehicle_Speed ;
	short Room_Temperature ;
	bool Engine_Temperature_Controller;
	short Engine_Temperature ;

};
struct vehicle_states Vehicle1;

// Sensors_set_menu : A function display the Sensors set menu in the terminal and take the input.
void  Sensors_set_menu (void){

	printf("a. Turn off the engine\n");
	printf("b. Set the traffic light color.\n");
	printf("c. Set the room temperature (Temperature Sensor)\n");
	printf("d. Set the engine temperature (Engine Temperature Sensor)\n\n");
	scanf(" %c",&input);

}
//---------------------------------------------------------------------------.

// Sensors_set_menu_input_check : A function to check the input.
//void  Sensors_set_menu_input_check (char  input){
//
//	do {
//		if (input == 'a' || input == 'b' || input == 'c' || input == 'd'  )
//		{
//			return;
//		}
//		else{
//			printf("You did not enter a valid answer just choose from the list (a, b, c, d)\n\n");
//			scanf(" %c",&input);
//		}
//	}while (1);
//
//}
//---------------------------------------------------------------------------.

// Set_traffic_light : A function to identify the traffic light as input.
void  Set_traffic_light (void){

	// A simple check on the input.
	do {
		printf("Enter the required color\n");
		scanf(" %c",&input);
		if (input == 'g' || input == 'y' || input == 'r')
		{
			break;
		}
		else{
			printf("You did not enter a valid answer just choose from the list (g, y, r)\n\n");
		}
	}while (1);
	//------------------------------.

	switch (input){
	case 'g':
		Vehicle1.Vehicle_Speed =100;
		break;
	case 'y':
		Vehicle1.Vehicle_Speed =30;
		break;
	case 'r':
		Vehicle1.Vehicle_Speed =0;
		break;
	}

}
//---------------------------------------------------------------------------.

// Display_vehicle_states : A function to Display the vehicle states.
void Display_vehicle_states(void){

	Vehicle1.Engine_state == ON?printf("\nEngine is ON \n"):printf("\nEngine is OFF \n");
	Vehicle1.AC == ON?printf("AC is ON \n"):printf("AC is OFF \n");
	printf("Vehicle speed : %d  Km/Hr\n",Vehicle1.Vehicle_Speed);
	printf("Room Temperature : %d  C \n",Vehicle1.Room_Temperature);
	Vehicle1.Engine_Temperature_Controller == ON?printf("Engine Temperature Controller is ON \n"):printf("Engine Temperature Controller is OFF \n");
	printf("Engine Temperature : %d  C \n\n",Vehicle1.Engine_Temperature);
}
//---------------------------------------------------------------------------.

// Set_room_temperature : A function to Set the room temperature.
void  Set_room_temperature (void){

	int temp;  // Local Variable for the input temperature.
	// A simple check on the input.
	do {
		printf("Enter the room temperature \n");
		scanf(" %d",&temp);
		if (temp > -100 && temp < 100)  // I just take a reasonable rang for input check
		{
			break;
		}
		else{
			printf("You did not enter a valid answer enter a number from the range [-100,100]\n\n");
		}
	}while (1);
	//-------------------------.

	if (temp < 10 || temp > 30 ){
		Vehicle1.AC = ON;
		Vehicle1.Room_Temperature = 20;
	}
	else {
		Vehicle1.AC = OFF;
	}

}
//---------------------------------------------------------------------------.

// Set_engine_temperature : A function to Set the engine temperature.
void  Set_engine_temperature (void){
	int temp;  // Local Variable for the input temperature.

	// A simple check on the input.
	do {
		printf("Enter the engine temperature \n");
		scanf(" %d",&temp);
		if (temp > -300 && temp < 300)   // I just take a reasonable rang for input check
		{
			break;
		}
		else{
			printf("You did not enter a valid answer enter a number from the range [-300,300]\n\n");
		}
	}while (1);
	//----------------------------.

	if (temp < 100 || temp > 150 ){
		Vehicle1.Engine_Temperature_Controller = ON;
		Vehicle1.Engine_Temperature = 125;
	}
	else {
		Vehicle1.Engine_Temperature_Controller = OFF;
	}

}
//---------------------------------------------------------------------------.

// set_vehicle_states : The main function to control the vehicle.
void set_vehicle_states(void){

	// initial value for the engine
	Vehicle1.Engine_state = ON;
	Vehicle1.Vehicle_Speed = 0;
	Vehicle1.AC = OFF;
	Vehicle1.Room_Temperature = 20  ;
	Vehicle1.Engine_Temperature_Controller = OFF;
	Vehicle1.Engine_Temperature = 125;
	//--------------------------------




	do {
		Sensors_set_menu ();


		if (Vehicle1.Vehicle_Speed == 30){
			Vehicle1.AC = ON;
			Vehicle1.Room_Temperature = Vehicle1.Room_Temperature*((float)5/4)+1  ;
			Vehicle1.Engine_Temperature_Controller = ON;
			Vehicle1.Engine_Temperature = Vehicle1.Engine_Temperature*((float)5/4)+1;
		}


		if (input == 'a')
		{
			break; // Turn off the vehicle engine
		}
		else if (input == 'b')
		{
			Set_traffic_light ();
			Display_vehicle_states();
		}

		else if (input == 'c')
		{
			Set_room_temperature();
			Display_vehicle_states();
		}
		else if (input == 'd')
		{
			Set_engine_temperature();
			Display_vehicle_states();
		}
		else {
			printf("You did not enter a valid answer just choose from the list (a, b, c, d)\n\n");
		}

	}while (1);


	return ;
}
//---------------------------------------------------------------------------.

// Display : A function to display the options in the terminal and take the input.
void Display (void){

	printf("a. Turn on the vehicle engine\n");
	printf("b. Turn off the vehicle engine\n");
	printf("c. Quit the system\n\n");
	scanf(" %c",&input);
}
//---------------------------------------------------------------------------.

// valid_choice : A function to check the input.
//void  valid_choice (char  input){
//
//	do {
//		if (input == 'a' || input == 'b' || input == 'c')
//		{
//			break;
//		}
//		else{
//			printf("You did not enter a valid answer just choose from the list (a, b, c)\n\n");
//			Display();
//		}
//	}while (1);
//}
//---------------------------------------------------------------------------.

// system_state : A function display the state.
void  system_state (char  input){

	switch(input){
	case 'a':
		printf("Turn on the vehicle engine\n\n");
		break;
	case 'b':
		printf("Turn off the vehicle engine\n\n");
		break;
	case 'c':
		printf("Quit the system\n");
		break;
	}

}
//---------------------------------------------------------------------------.



int main(void) {

	// For printing messages before "scanf" in eclipse terminal.
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	//----------------------------------------------------------.



	do {
		Display();


		if (input == 'a')        // Turn on the engine
		{

			set_vehicle_states ();


		}
		else if (input == 'b')   // Turn off the engine
		{
			system_state (input);
		}
		else if (input == 'c')   // Close the program
		{
			system_state (input);
			break;
		}
		else                    // Wrong input
		{
			printf("You did not enter a valid answer just choose from the list (a, b, c)\n\n");
		}


	}while (1);

	return 0;
}

