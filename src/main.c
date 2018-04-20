/*
	Baldomero Vargas
	861143461
	CS179J
	MC#1
*/
#include <avr/io.h>
#include "scheduler.h"
#include "states.h"

int main(void)
{
	//Sensor Input 
	DDRA=0x00; PORTA = 0xFF;
	//Keypad Input
	DDRB = 0xF0; PORTB = 0x0F;	
	//LCD output
	DDRC = 0xFF; PORTC = 0x00;
	//testing port
	DDRD = 0xFF; PORTD = 0x00;
	
	LCD_init();	
	LCD_ClearScreen();
	LCD_Cursor(1);
	
	unsigned char testing = 0;
	
	if(testing){
		//insert test cases
	}
	else{
		//Task Scheduler
		tasksNum = 2;
		task tsks[2];
		tasks = tsks;
			
		tasks[0].state = -1;
		tasks[0].period = 100;
		tasks[0].elapsedTime = tasks[0].period;
		tasks[0].TickFct = &Menu_Flow;
			
		tasks[1].state = -1;
		tasks[1].period = 100;   //update somehow
		tasks[1].elapsedTime = tasks[1].period;
		tasks[1].TickFct = &Keypad_Input;
			
		/*tasks[2].state = -1;
		tasks[2].period = 50;   //update somehow
		tasks[2].elapsedTime = tasks[2].period;
		tasks[2].TickFct = &Password_Reset;
			
		tasks[3].state = -1;
		tasks[3].period = 50;   //update somehow
		tasks[3].elapsedTime = tasks[3].period;
		tasks[3].TickFct = &Lock_Logic;*/
			
		TimerSet(100);
		TimerOn();
			
		while(1){}
	}
	
}

