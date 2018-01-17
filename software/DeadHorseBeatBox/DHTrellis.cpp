#include "DHTrellis.h"

namespace Controls {
	DHTrellis::DHTrellis() { 
		trellis_.begin(TRELLIS_ADDRESS_1, TRELLIS_ADDRESS_2);
		trellis_.setBrightness(TRELLIS_BRIGHTNESS);
		ClearBuffer();
		trellis_.writeDisplay();
	}

	DHTrellis::~DHTrellis() { }

	void DHTrellis::ReadSwitches(){
		//TODO:
	}

	void DHTrellis::UpdateDisplay(){
		for (int current_led = 0; current_led < TRELLIS_NUM_OF_BUTTONS; current_led++) {
			if (trellis_led_buffer_[current_led]) {
				trellis_.setLED(current_led);
			}
			else {
				trellis_.clrLED(current_led);
			}
		}
		trellis_.writeDisplay();
	}

	void DHTrellis::ClearBuffer() {
		//Clear the entire grid
		for (int button_num = 0; button_num < TRELLIS_NUM_OF_BUTTONS; button_num++) {
			trellis_led_buffer_[button_num] = false;
		}
	}

}