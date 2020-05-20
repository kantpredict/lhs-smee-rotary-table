/*
*
* Thanks for using this library! If you like it, please drop me a comment at brupje@gmail.com.
*
* File     : ButtonManager.cpp
* Version  : 1.4
* Released : 24/09/2016
* Author   : Bart Meijer (brupje@gmail.com)
*
* This is the Advanced Button library for the Arduino platform. It will enable you to easily 
* react to user input using simple press-to-make buttons. Features include:
* - Event based implementation 
* - Recording the time a button is pressed
* - Adjustable repeat delay, start delay for the keypressed event
* - requires only a single call in the main loop
*
* This file implements the ButtonManager class
*
*/

#include "ButtonManager.h"

ButtonManager *ButtonManager::s_instance = 0;



ButtonManager::ButtonManager()
{
	numButtons =0;
}


ButtonManager *ButtonManager::instance()
{
	if (!s_instance)
		s_instance = new ButtonManager();
	return s_instance;
}

void ButtonManager::addButton(AdvButton* but)
{
	if (numButtons+1 < MAXBUTTONS)
	{
		numButtons++;
		buttons[numButtons-1]= but;
	}
}	


void ButtonManager::checkButtons()
{
	for (int i=0; i< numButtons; i++)
		buttons[i]->check(); 		
}	
