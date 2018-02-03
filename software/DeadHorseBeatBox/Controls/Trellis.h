#ifndef _CONTROLS_TRELLIS_h
#define _CONTROLS_TRELLIS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "../DHBB_Types.h"
#include "../DHBB_Options.h"
#include "Adafruit_Trellis.h"

namespace Controls
{
	class Trellis
	{
	private:
		bool trellis_led_buffer_[32];	//Holds the current state of the LEDs before writing
		USHORT grid_renumber_[32] = {	//Used to fix the LED numbering for our Trellis arrangement
			0,  1,  2,  3, 16, 17, 18, 19, 
			4,  5,  6,  7, 20, 21, 22, 23,
			8,  9, 10, 11, 24, 25, 26, 27,
			12, 13, 14, 15, 28, 29, 30, 31
		};
		bool cancel_press_[32];			//When moving the encoder while holding a button we want to cancel the button press action
		//Trellis Setup
		Adafruit_Trellis trellis_matrix0_ = Adafruit_Trellis();
		Adafruit_Trellis trellis_matrix1_ = Adafruit_Trellis();
		Adafruit_TrellisSet trellis_ = Adafruit_TrellisSet(&trellis_matrix0_, &trellis_matrix1_);

		void ClearBuffer();

	public:
		Trellis();
		~Trellis();
		inline void SetBuffer(USHORT row, USHORT step, bool state) { trellis_led_buffer_[grid_renumber_[(row * TRELLIS_BUTTONS_PER_ROW) + step]] = state; }
		inline bool JustReleased(USHORT button) { return trellis_.justReleased(grid_renumber_[button]); }
		void ReadSwitches();
		void UpdateDisplay();
	};
}
#endif