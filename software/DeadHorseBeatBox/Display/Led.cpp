#include "Led.h"

namespace Display {
	Led::Led(USHORT pin) {
		pin_ = pin;
		pinMode(pin_, OUTPUT);
		digitalWrite(pin_, true);
	}

	Led::~Led() {}

	void Led::UpdateDisplay(ULONG pulse) {
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