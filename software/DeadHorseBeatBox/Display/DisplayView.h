#ifndef _DISPLAY_DISPLAYVIEW_h
#define _DISPLAY_DISPLAYVIEW_h

#include "arduino.h"
#include "SplashScreen.h"
#include "../Grid.h"
#include "../DHBB_Options.h"
#include "../DHBB_Types.h"
#include "../Song/Clock.h"
#include "../Song/Pattern.h"

/*  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/) */
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

namespace Display 
{
	class DisplayView
	{
	private:
		U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI oled_{ U8G2_R0, DISPLAY_PIN_CS, DISPLAY_PIN_DC, DISPLAY_PIN_RESET }; //Oled object
		const static USHORT kCharHeight = 9;
		const static USHORT kCharWidth = 8;
		const char track_labels[16] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6' };

	public:
		DisplayView();
		~DisplayView();
		inline void ClearBuffer() { oled_.clearBuffer(); };
		inline void SendBuffer() { oled_.sendBuffer(); }
		void DisplayPageHeader(USHORT x, USHORT y, String current_parameter, float tempo);
		void DisplaySplashScreen();
		void DisplayStepBox(USHORT step, USHORT y, bool step_enabled, bool step_accented, bool step_chance_set, bool step_retriggered);
		void DisplayStepBackground(USHORT y);
		void DisplayStepCursor(USHORT step, USHORT y);
		void DisplayTrackNumbers(USHORT y, USHORT current_track);
		void DisplayBargraphBackground(USHORT y, USHORT height);
		void DisplayBar(USHORT y, USHORT graph_height, USHORT bar_number, USHORT bar_value, bool bar_filled);
		static const USHORT kDisplayHeight = 64;
		static const USHORT kDisplayWidth = 128;
	};
}
#endif

