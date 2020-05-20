/*
*
* Thanks for using this library! If you like it, please drop me a comment at brupje@gmail.com.
*
* File     : AdvEncoder.cpp
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
* This file implements the AdvEncoder class
*
*/

#include "AdvButton.h"
#include "ButtonManager.h"
#include "AdvEncoder.h"



AdvEncoder::AdvEncoder(uint8_t pin_a,uint8_t pin_b,void (*OnValueChanged)(AdvEncoder*),uint8_t pin,void (*OnKeyPress)(AdvButton*) ): AdvButton(pin,OnKeyPress,300,500,btn_Digital) 
{
	AdvEncoder::init(pin_a,pin_b,OnValueChanged);
}


AdvEncoder::AdvEncoder(uint8_t pin_a,uint8_t pin_b,void (*OnValueChanged)(AdvEncoder*),int max_value, uint8_t pin,void (*OnKeyPress)(AdvButton*) ): AdvButton(pin,OnKeyPress,300,500,btn_Digital) 
{
	AdvEncoder::init(pin_a,pin_b,OnValueChanged,max_value);
}


void AdvEncoder::check()
{
	AdvButton::check();

	int enc_a = digitalRead(pin_a);
	int enc_b = digitalRead(pin_b);

	int newpos;

	if ((enc_a==LOW) && (enc_b==LOW)) {
		newpos = 0;
	}
	else if ((enc_a==LOW) && (enc_b==HIGH)) {
		newpos = 1;
		this->ppos=1;
	}
	else if ((enc_a==HIGH) && (enc_b==HIGH)) 
	{
		newpos = 2;
	}
	else if ((enc_a==HIGH) && (enc_b==LOW)) {
		newpos = 3; 
		this->ppos=3;
	}
   
	if (newpos == 2) {
		if (this->ppos==1) {
			changeValue(1);
		}
		else if (this->ppos==3) {
	 		changeValue(-1);
		}
		this->ppos=0;

	}
	else if (newpos == 0) {
		if (this->ppos==1) {
			changeValue(-1);
		}
		else if (this->ppos==3) {
	  		changeValue(1);
			
		}
		this->ppos=0;

	}
	  
	this->rotateryPos = newpos;

}

void AdvEncoder::changeValue(int delta) {
	value+=delta;
	if (max_value==-1) {
		if (value<0) 
			value=0;
	}
	else {
		if (value<0) 
			value = max_value;
		if (value > max_value)
			value = 0;
	} 

	if (func_valueChanged != NULL) 
		func_valueChanged(this);
}

void AdvEncoder::init(uint8_t pin_a,uint8_t pin_b,void (*OnValueChanged)(AdvEncoder*),int max_value) {
	this->pin_a = pin_a;
	this->pin_b = pin_b;
	this->value=0;
	this->max_value = max_value;
	this->func_valueChanged = *OnValueChanged;

	if (getMode() == btn_Digital){
		pinMode(pin_a, INPUT_PULLUP);
		digitalWrite(pin_a,HIGH);
		pinMode(pin_b, INPUT_PULLUP);
		digitalWrite(pin_b,HIGH);
	}
}
