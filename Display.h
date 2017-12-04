// Display.h

#ifndef _DISPLAY_h
#define _DISPLAY_h

#include "DHBB_Options.h"
#include "arduino.h"
#include "Clock.h"

/*  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/) */
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include "Pattern.h"

typedef unsigned short int USHORT;
typedef unsigned int ULONG;
typedef unsigned int UINT;

enum DisplayMode {
	DM_TRACK_DISPLAY,
	DM_TRACK_LINES_4,
	DM_TRACK_LINES_8,
	DM_BIG_CLOCK,
	DM_FLASH_TEXT
};

/*
* Pins in order GND, +3.3, 13, 11, 39,38, 37
*/

class Display
{
 private:
	 DisplayMode _displayMode = DM_TRACK_LINES_8;
	 DisplayMode _previousDisplayMode = DM_BIG_CLOCK;
	 const USHORT NUM_OF_DISPLAY_MODES = 4;
	 USHORT _revertDisplayCountdown = 0;
	 Pattern * _pPattern = 0;
	 Clock * _pClock = 0;
	 U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI oled_{ U8G2_R0, DISPLAY_PIN_CS, DISPLAY_PIN_DC, DISPLAY_PIN_RESET };
	 
	 //DisplayModes
	 void ShowBigClock(ULONG pulse);
	 void ShowTrackLines(ULONG pulse, USHORT numOfTracks, USHORT selectedTrack);
	 void ShowTrackDisplay(ULONG pulse, USHORT selectedTrack);
	 //void ShowFlashText(ULONG pulse, USHORT numOfTracks, USHORT trackOffset);
	 inline void revertToPreviousDisplayMode() { DisplayMode swapDisplayMode = _displayMode; _displayMode = _previousDisplayMode; _previousDisplayMode = swapDisplayMode; };

 public:
	Display();
	~Display();
	void GraphicsSetup();
	void SplashHorse(USHORT displayTime);
	void UpdateDisplay(ULONG pulse);
	inline void SetDisplayMode(DisplayMode displayMode) { _previousDisplayMode = _displayMode; _displayMode = displayMode; }
	inline void CycleDisplayMode() { _previousDisplayMode = _displayMode; _displayMode = (DisplayMode)((_displayMode + 1) % 4); }
	inline void SetPattern(Pattern * pPattern) { _pPattern = pPattern; }
	inline void SetClock(Clock * pClock) { _pClock = pClock; }
	static const USHORT DISPLAY_HEIGHT = 64;
	static const USHORT DISPLAY_WIDTH = 128;
};

#endif

