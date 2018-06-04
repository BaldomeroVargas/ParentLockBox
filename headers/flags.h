#ifndef FLAGS_H
#define FLAGS_H

unsigned char password_Input[4] = {'x', 'x', 'x', 'x'};
unsigned char time_Input[5] = {'x','x',':','x','x'};

//locker related
unsigned char locker_one_status = 1;
unsigned char locker_two_status = 1;
unsigned char unlockOne = 0;
unsigned char unlockTwo = 0;
unsigned char showTime = 0;
unsigned char keypadEntry = '\0';
unsigned char attempts = 0;

//delays counts
unsigned char inputCount = 0;
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
unsigned char coinDetect = 0;

//manual unlock/system reset flags
unsigned char systemResetUnlock = 0;
signed char manual_unlock_choice = -1;
unsigned char manual_count = 0;
signed char lock_choice = -1;
unsigned char time_position_count = 0;
unsigned char lockItem_count = 0;

//used to return to proper state after password input
enum inputUser{initialLogin, lockingItem, resettingSystem, resettingPassword, unlockingManually};
enum inputUser currentuser = -1;

//used to map delay messages
enum delayUser{initialDelay, lockingItemDelay, resettingSystemDelay, resettingPasswordDelay, unlockingManuallyDelay};
enum delayUser currentDelay = -1;

//used to map return
enum returnDirection{userPromptRet, menuRet};
enum returnDirection dir = -1;

#endif