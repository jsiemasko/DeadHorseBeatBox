// Display.h

#ifndef _DISPLAYVIEW_h
#define _DISPLAYVIEW_h

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

namespace Display {
	enum DisplayMode {
		kDisplayModePatternProperties,
	};

	/*
	* Pins in order GND, +3.3, 13, 11, 39,38, 37
	*/

	class DisplayView
	{
	private:
		DisplayMode display_mode_ = kDisplayModePatternProperties;
		const static USHORT kNumOfDisplayModes = 1;
		Midi::MidiManager * p_midi_manager_ = 0;
		Song::Pattern * p_pattern_ = 0;
		Song::Clock * p_clock_ = 0;
		Grid * p_grid_ = 0;
		const static USHORT kCharHeight = 9;
		const static USHORT kCharWidth = 8;
		const static USHORT kNumOfModes = 7;

		//These values are updated once per display
		GridMode default_grid_mode_ = kGridModeAccentEdit;

		//These arrays are used for easy display of mode initials, track nums and step nums
		const char track_labels[16] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6' };
		const char mode_initials[kNumOfModes] = { 'A', 'C', 'R', 'N', 'J', 'S' };

		//Used to track the current values of the bargraph
		USHORT bargraph_[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		ULONG last_bargraph_update_ = millis();

		//Oled object
		U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI oled_{ U8G2_R0, DISPLAY_PIN_CS, DISPLAY_PIN_DC, DISPLAY_PIN_RESET };

		//DisplayModes
		void ShowPatternProperties();
		void ShowTrackSelectDisplay();

		//Display sub sections
		void ShowPageHeader();
		void ShowSteps(USHORT y_offset, USHORT track);
		void ShowTracks(USHORT y_offset, USHORT current_track);
		void ShowBargraph(USHORT y_offset, USHORT accent_height, USHORT std_height, USHORT fall_speed);
		void DrawStepBox(USHORT x, USHORT y, bool step_accented, bool step_chance_set, bool step_retriggered);

	public:
		DisplayView(Midi::MidiManager * p_midi_manager, Grid * p_grid, Song::Pattern * p_pattern, Song::Clock * p_clock);
		~DisplayView();
		void GraphicsSetup();
		void SplashHorse();
		void UpdateDisplay(ULONG pulse);
		inline void SetDisplayMode(DisplayMode display_mode) { display_mode_ = display_mode; }
		static const USHORT kDisplayHeight = 64;
		static const USHORT kDisplayWidth = 128;
	};
}
#endif

