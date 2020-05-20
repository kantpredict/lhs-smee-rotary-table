/*
*
* Thanks for using this library! If you like it, please drop me a comment at brupje@gmail.com.
*
* File     : AdvEncoder.h
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
* This file defines the AdvEncoder class
*
* Usage: 
* - Include AdvButton.h and ButtonManager.h in your sketch
* - Add a call to ButtonManager::instance()->checkButtons(); in your main loop
* - Declare each button and define the events using a overload of AdvButton ( AdvButton button = AdvButton(<pin>) )
* - Declare the required event functions ( void OnKeyXXX(AdvButton* but) )
* - See the comments below for more help
*/



#if defined(ARDUINO) && ARDUINO >= 100
      #include "Arduino.h"
#else
      #include "WProgram.h"
#endif
#include "AdvButton.h"

#ifndef ADVENCODER_H
#define ADVENCODER_H


class AdvEncoder;

typedef void (*functiontype_enc)(AdvEncoder*);

class AdvEncoder: AdvButton
{

public:

	/*
	This is the second constructor call, use it for other types of buttons
	pin_a	   : Pin of encoder pin A
	pin_b	   : Pin of encoder pin A
	OnValueChanged : Event called when value of the encoder changes
	[max_value  : Maximum value (-1 is infinite)]
	pin        : pin number of the button
    OnKeyPress : Function to call when the keypress event occurs (optional)
 	OnKeyDown  : Function to call when the keydown event occurs (optional) 
	OnKeyUp    : Function to call when the keyup event occurs (optional) 
	mode       : type of button, analog or digital
	*/
	AdvEncoder(uint8_t pin_a,uint8_t pin_b,void (*OnValueChanged)(AdvEncoder*),uint8_t pin,void (*OnKeyPress)(AdvButton*) =NULL);
	AdvEncoder(uint8_t pin_a,uint8_t pin_b,void (*OnValueChanged)(AdvEncoder*),int max_value, uint8_t pin,void (*OnKeyPress)(AdvButton*) =NULL);
	/* 
	Checks the state of the button and triggers events accordingly
	Will be called from the ButtonManager	
	*/
	void check();

	/* 
	Retrieve the amount of milliseconds the button was pressed,  only valid in keyevents
	*/
	unsigned long getPressTime(); 

	/*
	Returns the current value
	*/
	inline int getValue() { return this->value;};
	inline void setValue(int val) { this->value=val; } ;

	/*
	Set the maximum value for the encoder (-1 is infinite)
	*/
	inline void setMaxValue(int max) { this->max_value = max; };
private: 
	/* private variables */
	functiontype_enc func_valueChanged;
  	uint8_t pin_a;
	uint8_t pin_b;
	int rotateryPos;
	int value;
	int ppos;
	int max_value;

	void init(uint8_t pin_a,uint8_t pin_b,void (*OnValueChanged)(AdvEncoder*), int max_value=-1);

	void changeValue(int delta);
};
#endif
