#ifndef STATES_H
#define STATES_H

#include <avr/eeprom.h>
#include "keypad.h"
#include "lcd.h"
#include "mstrings.h"
#include "memory.h"

unsigned char inputCount = 0;
unsigned char password_Input[4] = {'x', 'x', 'x', 'x'};
unsigned char locker_one_status = 0;
unsigned char locker_two_status = 0;
unsigned char incorrect_delay_count = 0;
unsigned short lock_delay_count = 0;
unsigned char valid_delay_count = 0;
unsigned char keypadEntry = '\0';
unsigned char attempts = 0;

unsigned char cursorIndex = 0;
unsigned char cursorPosition = 0;

//in prog flags
unsigned char passwordProg = 0;
unsigned char passwordComplete = 0;
unsigned char passwordCorrect = 0;
unsigned char checkComplete = 0;

unsigned char overwriteComplete = 0;
unsigned char overwriteDelay = 0;


//for unlock later
unsigned char systemResetUnlock = 0;
unsigned char reset_delay_count = 0;

//menu screen
//LCD_Cursor(((cursorIndex % 2) + 1) * 16);

enum inputUser{initialLogin, resettingSystem, resettingPassword, unlockingManually};
enum inputUser currentuser = -1;

enum delayUser{initialDelay, resettingSystemDelay, resettingPasswordDelay, unlockingManuallyDelay};
enum delayUser currentDelay = -1;

enum returnDirection{userPromptRet, menuRet};
enum returnDirection dir = -1;

void welcome_reset(){
	passwordProg = 0;
	passwordComplete = 0;
	passwordCorrect = 0;
	checkComplete = 0;

	overwriteComplete = 0;
	overwriteDelay = 0;
	attempts = 0;

	cursorIndex = 0;
	cursorPosition = 0;
	incorrect_delay_count = 0;
	lock_delay_count = 0;
	valid_delay_count = 0;
	inputCount = 0;
	dir = -1;
	currentDelay = -1;
	currentuser = -1;
}

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
	eeprom_write_byte((uint8_t*)104, 'x');
	eeprom_write_byte((uint8_t*)105, 'x');
	
	eeprom_write_byte((uint8_t*)200, 'x');
	eeprom_write_byte((uint8_t*)201, 'x');
	eeprom_write_byte((uint8_t*)202, 'x');
	eeprom_write_byte((uint8_t*)203, 'x');
	eeprom_write_byte((uint8_t*)204, 'x');
	eeprom_write_byte((uint8_t*)205, 'x');
}

void menu_start(){
	valid_delay_count = 0;
	LCD_ClearScreen();
	LCD_DisplayString(1,menuOptions[0]);
	LCD_DisplayString(17,menuOptions[1]);
	LCD_Cursor(16);
	LCD_WriteData('+');
	LCD_Cursor(32);
	LCD_WriteData('-');
	cursorIndex = 0;
	cursorPosition = 0;
	LCD_Cursor(((cursorIndex % 2) + 1) * 16);
	keypadEntry = 'x';	
}

enum Menu{setup, welcomeInit, welcomeToggle, prelogin, loginCheck, incorrectDelay, 
		  lockedState, validDelay, mainMenu, lockItem, manualUnlock, display, passwordReset,
		  newPasswordInput, systemReset, ResetMessageDelay, childUser};
		  
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
			welcome_reset();
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
				passwordProg = 1;
				currentuser = initialLogin;
				
			}
			else if(keypadEntry == 'D'){
				state = welcomeInit;
			}
			else{
				state = prelogin;
			}
			break;
			
		case loginCheck:
		
			if(!passwordProg && checkComplete && passwordCorrect){
				state = validDelay;
				passwordProg = 0;
				passwordCorrect = 0;
				attempts = 0;
			}
			else if(!passwordProg && checkComplete && !passwordCorrect){
				attempts += 1;
				passwordProg = 0;
				passwordCorrect = 0;
				if(attempts >= 3){
					state = lockedState;
					currentuser = -1;
					dir = -1;
					attempts = 0;
				}
				else{
					currentDelay = initialDelay;
					state = incorrectDelay;
				}
			}
			else{
				state = loginCheck;
			}
			
			if(dir == userPromptRet){
				currentuser = -1;
				dir = -1;
				LCD_DisplayString(1,preLoginTop);
				LCD_DisplayString(17,preLoginBot);
				LCD_Cursor(33);
				state = prelogin;
			}
			
			break;
			
		case lockedState:
			//locks system for 1 min (for now)
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
		
			switch(currentDelay){
				case -1:
					break;
					
				case initialDelay:
					if(incorrect_delay_count == 5){
						LCD_ClearScreen();
						LCD_DisplayString(1, failLogin);
						LCD_Cursor(33);
					}
					else if(incorrect_delay_count >= 20){
						incorrect_delay_count = 0;
						passwordProg = 1;
						state = loginCheck;
						currentDelay = -1;
						LCD_ClearScreen();
						LCD_DisplayString(1,loginTop);
						LCD_Cursor(33);
						keypadEntry = 'x';
					}
					break;
					
				case resettingSystemDelay:
					if(incorrect_delay_count == 5){
						LCD_ClearScreen();
						LCD_DisplayString(1, failLogin);
						LCD_Cursor(33);
					}
					else if(incorrect_delay_count >= 20){
						incorrect_delay_count = 0;
						passwordProg = 1;
						state = systemReset;
						currentDelay = -1;
						LCD_ClearScreen();
						LCD_DisplayString(1, systemResetTop);
						LCD_Cursor(33);
						keypadEntry = 'x';
					}
					break;
					
				case resettingPasswordDelay:
					if(incorrect_delay_count == 5){
						LCD_ClearScreen();
						LCD_DisplayString(1, failLogin);
						LCD_Cursor(33);
					}
					else if(incorrect_delay_count >= 20){
						incorrect_delay_count = 0;
						passwordProg = 1;
						state = passwordReset;
						currentDelay = -1;
						LCD_ClearScreen();
						LCD_DisplayString(1,loginTop);
						LCD_Cursor(33);
						keypadEntry = 'x';
					}
					break;
					
				case unlockingManuallyDelay:
					break;
					
				default:
					break;
			}
			//slight transition delay
			break;
			
		case validDelay:
			//slight transition delay
			if(valid_delay_count == 5){
				LCD_ClearScreen();
				LCD_DisplayString(1,successLogin);
				LCD_Cursor(33);
			}
			else if(valid_delay_count >= 20){
				menu_start();	
				state = mainMenu;
				valid_delay_count = 0;
			}
			break;
			
		case mainMenu:
			if(keypadEntry == 'B'){
				keypadEntry = 'x';
				switch (cursorIndex)
				{
					case 0:
						state = lockItem;
						LCD_ClearScreen();
						LCD_Cursor(1);
						LCD_WriteData(cursorIndex + 48);
						break;
						
					case 1:
						state = manualUnlock;
						LCD_ClearScreen();
						LCD_Cursor(1);
						LCD_WriteData(cursorIndex + 48);
						break;
						
					case 2:
						state = display;
						LCD_ClearScreen();
						LCD_Cursor(1);
						LCD_WriteData(cursorIndex + 48);
						break;
						
					case 3:
						state = passwordReset;
						LCD_ClearScreen();
						LCD_DisplayString(1, loginTop);
						LCD_Cursor(33);
						passwordProg = 1;
						currentuser = resettingPassword;
						break;
						
					case 4:
						state = systemReset;
						passwordProg = 1;
						currentuser = resettingSystem;
						LCD_ClearScreen();
						LCD_DisplayString(1, systemResetTop);
						LCD_Cursor(33);
						break;
					
					case 5:
						state = welcomeInit;
						break;
				}
				cursorIndex = 0;
				cursorPosition = 0;
			}
			break;
			
		case lockItem:
			break;
		
		case manualUnlock:
			break;
		
		case display:
			break;
		
		case passwordReset:
			if(!passwordProg && checkComplete && passwordCorrect){
				LCD_ClearScreen();
				LCD_DisplayString(1, newPassword);
				LCD_Cursor(33);
				passwordProg = 0;
				passwordCorrect = 0;
				attempts = 0;
				state = newPasswordInput;
			}
			else if(!passwordProg && checkComplete && !passwordCorrect){
				attempts += 1;
				passwordProg = 0;
				passwordCorrect = 0;
				if(attempts >= 3){
					state = mainMenu;
					currentuser = -1;
					dir = -1;
					menu_start();
					attempts = 0;
				}
				else{
					state = incorrectDelay;
					currentDelay = resettingPasswordDelay;
				}
			}
			else{
				state = passwordReset;
			}
			
			if(dir == menuRet){
				currentuser = -1;
				dir = -1;
				menu_start();	
				state = mainMenu;
				LCD_Cursor(33);
			}
			break;
		
		case newPasswordInput:
			if(overwriteComplete){
				if(overwriteDelay >= 30){
					overwriteDelay = 0;
					overwriteComplete = 0;
					menu_start();
					state = mainMenu;
				}
			}
			break;
		
		case systemReset:
			if(!passwordProg && checkComplete && passwordCorrect){
				passwordProg = 0;
				passwordCorrect = 0;
				attempts = 0;
				state = ResetMessageDelay;

			}
			else if(!passwordProg && checkComplete && !passwordCorrect){
				attempts += 1;
				passwordProg = 0;
				passwordCorrect = 0;
				if(attempts >= 3){
					state = mainMenu;
					currentuser = -1;
					dir = -1;
					menu_start();
					attempts = 0;
				}
				else{
					state = incorrectDelay;
					currentDelay = resettingSystemDelay;
				}
			}
			else{
				state = systemReset;
			}
			
			if(dir == menuRet){
				currentuser = -1;
				dir = -1;
				menu_start();
				state = mainMenu;
				LCD_Cursor(33);
			}
			break;
		
		case ResetMessageDelay:
		
			if(reset_delay_count == 5){
				LCD_ClearScreen();
				LCD_DisplayString(1, systemResetComplete);
				LCD_Cursor(33);
			}
			else if(reset_delay_count == 20){
				state = setup;
				reset_delay_count = 0;
			}
			break;		
		
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
			break;
		
		case lockedState:
			lock_delay_count += 1;
			break;		
		
		case incorrectDelay:
			incorrect_delay_count += 1;
			break;
			
		case validDelay:
			valid_delay_count += 1;
			break;
		
		case mainMenu:
			PORTD = (cursorPosition << 5) | cursorIndex;
			if(keypadEntry == 'A'){
				if(cursorIndex > 0){
					if(cursorPosition == 0){
						cursorPosition = 1;
						LCD_ClearScreen();
						LCD_DisplayString(1,menuOptions[cursorIndex - 1]);
						LCD_DisplayString(17,menuOptions[cursorIndex]);		
						LCD_Cursor(16);
						LCD_WriteData('+');
						LCD_Cursor(32);
						LCD_WriteData('-');
					}
					else{
						cursorPosition = 0;
						cursorIndex -= 1;
					}
				}
				keypadEntry = 'x';
			}
			else if(keypadEntry == 'C'){
				if(cursorIndex < 5){
					if(cursorPosition == 1){
						cursorPosition = 0;
						LCD_ClearScreen();
						LCD_DisplayString(1,menuOptions[cursorIndex]);
						LCD_DisplayString(17,menuOptions[cursorIndex + 1]);
						LCD_Cursor(16);
						LCD_WriteData('+');
						LCD_Cursor(32);
						LCD_WriteData('-');
					}
					else{
						cursorPosition = 1;	
						cursorIndex += 1;					
					}
				}
				keypadEntry = 'x';
			}
			LCD_Cursor(((cursorPosition % 2) + 1) * 16);
			break;
		
		case lockItem:
			break;
			
		case manualUnlock:
			break;
			
		case display:
			break;
			
		case passwordReset:
			break;
			
		case newPasswordInput:
			if(!overwriteComplete){
				if(keypadEntry <= '9' && keypadEntry >= '0'){
					password[inputCount] = keypadEntry;
					LCD_Cursor(inputCount + 17);
					LCD_WriteData(keypadEntry);
					++inputCount;
					keypadEntry = 'x';
				}
				if(inputCount >= 4){
					overwrite_password();
					overwriteComplete = 1;
					LCD_ClearScreen();
					LCD_DisplayString(1, successfulChangeTop);
					LCD_Cursor(17);
					LCD_WriteData(password[0]);
					LCD_Cursor(18);
					LCD_WriteData(password[1]);
					LCD_Cursor(19);
					LCD_WriteData(password[2]);
					LCD_Cursor(20);
					LCD_WriteData(password[3]);
				}			
			}
			else{
				overwriteDelay++;
			}
			break;
		
		case systemReset:
			break;
		
		case ResetMessageDelay:
			reset_delay_count += 1;
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
			if(GetKeypadKey() != '\0'){
				keypadEntry = GetKeypadKey();				
				state = press;	
			}
			else{
				state = toggle;
			}
			break;
			
		case press:	
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

enum PasswordCheck{idle, inputWait, passwordVerify};
int Password_Verify(int state)
{
	//transitions
	switch(state)
	{
		case -1:
			state = idle;
			break;
		
		case idle:
			if(passwordProg){
				state = inputWait;
			}
			else{
				state = idle;
			}
			break;
		
		case inputWait:
			if(passwordComplete){
				passwordProg = 0;
				passwordComplete = 0;
				state = passwordVerify;
			}
			else{
				state = inputWait;
			}
			break;				
		
		case passwordVerify:
			if(checkComplete){
				checkComplete = 0;
				state = idle;
			}
			else{
				state = passwordVerify;
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
		
		case idle:
			break;
		
		case inputWait:
			if(keypadEntry <= '9' && keypadEntry >= '0'){
				password_Input[inputCount] = keypadEntry;
				LCD_Cursor(inputCount + 17);
				LCD_WriteData(keypadEntry);
				++inputCount;
				keypadEntry = 'x';
			}
			else if(keypadEntry == 'D'){
				//handle all cases where this is used
				inputCount = 0;
				passwordProg = 0;
				state = idle;
				LCD_ClearScreen();
				keypadEntry = 'x';
				switch (currentuser)
				{
					case initialLogin:
						dir = userPromptRet;
						break;
						
					case resettingSystem:
						dir = menuRet;
						break;
						
					case resettingPassword:
						dir = menuRet;
						break;
						
					case unlockingManually:
						//TODO
						break;
						
					default:
						//TODO
						break;
				}
			}
			
			if(inputCount >= 4){
				inputCount = 0;
				retrieve_password();
				passwordComplete = 1;
			}
			break;
		
		case passwordVerify:
			for(int i = 0; i < 4; ++i){
				if(password_Input[i] != password[i]){
					passwordCorrect = 0;
					break;
				}
				else{
					passwordCorrect = 1;
				}
			}
			checkComplete = 1;
			break;
		
		default:
		break;
	}
	return state;
}

#endif