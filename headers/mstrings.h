#ifndef MSTRINGS_H
#define MSTRINGS_H

const unsigned char * welcomeTop = "Welcome! :)";
const unsigned char * welcomeBot = "Hit * to start";

const unsigned char * preLoginTop = "1:Child 2:Parent";
const unsigned char * preLoginBot = "Hit < to quit";

//child path


//parent path
const unsigned char * loginTop = "Enter password:";
const unsigned char * failLogin = "Try again";
const unsigned char * successLogin = "Correct login!";
const unsigned char * lockMessage = "Locked for 1 min";

//menu strings
const unsigned char * menuOptions[6] = {"Lock Item", "Manual Unlock","Status Display", 
							            "Password Reset", "System Reset", "Logout"};

//Lock Item

//Manual Unlock

//Display Time

//Password Reset
const unsigned char * newPassword = "New password:";
const unsigned char * successfulChangeTop = "Change complete!";
const unsigned char * successfulChangeBot = "Password: ";
const unsigned char * resetComplete = "Password Reset!";
//System Reset
const unsigned char * systemResetTop = "Confirm Reset:";
const unsigned char * systemResetComplete = "Resetting System";

//test
const unsigned char * child = "Kids";
const unsigned char * parent = "Parents";
#endif