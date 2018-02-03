#ifndef _GRID_h
#define _GRID_h

#include "DHBB_Options.h"
#include "DHBB_Types.h"
#include "arduino.h"
#include <Wire.h>
#include "Song/Clock.h"
#include "Song/Pattern.h"
#include "Song/Track.h"
#include "Controls/Button.h"
#include "Controls/Encoder.h"
#include "Controls/Trellis.h"
#include "string.h"

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
	 Song::Clock * p_clock_ = 0;				//Pointer to the current clock object
	 Midi::MidiManager * p_midi_manager_ = 0; //Pointer to the current Midi Manager object
	 ULONG current_pulse_ = 0;			//Current pulse that is being processed
	 long throttle_previous_ms_ = 0;	//Throttle counter for control reads
	 
	 //Controls Setup
	 Controls::Button track_select_button_ = Controls::Button(TRACK_SELECT_BTN_PIN);
	 Controls::Button function_select_button_ = Controls::Button(FUNCTION_SELECT_BTN_PIN);
	 Controls::Encoder encoder_ = Controls::Encoder();
	 Controls::Trellis trellis_ = Controls::Trellis();

	 //Current grid mode and grid mode to return to after an action
	 GridMode default_grid_mode_ = kGridModeAccentEdit;
	 GridMode current_grid_mode_ = kGridModeAccentEdit;
	 	 
	 void DisplayPlayingTracks();
	 void DisplaySingleTrackEditMode();
	 void CheckForModeSwitch();

 public:
	Grid(Midi::MidiManager * p_midi_manager, Song::Pattern * p_pattern, Song::Clock * p_clock);
	~Grid();
	inline void SetGridMode(GridMode grid_mode) { current_grid_mode_ = grid_mode; }
	inline GridMode GetGridMode() { return current_grid_mode_; }
	inline GridMode GetDefaultGridMode() { return default_grid_mode_; }
	inline bool TrackSelectButtonIsPressed() { return track_select_button_.IsPressed(); }
	void ProcessGridButton(USHORT button_num);
	void ReadSwitches();
	void UpdateDisplay(ULONG pulse);
	String GetCurrentEditParameter();
};

#endif