// TempoLed.h

#ifndef _TEMPOLED_h
#define _TEMPOLED_h

#include "DHBB_Options.h"
#include "arduino.h"
#include "Led.h"

typedef unsigned short int USHORT;
typedef unsigned int ULONG;
typedef unsigned int UINT;

class TempoLed: public Led
{
private:
	short int led_brightness = 255;

 public:
	 TempoLed(USHORT pin);
	 ~TempoLed();
	 
	 //Blink on the quarter notes
	 inline void UpdateDisplay(ULONG pulse) {
		 USHORT step_number = (pulse / PULSE_PER_STEP) - 1;
		 bool is_quarter_note = (step_number % TEMPO_LED_BLINK_FREQUENCY == 0);
		 if (is_quarter_note) {
			 led_brightness = 255;
		 }
		 else {
			 led_brightness = (led_brightness > 0) ? led_brightness - 32 : 0;
		 }

		 analogWrite(pin_, led_brightness);
	 }
};

#endif

