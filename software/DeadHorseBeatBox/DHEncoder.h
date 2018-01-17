#ifndef _DHENCODER_h
#define _DHENCODER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Encoder.h>
#include "Button.h"

namespace Controls
{
	class DHEncoder
	{
	private:
		//Encoder Setup
		Button button_ = Button(ENCODER_BTN_PIN);
		Encoder encoder_ = Encoder(ENCODER_PIN_1, ENCODER_PIN_2);
		USHORT scale_ = ENCODER_SCALE;
		long value_ = 0;

	public:
		DHEncoder();
		~DHEncoder();
		inline bool IsPressed() { return button_.IsPressed(); }
		inline bool JustPressed() { return button_.JustPressed(); }
		inline bool JustReleased() { return button_.JustReleased(); }
		inline long GetValue() { return value_; }
		inline bool HasValue() { return (value_ != 0) ? true : false; }
		void Update();
	};
}
#endif