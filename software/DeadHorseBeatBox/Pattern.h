// Pattern.h

#ifndef _PATTERN_h
#define _PATTERN_h

#include "DHBB_Options.h"
#include "arduino.h"
#include "Track.h"

typedef unsigned short int USHORT;
typedef unsigned int ULONG;
typedef unsigned int UINT;

class Pattern
{
 public:
	Pattern(MidiManager * p_midi_manager);
	~Pattern();

	inline bool GetTrackNotePlaying(USHORT track) { return p_midi_manager_->EventPlaying(track); }

	// ENABLE STATE
	inline bool GetEnableState(USHORT track, USHORT step) { return tracks_[track].GetStep(step).GetEnableState(); }
	inline void ToggleEnableState(USHORT track, USHORT step) { selected_step_ = step; tracks_[track].GetStep(step).ToggleEnableState(); }

	// SKIP STATE
	inline bool GetSkipState(USHORT track, USHORT step) { return tracks_[track].GetStep(step).GetSkipState(); }
	inline void ToggleSkipState(USHORT track, USHORT step) { selected_step_ = step; tracks_[track].GetStep(step).ToggleSkipState(); }

	// ACCENT STATE
	inline bool GetAccent(USHORT track, USHORT step) { return tracks_[track].GetStep(step).GetAccentState(); }
	inline void ToggleAccent(USHORT track, USHORT step) { selected_step_ = step; tracks_[track].GetStep(step).ToggleAccentState(); }

	// CHANCE
	inline bool GetChance(USHORT track, USHORT step) { return tracks_[track].GetStep(step).GetChanceState(); }
	inline void ToggleChance(USHORT track, USHORT step) { selected_step_ = step; tracks_[track].GetStep(step).ToggleChanceState(); }

	// TIMING
	inline USHORT GetCursorPosition(USHORT track) { return tracks_[track].GetCursorPosition(); }
	void ProcessPulse(ULONG pulse);

	// RETRIGGER
	inline bool GetRetrigger(USHORT track, USHORT step) { return tracks_[track].GetStep(step).GetRetriggerState(); }
	inline void ToggleRetrigger(USHORT track, USHORT step) { selected_step_ = step; tracks_[track].GetStep(step).ToggleRetriggerState(); }

	// NOTE
	inline bool GetNote(USHORT track, USHORT step) { return tracks_[track].GetStep(step).GetNoteState(); }
	inline void ToggleNote(USHORT track, USHORT step) { selected_step_ = step; tracks_[track].GetStep(step).ToggleNoteState(); }

	inline USHORT GetCurrentTrack() { return current_track_; }
	inline USHORT GetSelectedStep() { return selected_step_; }
	inline void SetCurrentTrack(USHORT current_track) { current_track_ = current_track; }
	inline void IncrementCurrentTrack() { current_track_ = (current_track_ + 1) % NUM_OF_TRACKS; }
	inline void DecrementCurrentTrack() {
		if (current_track_ == 0) {
			current_track_ = NUM_OF_TRACKS - 1;
		} else {
			current_track_ = current_track_ - 1;
		}
	}
private:
	MidiManager * p_midi_manager_ = 0;
	Track tracks_[NUM_OF_TRACKS] = { 
		{ 0, 36 },	{ 1, 38 },	{ 2, 42 },	{ 3, 46 }, 
		{ 4, 37 },	{ 5, 39 },	{ 6, 40 },	{ 7, 49 }, 
		{ 8, 53 },	{ 9, 41 },	{ 10, 43 },	{ 11, 56 }, 
		{ 12, 54 }, { 13, 55 },	{ 14, 60 },	{ 15, 61 } 
	};
	USHORT current_track_ = 0;
	USHORT selected_step_ = 0;
};

#endif