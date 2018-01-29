#include "Button.h"

namespace Controls 
{
	Button::Button(USHORT pin) 
	{
		pin_ = pin;
		pinMode(pin_, INPUT);
		debouncer_.attach(pin_);
		debouncer_.interval(5);
	}

	Button::~Button() {}

	void Button::CheckForPress() 
	{
		debouncer_.update();
		bool current_state = debouncer_.read();
		just_pressed_ = (previous_state_ == kButtonNotPressed && current_state == kButtonPressed);
		just_released_ = (previous_state_ == kButtonPressed && current_state == kButtonNotPressed);
		is_pressed_ = (current_state == kButtonPressed);
		previous_state_ = current_state;
	}
}