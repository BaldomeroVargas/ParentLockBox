#ifndef STATES_H
#define STATES_H

#include <avr/eeprom.h>
#include "keypad.h"
#include "lcd.h"
#include "mstrings.h"
#include "memory.h"

//ran on system reset or initialization
void system_setup(){
	
	eeprom_write_byte((uint8_t*)75, '0');
	eeprom_write_byte((uint8_t*)76, '0');
	eeprom_write_byte((uint8_t*)77, '0');
	eeprom_write_byte((uint8_t*)78, '0');
	
	eeprom_write_byte((uint8_t*)100, 'x');
	eeprom_write_byte((uint8_t*)101, 'x');
	eeprom_write_byte((uint8_t*)102, 'x');
	eeprom_write_byte((uint8_t*)103, 'x');	
	
	eeprom_write_byte((uint8_t*)200, 'x');
	eeprom_write_byte((uint8_t*)201, 'x');
	eeprom_write_byte((uint8_t*)202, 'x');
	eeprom_write_byte((uint8_t*)203, 'x');
	
	//send lock system for doors?
}

unsigned char inputCount = 0;
unsigned char passwordInput[4] = {'x', 'x', 'x', 'x'};
unsigned char incorrect_delay_count = 0;
unsigned char keypadEntry = '\0';
unsigned char attempts = 0;
unsigned short lock_delay_count = 0;

enum Menu{setup, welcomeInit, welcomeToggle, prelogin, loginCheck, incorrectDelay, lockedState, mainMenu, childUser};
int Menu_Flow(int state)
{
	//transitions
	switch(state)
	{
		case -1:
			state = setup;
			break;
		
		case setup:
			state = welcomeInit;
			break;
			
		case welcomeInit:
			state = welcomeToggle;
			break;
		
		case welcomeToggle:
			if(keypadEntry == '*'){
				state = prelogin;
				LCD_ClearScreen();
				LCD_DisplayString(1,preLoginTop);
				LCD_DisplayString(17,preLoginBot);
				LCD_Cursor(33);
				keypadEntry = 'x';
			}
			else{
				state = welcomeToggle;
			}
			break;
			
		case prelogin:
				
			if(keypadEntry == '1'){
				//child logic
				LCD_ClearScreen();
				LCD_DisplayString(1,child);
				LCD_Cursor(33);
				keypadEntry = 'x';
			}
			else if(keypadEntry == '2'){
				//parent logic
				state = loginCheck;
				LCD_ClearScreen();
				LCD_DisplayString(1,loginTop);
				LCD_Cursor(33);
				keypadEntry = 'x';
				
			}
			else if(keypadEntry == '#'){
				state = welcomeInit;
			}
			else{
				state = prelogin;
			}
			break;
			
		case loginCheck:

			if(keypadEntry <= '9' && keypadEntry >= '0'){
				passwordInput[inputCount] = keypadEntry;
				LCD_Cursor(inputCount + 17);
				LCD_WriteData(keypadEntry);
				++inputCount;
				keypadEntry = 'x';
			}
			break;
			
		case lockedState:
			if(lock_delay_count == 5){
				LCD_ClearScreen();
				LCD_DisplayString(1, lockMessage);
				LCD_Cursor(33);
			}
			else if(lock_delay_count >= 605){
				lock_delay_count = 0;
				state = welcomeInit;
				LCD_ClearScreen();
				keypadEntry = 'x';
			}
		break;
		
		case incorrectDelay:
			//slight transition delay
			if(incorrect_delay_count == 5){
				LCD_ClearScreen();
				LCD_DisplayString(1, failLogin);
				LCD_Cursor(33);
			}
			else if(incorrect_delay_count >= 20){
				incorrect_delay_count = 0;
				state = loginCheck;
				LCD_ClearScreen();
				LCD_DisplayString(1,loginTop);
				LCD_Cursor(33);
				keypadEntry = 'x';
			}
			break;
			
		case mainMenu:
			break;
		
		default:
			state = -1;
			break;
	}
	
	//actions
	switch(state)
	{
		case -1:
			break;
			
		case setup:
			system_setup();
			break;
		
		case welcomeInit:
			LCD_ClearScreen();
			LCD_DisplayString(1,welcomeTop);
			LCD_DisplayString(17,welcomeBot);
			LCD_Cursor(33);
			break;
			
		case welcomeToggle:
			break;
			
		case prelogin:
			break;
			
		case loginCheck:			
			if(inputCount >= 4){
				//advance
				inputCount = 0;
				retrieve_password();
				for(int i = 0; i < 4; ++i){
					if(passwordInput[i] != password[i]){
						attempts++;
						if(attempts >= 3){
							state = lockedState;	
							attempts = 0;
						}	
						else{					
							state = incorrectDelay;
						}
						break;
					}
				}
				//not sure if needed to test this later
				if(state == loginCheck){
					state = mainMenu;
					attempts = 0;
					LCD_ClearScreen();
					LCD_DisplayString(1,successLogin);
				}
			}			
			
			break;
		
		case lockedState:
			lock_delay_count += 1;
			break;		
		
		case incorrectDelay:
			incorrect_delay_count += 1;
			break;
		
		case mainMenu:
			break;
		
		default:
			break;
	}
	return state;
}

enum Input{toggle, press};
int Keypad_Input(int state)
{
	//transitions
	switch(state)
	{
		case -1:
			state = toggle;
			break;
		
		case toggle:
				PORTD = 0x00;
			if(GetKeypadKey() != '\0'){
				keypadEntry = GetKeypadKey();				
				state = press;	
			}
			else{
				state = toggle;
			}
			break;
			
		case press:
				PORTD = 0xFF;		
			if(GetKeypadKey() != '\0'){
				state = press;
			}
			else{
				state = toggle;
			}
			break;
			
		
		default:
			state = -1;
			break;
	}
	
	//actions
	switch(state)
	{

		case -1:
			break;
		
		case toggle:
			break;
		
		case press:
			break;	
		
		default:
			break;
	}
	return state;
}

#endif