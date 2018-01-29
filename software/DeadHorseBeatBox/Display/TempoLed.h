#ifndef _DHTEMPOLED_h
#define _DHTEMPOLED_h

#include "../DHBB_Options.h"
#include "../DHBB_Types.h"
#include "Led.h"
#include "arduino.h"

namespace Display {
	class TempoLed : public Led {
	private:
		short int led_brightness_ = 255;

	public:
		TempoLed(USHORT pin);
		~TempoLed();

		//Blink on the quarter notes
		inline void UpdateDisplay(ULONG pulse) {
			USHORT step_number = (pulse / PULSE_PER_STEP) - 1;
			bool is_quarter_note = (step_number % TEMPO_LED_BLINK_FREQUENCY == 0);
			if (is_quarter_note) {
				led_brightness_ = 255;
			}
			else {
				led_brightness_ = (led_brightness_ > 0) ? led_brightness_ - 32 : 0;
			}

			analogWrite(pin_, led_brightness_);
		}
	};
}

#endif