/*
*
* Thanks for using this library! If you like it, please drop me a comment at brupje@gmail.com.
*
* File     : AdvButton.cpp
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
* This file implements the AdvButton class
*
*/

#include "AdvButton.h"
#include "ButtonManager.h"



void AdvButton::init(uint8_t pin,
		void (*OnRepeatKeyPress)(AdvButton*),
		void (*OnKeyDown)(AdvButton*),
		void (*OnKeyUp)(AdvButton*),
		void (*OnKeyClick)(AdvButton*),
		void (*OnKeyLongPress)(AdvButton*),

		buttonMode mode){
	setPin(pin);
	setRepeat(300);
	setStartDelay(1000);
	setOnKeyRepeatPress( (*OnRepeatKeyPress));
	setOnKeyDown( (*OnKeyDown));
	setOnKeyUp( (*OnKeyUp));

	debounceTime = 50;
	clickDelay=400;
	longPressDelay=800;
	ButtonManager::instance()->addButton(this);
	if (mode == btn_Digital){
		pinMode(pin, INPUT_PULLUP);
		digitalWrite(pin,HIGH);
	}
	this->mode = mode;
	lastChange = millis();
	lastState = HIGH;
}

AdvButton::AdvButton(uint8_t pin,void (*OnRepeatKeyPress)(AdvButton*) , unsigned long repeat, unsigned long startDelay, buttonMode mode)
{
	init(pin,OnRepeatKeyPress,NULL,NULL,NULL,NULL,mode);
	
	setRepeat(repeat);
	setStartDelay(startDelay);
	

}

AdvButton::AdvButton(uint8_t pin,
		void (*OnRepeatKeyPress)(AdvButton*),
		void (*OnKeyDown)(AdvButton*),
		void (*OnKeyUp)(AdvButton*),
		buttonMode mode/* = btn_Digital*/)
{
	init(pin,OnRepeatKeyPress,OnKeyDown,OnKeyUp,NULL,NULL,mode);
}


AdvButton::AdvButton(uint8_t pin,
			void (*OnKeyClick)(AdvButton*),
			buttonMode mode/* = btn_Digital*/)
{
	init(pin,NULL,NULL,NULL,OnKeyClick,NULL,mode);
}

AdvButton::AdvButton(uint8_t pin,
		void (*OnKeyClick)(AdvButton*),
		void (*OnKeyLongPress)(AdvButton*),
		buttonMode mode/* = btn_Digital*/)
{
	init(pin,NULL,NULL,NULL,OnKeyClick,OnKeyLongPress,mode);
}


void AdvButton::check()
{
	// determine current pin state
	int cur;
	if (mode == btn_Digital)
		cur = digitalRead(pin);
	else if (mode == btn_Analog)
		cur = analogRead(pin) >200 ? HIGH : LOW;

	/* button state changed */
	if (lastState != cur)
	{
		

		if (millis() - lastChange < debounceTime)
		{
			return;
		}
		lastChange = millis();
		lastState = cur; 
	}

	
	
  	if (cur == LOW)
	{	
		// the button is pressed, but last time we checked, was the button still up?		
		if (startPress ==0)
		{
			/* mark the start time and notify others */
			startPress = millis();
			if (func_keyDown != NULL)
				func_keyDown(this);
		    if (func_keyRepeatPress != NULL)
		        func_keyRepeatPress(this);
		}
		

		/* is repeating enabled? */
		if ((repeat > 0 ) && (func_keyRepeatPress != NULL))
		{
			/* is the startdelay passed? */
			if (millis() >= startPress+startDelay)
			{
				/* is it time for a keypressed call? */
				if ((millis() - prevPres) > repeat)
				{
					prevPres = millis(); 	
					func_keyRepeatPress(this);
				}
				
			}
			else
				prevPres = millis();
		}
		/* is long press enabled? */
		if (func_keyLongPress !=NULL && millis() >= startPress+longPressDelay)
			func_keyLongPress(this);
	}
	else
	{
		prevPres = 0;
		/* the button is released, but was it previously pressed */		
		if (startPress != 0)
		{
			if (func_keyClick !=NULL && millis() <= startPress+clickDelay)
				func_keyClick(this);

			if (func_keyUp != NULL)
				func_keyUp(this);	
			/* reset start time */
			startPress = 0;
		}
	}

}

unsigned long AdvButton::getPressTime()
{
	return millis() - startPress;
}

void AdvButton::setOnKeyRepeatPress(void (*f)(AdvButton*))
{
	func_keyRepeatPress =  (*f);
}

void AdvButton::setOnKeyDown(void (*f)(AdvButton*))
{
	func_keyDown =  (*f);
}

void AdvButton::setOnKeyUp(void (*f)(AdvButton*))
{
	func_keyUp =  (*f);
}

void AdvButton::setOnKeyClick(void (*f)(AdvButton*))
{
	func_keyClick =  (*f);
}

void AdvButton::setOnKeyLongpress(void (*f)(AdvButton*))
{
	func_keyLongPress =  (*f);
}


void AdvButton::setRepeat(unsigned long repeat)
{
	this->repeat = repeat;
}

void AdvButton::setStartDelay(unsigned long startDelay)
{
	this->startDelay = startDelay;
}

void AdvButton::setPin(uint8_t pin)
{
	this->pin = pin;
}
