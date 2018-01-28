#ifndef _GRID_h
#define _GRID_h

#include "DHBB_Options.h"
#include "DHBB_Types.h"
#include "arduino.h"
#include <Wire.h>
#include "Clock.h"
#include "Song/Pattern.h"
#include "Song/Track.h"
#include "Controls/DHButton.h"
#include "Controls/DHEncoder.h"
#include "Controls/DHTrellis.h"
#include "Controls/DHLed.h"
#include "Controls/DHTempoLed.h"

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
	 Song::Pattern * p_pattern_ = 0;			//Pointer to the current pattern object
	 Clock * p_clock_ = 0;				//Pointer to the current clock object
	 DHMidi::MidiManager * p_midi_manager_ = 0; //Pointer to the current Midi Manager object
	 ULONG current_pulse_ = 0;			//Current pulse that is being processed
	 long throttle_previous_ms_ = 0;	//Throttle counter for control reads
	 
	 //Controls Setup
	 Controls::DHTempoLed tempo_led_ = Controls::DHTempoLed(TEMPO_LED_PIN);
	 Controls::DHLed track_select_led_ = Controls::DHLed(TRACK_SELECT_LED_PIN);
	 Controls::DHButton track_select_button_ = Controls::DHButton(TRACK_SELECT_BTN_PIN);
	 Controls::DHButton function_select_button_ = Controls::DHButton(FUNCTION_SELECT_BTN_PIN);
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
	Grid(DHMidi::MidiManager * p_midi_manager, Song::Pattern * p_pattern, Clock * p_clock);
	~Grid();
	inline void SetGridMode(GridMode grid_mode) { current_grid_mode_ = grid_mode; }
	inline GridMode GetGridMode() { return current_grid_mode_; }
	inline GridMode GetDefaultGridMode() { return default_grid_mode_; }
	void ProcessGridButton(USHORT button_num);
	void ReadSwitches();
	void UpdateDisplay(ULONG pulse);
};

#endif