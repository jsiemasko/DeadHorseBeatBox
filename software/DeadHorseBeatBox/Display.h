// Display.h

#ifndef _DISPLAY_h
#define _DISPLAY_h

#include "DHBB_Options.h"
#include "DHBB_Types.h"
#include "arduino.h"
#include "Clock.h"
#include "Grid.h"
#include "Pattern.h"
#include "SplashScreen.h"

/*  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/) */
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

enum DisplayMode {
	kDisplayModePatternProperties,
};

/*
* Pins in order GND, +3.3, 13, 11, 39,38, 37
*/

class Display
{
 private:
	 DisplayMode display_mode_ = kDisplayModePatternProperties;
	 const static USHORT kNumOfDisplayModes = 1;
	 Pattern * p_pattern_ = 0;
	 Clock * p_clock_ = 0;
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
	 USHORT bargraph[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	 //Oled object
	 U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI oled_{ U8G2_R0, DISPLAY_PIN_CS, DISPLAY_PIN_DC, DISPLAY_PIN_RESET };
	 
	 //DisplayModes
	 void ShowPatternProperties();
	 void ShowTrackSelectDisplay();

	 //Display sub sections
	 void ShowPageHeader();
	 void ShowSteps(USHORT y_offset, USHORT track);
	 void ShowTracks(USHORT y_offset, USHORT current_track);
	 void ShowBargraph(USHORT y_offset, USHORT height, USHORT fall_speed);
	 void DrawStepBox(USHORT x, USHORT y, bool step_accented, bool step_chance_set, bool step_retriggered);

 public:
	Display();
	~Display();
	void GraphicsSetup();
	void SplashHorse();
	void UpdateDisplay(ULONG pulse);
	inline void SetDisplayMode(DisplayMode displayMode) { display_mode_ = displayMode; }
	inline void SetPattern(Pattern * p_pattern) { p_pattern_ = p_pattern; }
	inline void SetClock(Clock * p_clock) { p_clock_ = p_clock; }
	inline void SetGrid(Grid * p_grid) { p_grid_ = p_grid; }
	static const USHORT kDisplayHeight = 64;
	static const USHORT kDisplayWidth = 128;
};

#endif

