#include "DHLed.h"

namespace Controls {
	DHLed::DHLed(USHORT pin) {
		pin_ = pin;
		pinMode(pin_, OUTPUT);
		digitalWrite(pin_, true);
	}

	DHLed::~DHLed() {}

	void DHLed::UpdateDisplay(ULONG pulse) {
		if (mode_ == kLedModeSolid) {
			TurnLightOn();
		}
		else if (mode_ == kLedModeFlash) {
			if ((pulse % 32) > 16) {
				TurnLightOn();
			}
			else {
				TurnLightOff();
			}
		}
		else {
			TurnLightOff();
		}
	}
}