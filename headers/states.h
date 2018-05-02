#ifndef STATES_H
#define STATES_H

#include <avr/eeprom.h>
#include "keypad.h"
#include "lcd.h"
#include "mstrings.h"
#include "memory.h"

unsigned char inputCount = 0;
unsigned char password_Input[4] = {'x', 'x', 'x', 'x'};
unsigned char time_Input[5] = {'x','x',':','x','x'};
unsigned char locker_one_status = 1;
unsigned char locker_two_status = 1;
unsigned char unlockOne = 0;
unsigned char unlockTwo = 0;
unsigned char showTime = 0;
unsigned char keypadEntry = '\0';
unsigned char attempts = 0;

//delays/ counts
unsigned char incorrect_delay_count = 0;
unsigned short lock_delay_count = 0;
unsigned char valid_delay_count = 0;
unsigned char displayCount = 0;
unsigned char timeCountdown = 0;
unsigned char reset_delay_count = 0;
unsigned char unlock1_count = 0;
unsigned char unlock2_count = 0;

//LCD related
unsigned char cursorIndex = 0;
unsigned char cursorPosition = 0;

//in prog flags
unsigned char passwordProg = 0;
unsigned char passwordComplete = 0;
unsigned char passwordCorrect = 0;
unsigned char checkComplete = 0;
unsigned char overwriteComplete = 0;
unsigned char overwriteDelay = 0;

//child user flags
unsigned char childUserWait = 0;
unsigned char childUserCursor = 0;
unsigned char inputPolling = 0;

//manual unlock/ system reset flags
unsigned char systemResetUnlock = 0;
signed char manual_unlock_choice = -1;
unsigned char manual_count = 0;
signed char lock_choice = -1;
unsigned char time_position_count = 0;
unsigned char lockItem_count = 0;

enum inputUser{initialLogin, lockingItem, resettingSystem, resettingPassword, unlockingManually};
enum inputUser currentuser = -1;

enum delayUser{initialDelay, lockingItemDelay, resettingSystemDelay, resettingPasswordDelay, unlockingManuallyDelay};
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
	//add rest of variables
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

void displayTime(long time, unsigned char position){
	
	long hours = 0;
	long mins = 0;
	long sec = 0;

	hours = time / 3600;
	mins = time % 3600;
	sec = mins % 60;
	mins = mins / 60;
	
	LCD_Cursor(4 + (position*16));
	LCD_WriteData((hours / 10) + '0');
	LCD_Cursor(5 + (position*16));
	LCD_WriteData((hours % 10) + '0');
	LCD_Cursor(7 + (position*16));
	LCD_WriteData((mins / 10) + '0');
	LCD_Cursor(8 + (position*16));
	LCD_WriteData((mins % 10) + '0');
	LCD_Cursor(10 + (position*16));
	LCD_WriteData((sec / 10) + '0');
	LCD_Cursor(11 + (position*16));
	LCD_WriteData((sec % 10) + '0');
	LCD_Cursor(33);
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
		  lockedState, validDelay, mainMenu, itemLock, itemLockMenu, itemLockSetTime, itemLockLogic,
		  manualUnlock, manualUnlockPassword, manualUnlockDelay, display, passwordReset,
		  newPasswordInput, systemReset, ResetMessageDelay, childUser};
		  
int Menu_Flow(int state)
{
	unsigned char inputA = ~PINA;
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
				state = childUser;
				showTime = 1;
				LCD_ClearScreen();
				LCD_DisplayString(1, showTimeA);
				LCD_DisplayString(17, showTimeB);
				displayTime(timeASeconds, 0);
				displayTime(timeBSeconds, 1);
				LCD_Cursor(32);
				LCD_WriteData('-');
				LCD_Cursor(16);
				LCD_WriteData('+');
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
			
			
		case childUser:
			if(keypadEntry == 'D' || childUserWait >= 100){
				childUserWait = 0;
				showTime = 0;
				state = prelogin;
				LCD_ClearScreen();
				LCD_DisplayString(1,preLoginTop);
				LCD_DisplayString(17,preLoginBot);
				LCD_Cursor(33);
				keypadEntry = 'x';
				break;
			}
			
			if(keypadEntry == 'C' && childUserCursor < 1){
				childUserCursor += 1;
			}
			else if(keypadEntry == 'A' && childUserCursor > 0){
				childUserCursor -= 1;
			}
			
			//temporary solution 
			//need to fix polling post demo
			
			if(inputPolling >= 1){
				inputPolling = 0;
				if(inputA & 0x04){
					if(childUserCursor == 0){
						if(timeASeconds <= 60){
							timeASeconds = 0;
						}
						else{
							timeASeconds -= 60;
						}
					}
					else{
						if(timeBSeconds <= 60){
							timeBSeconds = 0;
						}
						else{
							timeBSeconds -= 60;
						}
					}
				}
			}
			else{
				inputPolling += 1;
			}
			
			childUserWait += 1;
			
			LCD_Cursor((childUserCursor + 1) * 16);
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
					
				case lockingItemDelay:
					if(incorrect_delay_count == 5){
						LCD_ClearScreen();
						LCD_DisplayString(1, failLogin);
						LCD_Cursor(33);
					}
					else if(incorrect_delay_count >= 20){
						incorrect_delay_count = 0;
						passwordProg = 1;
						state = itemLock;
						currentDelay = -1;
						LCD_ClearScreen();
						LCD_DisplayString(1, loginTop);
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
					if(incorrect_delay_count == 5){
						LCD_ClearScreen();
						LCD_DisplayString(1, failLogin);
						LCD_Cursor(33);
					}
					else if(incorrect_delay_count >= 20){
						incorrect_delay_count = 0;
						passwordProg = 1;
						state = manualUnlockPassword;
						currentDelay = -1;
						LCD_ClearScreen();
						LCD_DisplayString(1,loginTop);
						LCD_Cursor(33);
						keypadEntry = 'x';
					}
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
						state = itemLock;
						LCD_ClearScreen();
						LCD_DisplayString(1, loginTop);
						LCD_Cursor(33);
						passwordProg = 1;
						currentuser = lockingItem;
						break;
						
					case 1:
						state = manualUnlock;
						LCD_ClearScreen();
						LCD_DisplayString(1, mUnlockTop);
						LCD_DisplayString(17, mUnlockBot);
						LCD_Cursor(33);
						break;
						
					case 2:
						state = display;
						showTime = 1;
						LCD_ClearScreen();
						LCD_DisplayString(1, showTimeA);
						LCD_DisplayString(17, showTimeB);
						displayTime(timeASeconds, 0);
						displayTime(timeBSeconds, 1);
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
			
		case itemLock:
			if(!passwordProg && checkComplete && passwordCorrect){
				LCD_ClearScreen();
				LCD_DisplayString(1, newPassword);
				LCD_Cursor(33);
				passwordProg = 0;
				passwordCorrect = 0;
				attempts = 0;
				state = itemLockMenu;
				LCD_ClearScreen();
				LCD_DisplayString(1, mUnlockTop);
				LCD_DisplayString(17, mUnlockBot);
				LCD_Cursor(33);
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
					currentDelay = lockingItemDelay;
				}
			}
			else{
				state = itemLock;
			}
			
			if(dir == menuRet){
				currentuser = -1;
				dir = -1;
				menu_start();
				state = mainMenu;
				LCD_Cursor(33);
			}
			break;
			
		case itemLockMenu:
			if(keypadEntry == '1'){
				state = itemLockSetTime;
				lock_choice = 0;
				LCD_ClearScreen();
				LCD_DisplayString(1, timePromptTop);
				LCD_Cursor(6);
				LCD_WriteData('A');
				LCD_DisplayString(17, timePromptBot);
				LCD_Cursor(17);
				keypadEntry = 'x';
			}
			else if(keypadEntry == '2'){
				state = itemLockSetTime;
				lock_choice = 1;
				LCD_ClearScreen();
				LCD_DisplayString(1, timePromptTop);
				LCD_Cursor(6);
				LCD_WriteData('B');
				LCD_DisplayString(17, timePromptBot);
				LCD_Cursor(17);
				keypadEntry = 'x';
			}
			else if(keypadEntry == 'D'){
				state = mainMenu;
				menu_start();
			}
			else{
				state = itemLockMenu;
			}
			keypadEntry = 'x';
			break;
			
		case itemLockSetTime:
			//confirm case
			if(time_position_count >= 5){
				if(keypadEntry == '*'){
					LCD_ClearScreen();
					LCD_DisplayString(1, lockDelayPrompt);
					LCD_Cursor(6);
					if(lock_choice == 0){
						locker_one_status = 1;
						timeASeconds = 0;
						LCD_WriteData('A');
					}
					else{
						locker_two_status = 1;
						timeBSeconds = 0;
						LCD_WriteData('B');
					}
					LCD_Cursor(33);
					time_position_count = 0;
					state = itemLockLogic;
				}
			}
			break;

		case itemLockLogic:
			if(lockItem_count >= 70){
				if(lock_choice == 0){
					timeASeconds = ((long)(time_Input[4] - '0') + (long)(10 * (time_Input[3] - '0')) + 
					(long)(60 * (time_Input[1] - '0')) + (long)(600 * (time_Input[0] - '0'))) * 60;
					locker_one_status = 1;
				}
				else{
					timeBSeconds = ((long)(time_Input[4] - '0') + (long)(10 * (time_Input[3] - '0')) +
					(long)(60 * (time_Input[1] - '0')) + (long)(600 * (time_Input[0] - '0'))) * 60;
					locker_two_status = 1;
				}	
				lockItem_count = 0;
				lock_choice = -1;
				menu_start();
				state = mainMenu;		
			}
			break;
		
		case manualUnlock:
			if(keypadEntry == '1'){
				state = manualUnlockPassword;
				manual_unlock_choice = 0;
				currentuser = unlockingManually;
				passwordProg = 1;
				LCD_ClearScreen();
				LCD_DisplayString(1,loginTop);
				LCD_Cursor(33);
			}
			else if(keypadEntry == '2'){
				state = manualUnlockPassword;
				manual_unlock_choice = 1;
				currentuser = unlockingManually;
				passwordProg = 1;
				LCD_ClearScreen();
				LCD_DisplayString(1,loginTop);
				LCD_Cursor(33);
			}
			else if(keypadEntry == 'D'){
				state = mainMenu;
				menu_start();
			}
			else{
				state = manualUnlock;
			}
			keypadEntry = 'x';
			break;
			
		case manualUnlockPassword:
		
			if(!passwordProg && checkComplete && passwordCorrect){
				passwordProg = 0;
				passwordCorrect = 0;
				attempts = 0;
				if(manual_unlock_choice == 0){
					timeASeconds = 0;					
				}
				else{
					timeBSeconds = 0;					
				}
				
				state = manualUnlockDelay;

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
					currentDelay = unlockingManuallyDelay;
				}
			}
			else{
				state = manualUnlockPassword;
			}
			
			if(dir == menuRet){
				currentuser = -1;
				dir = -1;
				menu_start();
				state = mainMenu;
				LCD_Cursor(33);
			}
			
			break;
			
		
		case manualUnlockDelay:
		
			if(manual_count == 5){
				//add a delay
				LCD_ClearScreen();
				LCD_DisplayString(1, unlockedManually1);
				LCD_Cursor(6);
				LCD_WriteData('A'+ manual_unlock_choice);
				LCD_DisplayString(8, unlockedManually2);
				LCD_Cursor(33);
				manual_unlock_choice = -1;
			}
			else if(manual_count == 20){
				menu_start();
				state = mainMenu;
				manual_count = 0;
			}
			break;
		
		case display:
			if(displayCount >= 50){
				state = mainMenu;
				menu_start();
				displayCount = 0;
				showTime = 0;
			}
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
				timeASeconds = 0;
				timeBSeconds = 0;
				//might be a problem with the locks here
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
			else if(reset_delay_count == 55){
				state = setup;
				reset_delay_count = 0;
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
		
		case itemLock:
			break;
			
		case itemLockMenu:
			break;	
			
		case itemLockSetTime:
		
			//clear case
			if(keypadEntry == 'D'){
				if(time_position_count == 0){
					state = itemLockMenu;
					LCD_ClearScreen();
					LCD_DisplayString(1, mUnlockTop);
					LCD_DisplayString(17, mUnlockBot);
					LCD_Cursor(33);
					keypadEntry = 'x';
					break;
				}
				else if(time_position_count >= 5){
					for(unsigned char i = 7; i < 15; ++i){
						LCD_Cursor(17 + i);
						LCD_WriteData(' ');
					}
					time_position_count -= 1;
				}
				else{
					if(time_position_count == 3){
						time_Input[time_position_count] = '#';
						LCD_Cursor(time_position_count + 17);
						LCD_WriteData('#');
						time_position_count -= 2;
					}
					else{
						time_Input[time_position_count] = '#';
						LCD_Cursor(time_position_count + 17);
						LCD_WriteData('#');
						time_position_count -= 1;
					}
				}
				LCD_Cursor(17 + time_position_count);
			}
		
			//confirm case
			if(time_position_count >= 5){
				LCD_DisplayString(24, timeComfirm);
				LCD_Cursor(33);
			}
			//enter case
			else{
				if(keypadEntry >= '0' && keypadEntry <= '9'){
					//cases for mins
					if(time_position_count == 1){
						LCD_Cursor(time_position_count + 17);
						time_Input[time_position_count] = keypadEntry;
						LCD_WriteData(keypadEntry);
						time_position_count += 2;
					}
					else if(time_position_count == 3){
						if(keypadEntry <= '5'){
							LCD_Cursor(time_position_count + 17);
							time_Input[time_position_count] = keypadEntry;
							LCD_WriteData(keypadEntry);
							time_position_count += 1;
						}
					}
					else{
						LCD_Cursor(time_position_count + 17);
						time_Input[time_position_count] = keypadEntry;
						LCD_WriteData(keypadEntry);
						//write to array
						time_position_count += 1;
					}
					LCD_Cursor(17 + time_position_count);
				}		
			}
			
			keypadEntry = 'x';

			break;
			
		case itemLockLogic:
			lockItem_count += 1;
			break;
			
		case manualUnlock:
			break;
			
		case manualUnlockPassword:
			break;
			
		case manualUnlockDelay:
			manual_count += 1;
			break;
			
		case display:
			displayCount += 1;
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
						dir = menuRet;
						break;
						
					case lockingItem:
						dir = menuRet;
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

enum TimerCount{updateTime, timerDone};
int Timer_Status(int state)
{
	
	//transitions
	switch(state)
	{
		case -1:
			state = updateTime;
			break;
		
		case updateTime:
			
			if(timeCountdown >= 10){
				if(locker_one_status){
					if(timeASeconds <= 0){
						timeASeconds = 0;
						unlockOne = 1;
						PORTA = PORTA | 0x01;
						state = timerDone;
					}
					else{
						timeASeconds -= 1;
					}
				}
				
				if(locker_two_status){
					if(timeBSeconds <= 0){
						timeBSeconds = 0;
						unlockTwo = 1;
						PORTA = PORTA | 0x02;
						state = timerDone;
					}
					else{
						timeBSeconds -= 1;
					}
				}
				
				if(showTime){
					displayTime(timeASeconds, 0);
					displayTime(timeBSeconds, 1);
				}
				timeCountdown = 0;
			}
			
			timeCountdown += 1;
			
			break;
		
		case timerDone:
			//add a 3 second delay here
			//send via USART here
			if(unlockOne){
				if(unlock1_count >= 40){
					PORTA = PORTA & 0xFE;
					unlockOne = 0;
					unlock1_count = 0;
					locker_one_status = 0;
					state = updateTime;
				}
				else{
					unlock1_count += 1;
					state = timerDone;
				}
			}
			
			if(unlockTwo){
				if(unlock2_count >= 40){
					PORTA = PORTA & 0xFD;
					unlockTwo = 0;
					unlock2_count = 0;
					locker_two_status = 0;
					state = updateTime;
				}
				else{
					unlock2_count += 1;
					state = timerDone;
				}

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
		
		case updateTime:
			break;
			
		case timerDone:
			break;
			
		default:
			break;
	}
	return state;
}


#endif