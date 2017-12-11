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
	inline bool GetEnableState(USHORT track, USHORT step) { return _tracks[track].GetEnableState(step); }
	inline void ToggleEnableState(USHORT track, USHORT step) { _tracks[track].ToggleEnableState(step); }

	// SKIP STATE
	inline bool GetSkipState(USHORT track, USHORT step) { return _tracks[track].GetSkipState(step); }
	inline void ToggleSkipState(USHORT track, USHORT step) { _tracks[track].ToggleSkipState(step); }

	// ACCENT STATE
	inline USHORT GetAccent(USHORT track, USHORT step) { return _tracks[track].GetAccent(step); }
	inline void ToggleAccent(USHORT track, USHORT step) { _tracks[track].ToggleAccent(step); }

	// PROBABILITY STATE
	inline USHORT GetProbability(USHORT track, USHORT step) { return _tracks[track].GetProbability(step); }
	inline void ToggleProbability(USHORT track, USHORT step) { _tracks[track].ToggleProbability(step); }

	// TIMING
	inline USHORT GetCursorPosition(USHORT track) { return _tracks[track].GetCursorPosition(); }
	void ProcessPulse(ULONG pulse);

	// BURST MULTIPLIER
	inline USHORT GetBurstMultiplier(USHORT track, USHORT step) { return _tracks[track].GetBurstMultiplier(step); }
	inline void ToggleBurstMultiplier(USHORT track, USHORT step) { _tracks[track].ToggleBurstMultiplier(step); }


	inline USHORT GetCurrentTrack() { return _currentTrack; }
	inline void SetCurrentTrack(USHORT currentTrack) { _currentTrack = currentTrack; }
	inline void IncrementCurrentTrack() { _currentTrack = (_currentTrack + 1) % NUM_OF_TRACKS; }
	inline void DecrementCurrentTrack() {
		if (_currentTrack == 0) {
			_currentTrack = NUM_OF_TRACKS - 1;
		} else {
			_currentTrack = _currentTrack - 1;
		}
	}
private:
	MidiManager * p_midi_manager_ = 0;
	Track _tracks[NUM_OF_TRACKS] = { 
		{ 0, 36 },	{ 1, 38 },	{ 2, 42 },	{ 3, 46 }, 
		{ 4, 37 },	{ 5, 39 },	{ 6, 40 },	{ 7, 49 }, 
		{ 8, 53 },	{ 9, 41 },	{ 10, 43 },	{ 11, 56 }, 
		{ 12, 54 }, { 13, 55 },	{ 14, 60 },	{ 15, 61 } 
	};
	USHORT _currentTrack = 0;
};

#endif