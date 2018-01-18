#ifndef _DHBUTTON_h
#define _DHBUTTON_h

#include "../DHBB_Options.h"
#include "../DHBB_Types.h"
#include "arduino.h"
#include <Bounce2.h>

namespace Controls {
	class DHButton {
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
		DHButton(USHORT pin);
		~DHButton();

		//Check button status
		void CheckForPress();
		inline bool IsPressed() { return is_pressed_; }
		inline bool JustPressed() { return just_pressed_; }
		inline bool JustReleased() { return just_released_; }
	};
}
#endif

