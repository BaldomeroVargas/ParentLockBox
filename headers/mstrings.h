#ifndef MSTRINGS_H
#define MSTRINGS_H

const unsigned char * welcomeTop = "Welcome! :)";
const unsigned char * welcomeBot = "Hit * to start";

const unsigned char * preLoginTop = "1:Child 2:Parent";
const unsigned char * preLoginBot = "Hit < to quit";

//parent path
const unsigned char * loginTop = "Enter password:";
const unsigned char * failLogin = "Try again";
const unsigned char * successLogin = "Correct login!";
const unsigned char * lockMessage = "Locked for 1 min";

//menu strings
const unsigned char * menuOptions[6] = {"Lock Item", "Manual Unlock","Status Display", 
							            "Password Reset", "System Reset", "Logout"};

//Password Reset
const unsigned char * newPassword = "New password:";
const unsigned char * successfulChangeTop = "Change complete!";
const unsigned char * successfulChangeBot = "Password: ";
const unsigned char * resetComplete = "Password Reset!";

//System Reset
const unsigned char * systemResetTop = "Confirm Reset:";
const unsigned char * systemResetComplete = "Resetting System";

//display
const unsigned char * showTimeA = "A: ";
const unsigned char * showTimeB = "B: ";

//manual unlock
const unsigned char * mUnlockTop = "1: Lock A";
const unsigned char * mUnlockBot = "2: Lock B";
const unsigned char * unlockedManually1 = "Lock ";
const unsigned char * unlockedManually2 = "Open!";

//locking item
const unsigned char * timePromptBot = "HH:MM";
const unsigned char * lockDelayPrompt = "Lock x Open";
//overwrite position 6 with locker selection
const unsigned char * timePromptTop = "Lock X Time";
//start at char position 7
const unsigned char * timeComfirm = "* to Set";

//test
const unsigned char * child = "Kids";
const unsigned char * parent = "Parents";
const unsigned char * logic = "Logic";
#endif