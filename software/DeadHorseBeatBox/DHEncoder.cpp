// 
// 
// 

#include "DHEncoder.h"

namespace Controls {
	DHEncoder::DHEncoder() { }

	DHEncoder::~DHEncoder() { }

	void DHEncoder::Update() {
		button_.CheckForPress();

		//Zero out the encoder every time through
		value_ = 0;
		long current_position = encoder_.read();

		//If we have enough movement of the encoder then change the value
		if (current_position >= scale_ || current_position <= -scale_) {
			value_ = current_position / scale_;
			encoder_.write(0);
		}
	}
}