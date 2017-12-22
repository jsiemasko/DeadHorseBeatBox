// Led.h

#ifndef _LED_h
#define _LED_h

#include "DHBB_Options.h"
#include "arduino.h"

typedef unsigned short int USHORT;
typedef unsigned int ULONG;
typedef unsigned int UINT;

enum LedMode {
	kLedModeSolid,
	kLedModeFlash,
	kLedModeOff
};

class Led
{
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

#endif

