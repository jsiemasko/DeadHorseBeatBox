#ifndef _DISPLAY_LED_h
#define _DISPLAY_LED_h

#include "../DHBB_Options.h"
#include "../DHBB_Types.h"
#include "arduino.h"

namespace Display {
	enum LedMode {
		kLedModeSolid,
		kLedModeFlash,
		kLedModeOff
	};

	class Led {
	protected:
		//Pin numbers
		USHORT pin_ = 0;

		//LED mode controls the flashing pattern of the LED when display is updated
		LedMode mode_ = kLedModeSolid;

		//Control LEDs
		inline void TurnLightOn() { digitalWrite(pin_, true); }
		inline void TurnLightOff() { digitalWrite(pin_, false); }

	public:
		//Constructor
		Led(USHORT pin);
		~Led();

		//Display
		inline void SetMode(LedMode mode) { mode_ = mode; }
		virtual void UpdateDisplay(ULONG pulse);
	};
}
#endif

