#ifndef _GRID_h
#define _GRID_h

#include "DHBB_Options.h"
#include "DHBB_Types.h"
#include "Button.h"
#include "Led.h"
#include "arduino.h"
#include <Wire.h>
#include "Pattern.h"
#include "Track.h"
#include "Clock.h"
#include "DHEncoder.h"
#include "DHTrellis.h"

enum GridMode {
	kGridModeSelectTrack,
	kGridModeAccentEdit,
	kGridModeChanceEdit,
	kGridModeSkipEdit,
	kGridModeRetriggerEdit,
	kGridModeNoteEdit,
	kGridModeJumpEdit
};

class Grid
{
 private:
	 Pattern * p_pattern_ = 0;			//Pointer to the current pattern object
	 Clock * p_clock_ = 0;				//Pointer to the current clock object
	 MidiManager * p_midi_manager_ = 0; //Pointer to the current Midi Manager object
	 ULONG current_pulse_ = 0;			//Current pulse that is being processed
	 long throttle_previous_ms_ = 0;	//Throttle counter for control reads
	 
	 //Controls Setup
	 Button track_select_button_ = Button(TRACK_SELECT_BTN_PIN);
	 Led track_select_led_ = Led(TRACK_SELECT_LED_PIN);
	 Button function_select_button_ = Button(FUNCTION_SELECT_BTN_PIN);
	 Controls::DHEncoder encoder_ = Controls::DHEncoder();
	 Controls::DHTrellis trellis_ = Controls::DHTrellis();

	 //Current grid mode and grid mode to return to after an action
	 GridMode default_grid_mode_ = kGridModeAccentEdit;
	 GridMode current_grid_mode_ = kGridModeAccentEdit;
	 	 
	 void DisplayPlayingTracks();
	 void DisplaySingleTrackEditMode();
	 void UpdateSelectButtonDisplay();
	 void CheckForModeSwitch();

 public:
	Grid(MidiManager * p_midi_manager);
	~Grid();
	inline void SetPattern(Pattern * p_pattern) { p_pattern_ = p_pattern; }
	inline void SetClock(Clock * p_clock) { p_clock_ = p_clock; }
	inline void SetGridMode(GridMode grid_mode) { current_grid_mode_ = grid_mode; }
	inline GridMode GetGridMode() { return current_grid_mode_; }
	inline GridMode GetDefaultGridMode() { return default_grid_mode_; }
	void ProcessGridButton(USHORT button_num);
	void ReadSwitches();
	void UpdateDisplay(ULONG pulse);
};


#endif