/*
*
* Thanks for using this library! If you like it, please drop me a comment at brupje@gmail.com.
*
* File     : AdvButton.h
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
* This file defines the AdvButton class
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


#ifndef ADVBUTTON_H
#define ADVBUTTON_H

class AdvButton;

typedef void (*functiontype)(AdvButton*);


enum buttonMode {btn_Digital, btn_Analog}; 


class AdvButton
{

public:


	/*
	This is the first constructor call, use it for keypress buttons 
	pin        : pin number of the button
    OnKeyPress : Function to call when the event occurs
 	repeat     : time between the event is raised while pressing the button (optional)
	startDelay : amount of time between the initial keypress event and when to start repeating (optional)
	mode       : type of button, analog or digital
	*/
	AdvButton(uint8_t pin,
			void (*OnRepeatKeyPress)(AdvButton*),
			unsigned long repeat = 300, unsigned long startDelay = 500, buttonMode mode = btn_Digital);

	/*
	This is the second constructor call, use it for other types of buttons
	pin        : pin number of the button
    OnKeyPress : Function to call when the keypress event occurs (optional)
 	OnKeyDown  : Function to call when the keydown event occurs (optional) 
	OnKeyUp    : Function to call when the keyup event occurs (optional) 
	mode       : type of button, analog or digital
	*/
	AdvButton(uint8_t pin,
			void (*OnRepeatKeyPress)(AdvButton*) =NULL,
			void (*OnKeyDown)(AdvButton*)=NULL,
			void (*OnKeyUp)(AdvButton*)=NULL,
			buttonMode mode = btn_Digital);


	/*
		This is the third constructor call, use it for other types of buttons
		pin        : pin number of the button
	    OnKeyClick : Function to call when the keyclick event occurs
		mode       : type of button, analog or digital
		*/
	AdvButton(uint8_t pin,
				void (*OnKeyClick)(AdvButton*) =NULL,
				buttonMode mode = btn_Digital);


	/*
			This is the third constructor call, use it for other types of buttons
			pin        : pin number of the button
		    OnKeyClick : Function to call when the keyclick event occurs
		    OnKeyLongpress : Function to call when the long press event occurs
			mode       : type of button, analog or digital
			*/
	AdvButton(uint8_t pin,
				void (*OnKeyClick)(AdvButton*) =NULL,
				void (*OnKeyLongPress)(AdvButton*) =NULL,
				buttonMode mode = btn_Digital);

	/* 
	Checks the state of the button and triggers events accordingly
	Will be called from the ButtonManager	
	*/
	virtual void check();

	/* 
	This function will set the function to call when the keypress event occurs
	*/
	void setOnKeyRepeatPress(void (*f)(AdvButton*));

	/* 
	This function will set the function to call when the keydown event occurs
	*/
	void setOnKeyDown(void (*f)(AdvButton*));

	/* 
	This function will set the function to call when the keyup event occurs
	*/
	void setOnKeyUp(void (*f)(AdvButton*));

	/*
	This function will set the function to call when the key click event occurs
	*/
	void setOnKeyClick(void (*f)(AdvButton*));

	/* 
	This function will set the function to call when the key longpress event occurs
	*/
	void setOnKeyLongpress(void (*f)(AdvButton*));

	/*
	Sets the time (milliseconds) between each repeated keypress event
	*/
	void setRepeat(unsigned long repeat);

	/* 
	Sets the delay (milliseconds) before the keypress event is repeated
	*/
	void setStartDelay(unsigned long startDelay);

	/* 
	Changes the pin number 
	*/
	void setPin(uint8_t pin);
	
	/* 
	Retrieve the pin number 
	*/
	uint8_t getPin() { return pin; };
	
	/* 
	Changes the button id
	*/
	void setID(uint8_t id) { this->id = id; } ;
	
	/* 
	Retrieve the button id
	*/
	uint8_t getID() { return id; };
	
	
	/* 
	Retrieve the amount of milliseconds the button was pressed,  only valid in keyevents
	*/
	unsigned long getPressTime(); 

	/*
	Return the button mode
	*/
	inline buttonMode getMode() { return mode; };

private: 
	/* private variables */
	functiontype func_keyUp;
	functiontype func_keyDown;
	functiontype func_keyRepeatPress;
	functiontype func_keyLongPress;
	functiontype func_keyClick;
  	uint8_t pin;
  	uint8_t id;
	buttonMode mode;
	int debounceTime;
    unsigned long repeat;
	unsigned long startDelay;
	unsigned long clickDelay;
	unsigned long longPressDelay;
	unsigned long startPress;
	unsigned long lastChange;
	int lastState=HIGH;
	unsigned long prevPres;

	void init(uint8_t pin,void (*OnKeyPress)(AdvButton*) , void (*OnKeyDown)(AdvButton*),void (*OnKeyUp)(AdvButton*),void (*OnKeyClick)(AdvButton*), void (*OnKeyLongPress)(AdvButton*), buttonMode mode);
};
#endif
