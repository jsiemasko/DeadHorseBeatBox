// Button.h

#ifndef _BUTTON_h
#define _BUTTON_h

#include "DHBB_Options.h"
#include "arduino.h"
#include <Bounce2.h>

typedef unsigned short int USHORT;
typedef unsigned int ULONG;
typedef unsigned int UINT;

class Button
{
 protected:
	 //Button state constants
	 static const bool kButtonPressed = false;
	 static const bool kButtonNotPressed = true;
	 
	 //Pin numbers
	 USHORT pin_ = 0;

	 //Button state
	 bool is_pressed_ = false;
	 bool just_pressed_ = false;
	 bool just_released_ = false;

	 //Bouncer is used for button debouncing
	 Bounce debouncer_ = Bounce();

	 //Previous state is used to determine if the button has changed states and a button press or release event has happened
	 bool previous_state_ = kButtonNotPressed;
	 
 public:
	//Constructor
	Button(USHORT pin);
	~Button();
	
	//Check button status
	void CheckForPress();
	inline bool IsPressed() { return is_pressed_; }
	inline bool JustPressed() { return just_pressed_; }
	inline bool JustReleased() { return just_released_; }
};

#endif

