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
	DDRA = 0xFB; PORTA = 0x04;
	//Keypad Input
	DDRB = 0xF0; PORTB = 0x0F;	
	//LCD output
	DDRC = 0xFF; PORTC = 0x00;
	
	LCD_init();	
	
	//Task Scheduler
	tasksNum = 4;
	task tsks[4];
	tasks = tsks;
			
	tasks[0].state = -1;
	tasks[0].period = 100;
	tasks[0].elapsedTime = tasks[0].period;
	tasks[0].TickFct = &Menu_Flow;
			
	tasks[1].state = -1;
	tasks[1].period = 100;
	tasks[1].elapsedTime = tasks[1].period;
	tasks[1].TickFct = &Keypad_Input;
			
	tasks[2].state = -1;
	tasks[2].period = 100;  
	tasks[2].elapsedTime = tasks[2].period;
	tasks[2].TickFct = &Password_Verify;
			
	tasks[3].state = -1;
	tasks[3].period = 100;  
	tasks[3].elapsedTime = tasks[3].period;
	tasks[3].TickFct = &Timer_Status;
			
	TimerSet(100);
	TimerOn();
			
	while(1){}
	
}

